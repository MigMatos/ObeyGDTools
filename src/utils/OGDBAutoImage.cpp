#include "OGDBAutoImage.hpp"
#include "OGDBRequests.hpp"

using namespace geode::prelude;

namespace {

    void onErrorImage(const std::string& message) {
        log::error("OGDBAutoImage: {}", message);
    }

    bool isURL(const std::string& str) {
        return str.starts_with("http://") || str.starts_with("https://");
    }

    CCLayer* onDefaultImage(float ancho, float largo, const char* fallbackPath = "GJ_gradientBG.png" ) {
        auto bgLayer = CCLayer::create();
        bgLayer->setContentSize({ancho, largo});
        bgLayer->setID("ogdb-autoimage-layer"_spr);
        //
        auto ret = CCFileUtils::sharedFileUtils()->fullPathForFilename(fallbackPath, false);
        if (ret.empty()) return bgLayer;
        //
        log::info("TEST RET: {}", ret);

        if (CCFileUtils::sharedFileUtils()->isFileExist( ret )) {
            auto bg = CCSprite::create(fallbackPath);
            if (bg) {
                log::info("OGDBAutoImage: Loaded fallback image '{}'", fallbackPath);
                bg->setAnchorPoint({0.f, 0.f});
                bg->setPosition({0.f, 0.f});
                bg->setScaleX(ancho / bg->getContentSize().width);
                bg->setScaleY(largo / bg->getContentSize().height);

                bg->setID("ogdb-autoimage-image"_spr);

                log::info("OGDBautoImage: Added image to child...");
                bgLayer->addChild(bg);
                return bgLayer;
            } else {
                log::error("OGDBAutoImage: Failed to load fallback image '{}'", fallbackPath);
                onErrorImage("Error loading image...");
            }
        } else {
            onErrorImage("Error image no exist...");
        }
        
        return bgLayer;
    }
}

