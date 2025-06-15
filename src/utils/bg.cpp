#include "bg.hpp"

/*
    This code is horrible but it works, I will update it very soon.
    :3
*/

OGDBBackground* OGDBBackground::create() {

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto bgNode = new OGDBBackground();


    auto gradient = CCLayerGradient::create(ccc4(110, 0, 253, 255), ccc4(0, 9, 115, 255));
    gradient->setID("bg-ogdb-gradient");
    gradient->setContentSize(winSize);
    gradient->setAnchorPoint({0.0f, 0.0f});
    gradient->setPosition({0.0f, 0.0f});
    gradient->setVector(ccp(0, -1)); 
    bgNode->addChild(gradient, -999); 


    auto sprite = CCSprite::create("wavemask.png"_spr);
    auto sprite2 = CCSprite::create("wavemask.png"_spr);
    auto sprite3 = CCSprite::create("wavemask.png"_spr);

    ccTexParams params = {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_CLAMP_TO_EDGE};
    sprite3->getTexture()->setTexParameters(&params);

    auto rect = sprite3->getTextureRect();
    float globalScale = 0.55f;
    rect.size.height = 320;
    rect.origin.y = 13.5;
    rect.size.width = winSize.width / globalScale; // Fix for mobiles
    
    sprite3->setTextureRect(rect);

    sprite3->setAnchorPoint({ 0.0f, 1.0f }); 
    sprite3->setPosition({ 0, (rect.size.height * 0.55f)});  
    sprite3->setScale(globalScale);
    sprite3->setColor(ccc3(132, 2, 255));  

    sprite3->setContentSize({ rect.size.width, rect.size.height });
    sprite3->setTag(1); 
    sprite3->setUserObject("speed", CCFloat::create(-48.f));

    sprite3->schedule(schedule_selector(OGDBBackground::updateSprite));

    bgNode->addChild(sprite3,-902);  
    
    // --------------------- //

    sprite2->getTexture()->setTexParameters(&params);

    sprite2->setTextureRect(rect);

    sprite2->setAnchorPoint({ 0.0f, 1.0f }); 
    sprite2->setPosition({ 0, ((rect.size.height * 0.45f)) }); 
    sprite2->setScale(globalScale);
    sprite2->setColor(ccc3(58, 0, 209));          

    sprite2->setContentSize({ rect.size.width, rect.size.height });
    sprite2->setTag(2); 
    sprite2->setUserObject("speed", CCFloat::create(-24.f));

    sprite2->schedule(schedule_selector(OGDBBackground::updateSprite));

    bgNode->addChild(sprite2,-901);  

    // --------------------- //

    sprite->getTexture()->setTexParameters(&params);

    sprite->setTextureRect(rect);

    sprite->setAnchorPoint({ 0.0f, 1.0f }); 
    sprite->setPosition({ 0, (rect.size.height * 0.3f) });  
    sprite->setScale(globalScale);
    sprite->setColor(ccc3(50, 9, 123)) ;

    sprite->setContentSize({ rect.size.width, rect.size.height });
    sprite->setTag(3); 
    sprite->setUserObject("speed", CCFloat::create(-12.f));

    sprite->schedule(schedule_selector(OGDBBackground::updateSprite));

    bgNode->addChild(sprite, -900);  

    return bgNode;
}


void OGDBBackground::updateSprite(float dt) {
    auto speed = static_cast<CCFloat*>(this->getUserObject("speed"))->getValue();
    auto sprite = typeinfo_cast<CCSprite*>(this);

    auto rect = sprite->getTextureRect();
    rect.origin.x -= speed * dt;

    if (rect.origin.x <= -rect.size.width) {
        rect.origin.x = -rect.size.width;
        sprite->setUserObject("speed", CCFloat::create(-speed));
    } else if (rect.origin.x / 2 >= rect.size.width) {
        rect.origin.x = rect.size.width;
        sprite->setUserObject("speed", CCFloat::create(-speed));
    }

    sprite->setTextureRect(rect);
}