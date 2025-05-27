#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/LazySprite.hpp>
#include "OGDBCache.hpp"

using namespace geode::prelude;

class OGDBAutoImage : public cocos2d::CCNode {
public:
    static CCNode* setAutoImage(
        float ancho,
        float largo,
        const std::string imgbgSprite_char,
        const std::string bgIDSprite,
        int animateID,
        ccColor4B color1,
        ccColor4B color2
    );

    void infiniteSpriteToX(float dt);

};
