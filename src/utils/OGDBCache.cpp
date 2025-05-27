#include "OGDBCache.hpp"

using namespace geode;
using namespace std::chrono;

void OGDBCache::set(std::string key, std::string rawData) {
    auto cacheMode = OGDBSetting::get("GENERAL_CACHEMODE")->getValue<int>();
    auto expireMinutes = OGDBSetting::get("GENERAL_CACHETIMEEXPIRE")->getValue<float>();
    auto expireMs = static_cast<int64_t>(expireMinutes * 60000.0f);

    auto now = system_clock::now();
    auto nowMs = duration_cast<milliseconds>(now.time_since_epoch()).count();
    auto expireTime = nowMs + expireMs;

    auto encodedKey = encodeKey(key);
    auto encodedData = utils::base64::encode(rawData, utils::base64::Base64Variant::UrlWithPad);

    CacheEntry entry{ encodedData, expireTime };

    if (cacheMode == 0) {
        auto rawCache = Mod::get()->getSavedValue<matjson::Value>("OGDB_CACHE", matjson::Value::array());
        auto cacheListResult = rawCache.asArray();
        if (!cacheListResult.isOk()) return;
        std::vector<matjson::Value> cacheList = cacheListResult.unwrap();

        bool replaced = false;
        for (auto& item : cacheList) {
            if (!item.contains("key")) continue;

            auto keyValResult = item["key"].asString();
            if (!keyValResult.isOk()) continue;
            std::string keyVal = keyValResult.unwrap();

            if (keyVal == encodedKey) {
                item["data"] = entry.data;
                item["expire_time"] = entry.expireTime;
                replaced = true;
                break;
            }
        }

        if (!replaced) {
            matjson::Value newEntry = matjson::Value::object();
            newEntry["key"] = encodedKey;
            newEntry["data"] = entry.data;
            newEntry["expire_time"] = entry.expireTime;
            cacheList.push_back(newEntry);
        }

        Mod::get()->setSavedValue("OGDB_CACHE", matjson::Value(cacheList));
    }
    else {
        memoryCache[encodedKey] = entry;
    }

    cleanUp();
}

std::optional<std::string> OGDBCache::get(std::string key) {
    auto encodedKey = encodeKey(key);
    auto cacheMode = OGDBSetting::get("GENERAL_CACHEMODE")->getValue<int>();
    auto now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

    if (cacheMode == 0) {
        auto rawCache = Mod::get()->getSavedValue<matjson::Value>("OGDB_CACHE", matjson::Value::array());
        auto cacheListResult = rawCache.asArray();
        if (!cacheListResult.isOk()) return std::nullopt;
        std::vector<matjson::Value> cacheList = cacheListResult.unwrap();

        for (auto& item : cacheList) {
            if (!item.contains("key")) continue;

            auto keyValResult = item["key"].asString();
            if (!keyValResult.isOk()) continue;
            std::string keyVal = keyValResult.unwrap();

            if (keyVal == encodedKey) {
                if (!item.contains("expire_time")) return std::nullopt;
                auto expireTimeResult = item["expire_time"].asInt();
                if (!expireTimeResult.isOk()) return std::nullopt;
                int64_t expireTime = expireTimeResult.unwrap();

                if (expireTime < now) return std::nullopt;

                if (!item.contains("data")) return std::nullopt;
                auto dataResult = item["data"].asString();
                if (!dataResult.isOk()) return std::nullopt;
                std::string dataStr = dataResult.unwrap();

                auto decodedResult = utils::base64::decodeString(dataStr, utils::base64::Base64Variant::UrlWithPad);
                if (!decodedResult.isOk()) return std::nullopt;

                return decodedResult.unwrap();
            }
        }

        return std::nullopt;
    }
    else {
        if (!memoryCache.contains(encodedKey)) return std::nullopt;
        auto& entry = memoryCache[encodedKey];

        if (entry.expireTime < now) {
            memoryCache.erase(encodedKey);
            return std::nullopt;
        }

        auto decodedResult = utils::base64::decodeString(entry.data, utils::base64::Base64Variant::UrlWithPad);
        if (!decodedResult.isOk()) return std::nullopt;
        return decodedResult.unwrap();
    }
}

bool OGDBCache::exists(std::string key) {
    return get(key).has_value();
}

std::string OGDBCache::encodeKey(const std::string& key) {
    return utils::base64::encode(key, utils::base64::Base64Variant::UrlWithPad);
}

std::string OGDBCache::decodeKey(const std::string& b64key) {
    auto decodedResult = utils::base64::decodeString(b64key, utils::base64::Base64Variant::UrlWithPad);
    if (!decodedResult.isOk()) return "";
    return decodedResult.unwrap();
}

void OGDBCache::cleanUp() {
    auto cacheMode = OGDBSetting::get("GENERAL_CACHEMODE")->getValue<int>();
    auto sizeLimit = OGDBSetting::get("GENERAL_CACHESIZEMBLIMIT")->getValue<int>();
    size_t maxBytes = sizeLimit * 1024 * 1024;

    if (cacheMode == 0) {
        auto rawCache = Mod::get()->getSavedValue<matjson::Value>("OGDB_CACHE", matjson::Value::array());
        auto cacheListResult = rawCache.asArray();
        if (!cacheListResult.isOk()) return;
        std::vector<matjson::Value> cacheList = cacheListResult.unwrap();

        auto now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

        std::vector<matjson::Value> newCache;
        for (auto& item : cacheList) {
            if (!item.contains("expire_time")) continue;
            auto expireTimeResult = item["expire_time"].asInt();
            if (!expireTimeResult.isOk()) continue;
            int64_t expireTime = expireTimeResult.unwrap();

            if (expireTime >= now) {
                newCache.push_back(item);
            }
        }

        while (matjson::Value(newCache).dump().size() > maxBytes && !newCache.empty()) {
            newCache.erase(newCache.begin());
        }

        Mod::get()->setSavedValue("OGDB_CACHE", matjson::Value(newCache));
    }
    else {
        auto now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

        for (auto it = memoryCache.begin(); it != memoryCache.end();) {
            if (it->second.expireTime < now) {
                it = memoryCache.erase(it);
            }
            else {
                ++it;
            }
        }

        while (calculateSizeInMB(memoryCache) > static_cast<size_t>(sizeLimit)) {
            if (memoryCache.empty()) break;
            memoryCache.erase(memoryCache.begin());
        }
    }
}

size_t OGDBCache::calculateSizeInMB(const std::unordered_map<std::string, CacheEntry>& map) {
    size_t totalBytes = 0;
    for (const auto& [k, v] : map) {
        totalBytes += k.size() + v.data.size() + sizeof(v.expireTime);
    }
    return totalBytes / (1024 * 1024);
}
