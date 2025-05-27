// #pragma once
// #include <Geode/Geode.hpp>
// #include <Geode/utils/web.hpp>
// #include <Geode/loader/Event.hpp>
// #include <matjson/stl_serialize.hpp>
// #include <functional>

// using namespace geode::prelude;

// class OGDBRequests {
// public:
//     using SuccessCallback = std::function<void(matjson::Value)>;
//     using ErrorCallback = std::function<void(std::string)>;
//     using ProgressCallback = std::function<void(float)>;
//     using CancelCallback = std::function<void()>;
    

//     std::string m_baseURL = "https://api.obeygdteam.site/mod/obeygdtools/";
//     std::string m_platformRequest = GEODE_PLATFORM_NAME; 

//     struct Fields {
//         EventListener<web::WebTask> listener;
//         web::WebTask currentTask; 
//         SuccessCallback onSuccess = nullptr;
//         ErrorCallback onError = nullptr;
//         ProgressCallback onProgress = nullptr;
//         CancelCallback onCancel = nullptr;
//     };

//     Fields m_fields;
//     web::WebRequest m_request; 

//     OGDBRequests() {
//         log::info("OGDBRequests: Listener initialized");

//         m_fields.listener.bind([this](web::WebTask::Event* e) {
//             if (auto* res = e->getValue()) {
//                 log::info("OGDBRequests: Response received");
//                 log::info("{}", res->string().unwrapOr("Uh oh!"));

//                 if (m_fields.onSuccess && res->ok()) {
//                     log::info("is OK!");
//                     auto contentType = res->header("content-type").value_or("");
//                     // log::info("HEADER: {}", contentType);
//                     // auto contentType2 = res->header("access-control-allow-headers").value_or("");
//                     // log::info("HEADER: {}", contentType2);
//                     if (contentType.find("application/json") != std::string::npos) {
//                         // log::info("is APP JSON!");
//                         auto result = res->json();
//                         if (result.isOk()) {
//                             m_fields.onSuccess(result.unwrap());
//                         } else {
//                             if (m_fields.onError)
//                                 m_fields.onError("Failed to parse JSON response.");
//                         }
//                     } else {
//                         log::info("is STRING!");
//                         auto body = res->string();
//                         m_fields.onSuccess(matjson::Value(body.unwrapOr("")));
//                     }
//                 } else if (m_fields.onError) {
//                     auto errStr = res->string().unwrapOr("Unknown error");
//                     m_fields.onError(fmt::format("HTTP {}: {}", res->code(), errStr));
//                 }
//             } else if (auto* prog = e->getProgress()) {
//                 if (m_fields.onProgress)
//                     m_fields.onProgress(prog->downloadProgress().value_or(0.f));
//             } else if (e->isCancelled()) {
//                 if (m_fields.onCancel)
//                     m_fields.onCancel();
//             }
//         });
//     }

//     void get(
//         const std::string& endpoint,
//         SuccessCallback onSuccess,
//         ErrorCallback onError,
//         ProgressCallback onProgress = nullptr,
//         CancelCallback onCancel = nullptr
//     ) {
//         storeCallbacks(onSuccess, onError, onProgress, onCancel);
//         log::info("OGDBRequests: GET request started");

//         m_request = web::WebRequest(); 
//         m_request.header("User-Agent", fmt::format("{}/{} {}", Mod::get()->getID(), Mod::get()->getVersion(), m_platformRequest));
//         // m_request.header("Content-Type", "application/json");
        
//         m_fields.currentTask = m_request.get(m_baseURL + endpoint);
//         m_fields.listener.setFilter(m_fields.currentTask);
//     }

//     void post(
//         const std::string& endpoint,
//         const matjson::Value& payload,
//         SuccessCallback onSuccess,
//         ErrorCallback onError,
//         ProgressCallback onProgress = nullptr,
//         CancelCallback onCancel = nullptr
//     ) {
//         storeCallbacks(onSuccess, onError, onProgress, onCancel);
//         log::info("OGDBRequests: POST request started");

