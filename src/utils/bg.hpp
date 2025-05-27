#pragma once
#include <Geode/Geode.hpp>
using namespace geode::prelude;

class OGDBBackground : public cocos2d::CCNode {
public:
    static OGDBBackground* create();

    struct MaskedGradient {
        CCSprite* finalSprite;
        CCSprite* maskSprite;
    };

    // CCSprite* waveSprite = nullptr;
    // CCSprite* waveMask = nullptr;

    // static void createMaskedGradient(
    //     const std::string& maskImage,
    //     const ccColor4B& startColor,
    //     const ccColor4B& endColor,
    //     const CCPoint& vector,
    //     const CCSize& size,
    //     CCSprite*& outFinalSprite,
    //     CCSprite*& outMaskSprite
    // );


    void updateSprite(float dt); 
};