#pragma once

#include <Geode/Geode.hpp>
#include <unordered_map>
#include <string>
#include <Geode/utils/base64.hpp>
#include "../settings/OGDBSetting.hpp"

using namespace geode::prelude;

class OGDBCache {
public:
    static void set(std::string key, std::string rawData);
    static std::optional<std::string> get(std::string key);
    static bool exists(std::string key);

private:
    struct CacheEntry {
        std::string data;
        int64_t expireTime;
    };

    static std::string encodeKey(const std::string& key);
    static std::string decodeKey(const std::string& b64key);
    static void cleanUp();
    static size_t calculateSizeInMB(const std::unordered_map<std::string, CacheEntry>& map);

    static inline std::unordered_map<std::string, CacheEntry> memoryCache;
};