//         m_request = web::WebRequest();
//         m_request.header("User-Agent", fmt::format("{}/{} {}", Mod::get()->getID(), Mod::get()->getVersion(), m_platformRequest));
//         // m_request.header("Content-Type", "application/json");
//         m_request.bodyJSON(payload);

//         m_fields.currentTask = m_request.post(m_baseURL + endpoint);
//         m_fields.listener.setFilter(m_fields.currentTask);
//     }

//     void cancelCurrentRequest() {
//         if (!m_fields.currentTask.isCancelled()) {
//             m_fields.currentTask.cancel();
//             log::info("OGDBRequests: Request cancelled.");
//         }
//     }

// private:
//     void storeCallbacks(
//         SuccessCallback onSuccess,
//         ErrorCallback onError,
//         ProgressCallback onProgress,
//         CancelCallback onCancel
//     ) {
//         m_fields.onSuccess = onSuccess;
//         m_fields.onError = onError;
//         m_fields.onProgress = onProgress;
//         m_fields.onCancel = onCancel;
//     }
// };


// #pragma once
// #include <Geode/Geode.hpp>
// #include <Geode/utils/web.hpp>
// #include <Geode/loader/Event.hpp>
// #include <matjson/stl_serialize.hpp>
// #include <functional>
// #include "OGDBCache.hpp"

// using namespace geode::prelude;

// class OGDBRequests {
// public:
//     using SuccessCallback = std::function<void(matjson::Value)>;
//     using ErrorCallback = std::function<void(std::string)>;
//     using ProgressCallback = std::function<void(float)>;
//     using CancelCallback = std::function<void()>;

//     std::string m_baseURL = "https://api.obeygdteam.site/mod/obeygdtools/";
//     std::string m_platformRequest = GEODE_PLATFORM_NAME;

//     struct Fields {
//         EventListener<web::WebTask> listener;
//         web::WebTask currentTask;
//         SuccessCallback onSuccess = nullptr;
//         ErrorCallback onError = nullptr;
//         ProgressCallback onProgress = nullptr;
//         CancelCallback onCancel = nullptr;
//     };

//     Fields m_fields;
//     web::WebRequest m_request;

//     OGDBRequests() {
//         log::info("OGDBRequests: Listener initialized");

//         m_fields.listener.bind([this](web::WebTask::Event* e) {
//             if (auto* res = e->getValue()) {
//                 log::info("OGDBRequests: Response received");
//                 log::info("{}", res->string().unwrapOr("Uh oh!"));

//                 if (m_fields.onSuccess && res->ok()) {
//                     auto contentType = res->header("content-type").value_or("");
//                     if (contentType.find("application/json") != std::string::npos) {
//                         auto result = res->json();
//                         if (result.isOk()) {
//                             m_fields.onSuccess(result.unwrap());


                            
//                         } else {
//                             if (m_fields.onError)
//                                 m_fields.onError("Failed to parse JSON response.");
//                         }
//                     } else {
//                         auto body = res->string();
//                         m_fields.onSuccess(body.unwrapOr(""));

//                     }
//                 } else if (m_fields.onError) {
//                     auto errStr = res->string().unwrapOr("Unknown error");
//                     m_fields.onError(fmt::format("HTTP {}: {}", res->code(), errStr));
//                 }
//             } else if (auto* prog = e->getProgress()) {
//                 if (m_fields.onProgress)
//                     m_fields.onProgress(prog->downloadProgress().value_or(0.f));
//             } else if (e->isCancelled()) {
//                 if (m_fields.onCancel)
//                     m_fields.onCancel();
//             }
//         });
//     }

//     void get(
//         const std::string& endpoint,
//         SuccessCallback onSuccess,
//         ErrorCallback onError,
//         ProgressCallback onProgress = nullptr,
//         CancelCallback onCancel = nullptr
//     ) {
//         storeCallbacks(onSuccess, onError, onProgress, onCancel);