CCNode* OGDBAutoImage::setAutoImage(
    float ancho,
    float largo,
    std::string imgbgSprite_char,
    std::string bgIDSprite,
    int animateID,
    ccColor4B color1,
    ccColor4B color2
) {
    auto parent = CCNode::create();
    parent->setContentSize({ancho, largo});
    parent->setAnchorPoint({0.5f, 0.5f});
    parent->setPosition({ancho / 2, largo / 2});

    auto loadingSprite = LoadingSpinner::create(ancho / 3.5);

    loadingSprite->setPosition({ancho / 2, largo / 2});

    parent->addChild(loadingSprite, -2);

    auto loadImage = [=](std::string realBgID, std::string realImgPath, float ancho, float largo, int animateID, CCNode* parent) {
        CCLayer* background = nullptr;

        log::info("Loading file: {} / {} / {} / {} / {}", realBgID, realImgPath, ancho, largo, animateID);

        if (realBgID.starts_with("special_")) {
            if (realBgID.ends_with("none")) return;
            else if (realBgID.ends_with("simple")) {
                background = CCLayerColor::create(color1);
                background->setContentSize({ancho, largo});
            } else if (realBgID.ends_with("gradient")) {
                background = CCLayerGradient::create(color1, color2);
                background->setContentSize({ancho, largo});
            }

        } else if (realBgID.starts_with("c9_") || realBgID.starts_with("border_c9_") || realBgID.starts_with("sub_c9_")) {
            background = CCLayer::create();
            auto bg = CCScale9Sprite::create(realImgPath.c_str());
            bg->setColor({color1.r, color1.g, color1.b});
            bg->setAnchorPoint({0.f, 0.f});
            bg->setPosition({0.f, 0.f});
            bg->setOpacity(color1.a);
            bg->setContentSize({ancho, largo});
            background->addChild(bg);

        } else if (realImgPath.starts_with(""_spr) || realBgID.starts_with("bg_")) {
            background = onDefaultImage(ancho, largo, realImgPath.c_str());

        } else {
            auto* bg = LazySprite::create({ancho, largo}, true);
            if (!bg) {

                log::error("LazySprite dev nullptr {}", realImgPath);
                return;
            }


            // bg->setScale(1.f);
            bg->setAnchorPoint({0.f, 0.f});
            bg->setPosition({0.f, 0.f});

            bg->setLoadCallback([=](Result<> res) {
                if (!res) {
                    onErrorImage(fmt::format("Failed to load background image: {}", res.unwrapErr()));
                    auto fallback = onDefaultImage(ancho, largo);
                    // _
                    // loadingSprite->setVisible(false);
                    // _
                    if (auto parent = bg->getParent()) {
                        parent->removeAllChildren();
                        if (fallback && fallback->getChildrenCount() > 0) {
                            auto child = fallback->getChildren()->objectAtIndex(0);
                            if (auto node = dynamic_cast<CCNode*>(child)) {
                                parent->addChild(node);
                            }
                        }
                    }
                } else {
                    // _
                    // loadingSprite->setVisible(false);
                    // _
                    bg->setAnchorPoint({0.f,0.f});
                    bg->setPosition({0,0});

                    auto origSize = bg->getContentSize();
                    float scaleX = ancho  / origSize.width;
                    float scaleY = largo / origSize.height;
                    float scale  = std::max(scaleX, scaleY);
                    bg->setScale(scale);

                    if (animateID == 1) {
                        ccTexParams params = {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_CLAMP_TO_EDGE};
                        if (auto tex = bg->getTexture())
                            tex->setTexParameters(&params);
                        bg->setUserObject("speed", CCFloat::create(15.f));
                        bg->schedule(schedule_selector(OGDBAutoImage::infiniteSpriteToX));
                    }
                }
            });

            if (isURL(realImgPath)) {
                bg->loadFromUrl(realImgPath, LazySprite::Format::kFmtPng, false);
            } else {
                std::string ext = realImgPath.substr(realImgPath.find_last_of('.') + 1);
                LazySprite::Format fmt = LazySprite::Format::kFmtPng;
                if (ext == "jpg") fmt = LazySprite::Format::kFmtJpg;
                else if (ext == "webp") fmt = LazySprite::Format::kFmtWebp;
                bg->loadFromFile(realImgPath, fmt, false);
            }

            background = CCLayer::create();
            background->setContentSize({ancho, largo});
            background->addChild(bg);
        }

        if (!background) {
            background = CCLayerGradient::create({0, 0, 0, 0}, {0, 0, 0, 0});
            background->setContentSize({ancho, largo});
            // loadingSprite->setVisible(false);
        }

        parent->addChild(background);
        
    };

    if (bgIDSprite == "event_background_00") {
        static OGDBRequests request;

        matjson::Value payload = matjson::Value::object();
        payload["client"] = Mod::get()->getID();
        payload["version"] = Mod::get()->getVersion();
        
        request.post(
            "events/background.php",
            payload,

            [=, &loadImage ](OGDBRequests::Response resp ) {
                std::string newImage = "default.png";
                std::string newBgID = "bg_default";
                int newanimateID = 0;


                std::string body = resp.body;
                // auto headers = resp.headers;
                auto jsonData = matjson::parse(body);
                if(jsonData.isOk()) {
                    auto json = jsonData.unwrap();
                    
                    OGDBCache::set( request.m_baseURL + "events/background.php" + payload.dump()  , body );

                    log::info("Loading file: {} / {} / {}", ancho, largo, animateID);

                    if (json.contains("event_background"))
                        newImage = json["event_background"].asString().unwrapOr("special_gradient");
                    if (json.contains("event_background_id"))
                        newBgID = json["event_background_id"].asString().unwrapOr("transparent.png"_spr);
                    if (json.contains("event_background_animate_id"))
                        newanimateID = json["event_background_animate_id"].asInt().unwrapOr(0);

                    loadingSprite->setVisible(false);
                }
                loadImage(newBgID, newImage, ancho, largo, newanimateID, parent);
            },

            [=, &loadImage ](std::string err) {
                // log::error("Web load failed: {}", err);
                loadingSprite->setVisible(false);
                loadImage(bgIDSprite, imgbgSprite_char, ancho, largo, animateID, parent);
            }
        );

        return parent;
    } else {
        loadingSprite->setVisible(false);
        loadImage(bgIDSprite, imgbgSprite_char, ancho, largo, animateID, parent);
    }

    
    return parent;
}
// void OGDBAutoImage::selectAnimation(CCSprite* spriteImage, int animateID) {
//     switch (animateID) {
//         case 1: {
            
//             break;
//         }
        
//         default:break;
//     }
// }

void OGDBAutoImage::infiniteSpriteToX(float dt) {
    auto sprite = typeinfo_cast<CCSprite*>(this);
    if (!sprite) return;

    auto speed = static_cast<CCFloat*>(sprite->getUserObject("speed"))->getValue();
    // auto sprite = typeinfo_cast<CCSprite*>(this);

    auto rect = sprite->getTextureRect();
    rect.origin.x -= speed * dt;

    if (rect.origin.x <= -rect.size.width) {
        rect.origin.x = 0; 
        sprite->setUserObject("speed", CCFloat::create(speed)); 
    } else if (rect.origin.x / 2 >= rect.size.width) {
        rect.origin.x = 0; 
        sprite->setUserObject("speed", CCFloat::create(speed)); 
    }

    sprite->setTextureRect(rect);
}