//         std::string fullURL = m_baseURL + endpoint;


//         auto cached = OGDBCache::get(fullURL);
//         if (cached) {
//             auto cachedStr = cached.value();
//             auto jsonResult = matjson::parse(cachedStr);
//             if (jsonResult.isOk()) {
//                 log::info("OGDBRequests: Cache hit for GET {}", fullURL);
//                 onSuccess(jsonResult.unwrap());
//                 return;
//             }
            
//         }

//         log::info("OGDBRequests: GET request started: {}", fullURL);

//         m_request = web::WebRequest();
//         m_request.header("User-Agent", fmt::format("{}/{} {}", Mod::get()->getID(), Mod::get()->getVersion(), m_platformRequest));
        


//         m_fields.currentTask = m_request.get(fullURL);

//         m_fields.listener.setFilter(m_fields.currentTask);
//     }

//     void post(
//         const std::string& endpoint,
//         const matjson::Value& payload,
//         SuccessCallback onSuccess,
//         ErrorCallback onError,
//         ProgressCallback onProgress = nullptr,
//         CancelCallback onCancel = nullptr
//     ) {
//         storeCallbacks(onSuccess, onError, onProgress, onCancel);

//         std::string fullURL = m_baseURL + endpoint;


//         auto cached = OGDBCache::get(fullURL + payload.dump());
//         if (cached) {
//             auto cachedStr = cached.value();
//             auto jsonResult = matjson::parse(cachedStr);
//             if (jsonResult.isOk()) {
//                 log::info("OGDBRequests: Cache hit for POST {}", fullURL);
//                 onSuccess(jsonResult.unwrap());
//                 return;
//             }

//         }

//         log::info("OGDBRequests: POST request started: {}", fullURL);

//         m_request = web::WebRequest();
//         m_request.header("User-Agent", fmt::format("{}/{} {}", Mod::get()->getID(), Mod::get()->getVersion(), m_platformRequest));
//         m_request.bodyJSON(payload);

//         m_fields.currentTask = m_request.post(fullURL);
//         m_fields.listener.setFilter(m_fields.currentTask);
//     }

//     void cancelCurrentRequest() {
//         if (!m_fields.currentTask.isCancelled()) {
//             m_fields.currentTask.cancel();
//             log::info("OGDBRequests: Request cancelled.");
//         }
//     }

// private:
//     void storeCallbacks(
//         SuccessCallback onSuccess,
//         ErrorCallback onError,
//         ProgressCallback onProgress,
//         CancelCallback onCancel
//     ) {
//         m_fields.onSuccess = onSuccess;
//         m_fields.onError = onError;
//         m_fields.onProgress = onProgress;
//         m_fields.onCancel = onCancel;
//     }
// };

#pragma once
#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/loader/Event.hpp>
#include <matjson/stl_serialize.hpp>
#include <functional>
#include <map>          // Para std::map
#include "OGDBCache.hpp"

using namespace geode::prelude;

class OGDBRequests {
public:
    struct Response {
        std::string body;
        std::map<std::string, std::string> headers;
    };

    using SuccessCallback = std::function<void(Response)>;
    using ErrorCallback = std::function<void(std::string)>;
    using ProgressCallback = std::function<void(float)>;
    using CancelCallback = std::function<void()>;

    std::string m_baseURL = "https://api.obeygdteam.site/mod/obeygdtools/";
    std::string m_platformRequest = GEODE_PLATFORM_NAME;

    struct Fields {
        EventListener<web::WebTask> listener;
        web::WebTask currentTask;
        SuccessCallback onSuccess = nullptr;
        ErrorCallback onError = nullptr;
        ProgressCallback onProgress = nullptr;
        CancelCallback onCancel = nullptr;
    };

    Fields m_fields;
    web::WebRequest m_request;

    OGDBRequests() {
        log::info("OGDBRequests: Listener initialized");

        m_fields.listener.bind([this](web::WebTask::Event* e) {
            if (auto* res = e->getValue()) {
                log::info("OGDBRequests: Response received");
                log::info("{}", res->string().unwrapOr("Uh oh!"));

                if (m_fields.onSuccess && res->ok()) {
                    Response resp;
                    resp.body = res->string().unwrapOr("");


                    for (const auto& headerLine : res->headers()) {
                        auto pos = headerLine.find(':');
                        if (pos != std::string::npos) {
                            std::string key = headerLine.substr(0, pos);
                            std::string value = headerLine.substr(pos + 1);
                            value.erase(0, value.find_first_not_of(" \t"));
                            value.erase(value.find_last_not_of(" \t") + 1);
                            resp.headers[key] = value;
                        }
                    }

                    m_fields.onSuccess(resp);

                } else if (m_fields.onError) {
                    auto errStr = res->string().unwrapOr("Unknown error");
                    m_fields.onError(fmt::format("HTTP {}: {}", res->code(), errStr));
                }
            } else if (auto* prog = e->getProgress()) {
                if (m_fields.onProgress)
                    m_fields.onProgress(prog->downloadProgress().value_or(0.f));
            } else if (e->isCancelled()) {
                if (m_fields.onCancel)
                    m_fields.onCancel();
            }
        });
    }

    void get(
        const std::string& endpoint,
        SuccessCallback onSuccess,
        ErrorCallback onError,
        ProgressCallback onProgress = nullptr,
        CancelCallback onCancel = nullptr
    ) {
        storeCallbacks(onSuccess, onError, onProgress, onCancel);

        std::string fullURL = m_baseURL + endpoint;

        auto cached = OGDBCache::get(fullURL);
        if (cached) {
            auto cachedStr = cached.value();
            Response cachedResp;
            cachedResp.body = cachedStr;

            log::info("OGDBRequests: Cache hit for GET {}", fullURL);
            onSuccess(cachedResp);
            return;
        }

        log::info("OGDBRequests: GET request started: {}", fullURL);

        m_request = web::WebRequest();
        m_request.header("User-Agent", fmt::format("{}/{} {}", Mod::get()->getID(), Mod::get()->getVersion(), m_platformRequest));

        m_fields.currentTask = m_request.get(fullURL);

        m_fields.listener.setFilter(m_fields.currentTask);
    }

    void post(
        const std::string& endpoint,
        const matjson::Value& payload,
        SuccessCallback onSuccess,
        ErrorCallback onError,
        ProgressCallback onProgress = nullptr,
        CancelCallback onCancel = nullptr
    ) {
        storeCallbacks(onSuccess, onError, onProgress, onCancel);

        std::string fullURL = m_baseURL + endpoint;

        auto cached = OGDBCache::get(fullURL + payload.dump());
        if (cached) {
            auto cachedStr = cached.value();
            Response cachedResp;
            cachedResp.body = cachedStr;

            log::info("OGDBRequests: Cache hit for POST {}", fullURL);
            onSuccess(cachedResp);
            return;
        }

        log::info("OGDBRequests: POST request started: {}", fullURL);

        m_request = web::WebRequest();
        m_request.header("User-Agent", fmt::format("{}/{} {}", Mod::get()->getID(), Mod::get()->getVersion(), m_platformRequest));
        m_request.bodyJSON(payload);

        m_fields.currentTask = m_request.post(fullURL);
        m_fields.listener.setFilter(m_fields.currentTask);
    }

    void cancelCurrentRequest() {
        if (!m_fields.currentTask.isCancelled()) {
            m_fields.currentTask.cancel();
            log::info("OGDBRequests: Request cancelled.");
        }
    }

private:
    void storeCallbacks(
        SuccessCallback onSuccess,
        ErrorCallback onError,
        ProgressCallback onProgress,
        CancelCallback onCancel
    ) {
        m_fields.onSuccess = onSuccess;
        m_fields.onError = onError;
        m_fields.onProgress = onProgress;
        m_fields.onCancel = onCancel;
    }
};

