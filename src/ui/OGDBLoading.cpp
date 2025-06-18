#include "OGDBLoading.hpp"

OGDBLoading* OGDBLoading::create(bool showText, bool showAnimation) {
    auto ret = new OGDBLoading();
    if (ret && ret->init()) {
        ret->autorelease();
        ret->m_showText = showText;
        ret->m_showAnimation = showAnimation;
        ret->setID("loading-ogdb");

        ret->setUserObject("isHidden", CCBool::create(true));
        ret->setVisible(false);

        ret->createDots();

        if (ret->m_showText) {
            ret->m_textLabel = CCLabelBMFont::create("Loading", "bigFont.fnt");
            ret->m_textLabel->setScale(0.5f);
            ret->m_textLabel->setPositionY(-45.f);
            ret->m_textLabel->setVisible(false);
            ret->addChild(ret->m_textLabel);
        }

        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void OGDBLoading::start() {
    if (m_showAnimation)
        animateIn();
    else {
        setVisibleOptimized(true);
        if (m_textLabel)
            m_textLabel->setVisible(true);
        startLoopAnimation();
        animateText();
    }
}

void OGDBLoading::createDots() {
    const int numDots = 8;
    for (int i = 0; i < numDots; ++i) {
        auto dot = CCSprite::create("OT_uiDot_001.png"_spr);
        dot->setScale(0.8f);
        dot->setOpacity(0);
        dot->setPosition({ 0.f, 0.f });
        this->addChild(dot);
        m_dots.push_back(dot);
    }
}

void OGDBLoading::animateIn() {
    setVisibleOptimized(true);
    stopLoopAnimation();

    const float radius = 30.f;
    const float baseAngle = 90.f;
    const size_t count = m_dots.size();

    for (size_t i = 0; i < count; ++i) {
        auto dot = m_dots[i];
        dot->stopAllActions();
        dot->setOpacity(0);
        dot->setPosition({ 0.f, 0.f });
        dot->setScale(0.4f);

        float angle = baseAngle + (360.f / count) * i;
        float rad = CC_DEGREES_TO_RADIANS(angle);
        float x = radius * cosf(rad);
        float y = radius * sinf(rad);

        dot->runAction(CCSpawn::create(
            CCMoveTo::create(0.35f, { x, y }),
            CCFadeTo::create(0.35f, 255),
            CCScaleTo::create(0.35f, 0.8f),
            nullptr
        ));
    }

    this->runAction(CCSequence::create(
        CCDelayTime::create(0.35f),
        CCCallFunc::create(this, callfunc_selector(OGDBLoading::startLoopAnimation)),
        nullptr
    ));

    if (m_textLabel) {
        m_textLabel->setOpacity(0);
        m_textLabel->setScale(0.4f);
        m_textLabel->setVisible(true);
        m_textLabel->runAction(CCSpawn::create(
            CCFadeTo::create(0.35f, 255),
            CCScaleTo::create(0.35f, 0.5f),
            nullptr
        ));
        animateText();
    }
}

void OGDBLoading::transitionToCircle() {
    const float radius = 30.f;
    const float baseAngle = 90.f;
    const size_t count = m_dots.size();

    for (size_t i = 0; i < count; ++i) {
        float angle = baseAngle + (360.f / count) * i;
        float rad = CC_DEGREES_TO_RADIANS(angle);
        float x = radius * cosf(rad);
        float y = radius * sinf(rad);

        m_dots[i]->setPosition({ x, y });
        m_dots[i]->setOpacity(255);
        m_dots[i]->setScale(0.8f);
    }

    this->runAction(CCSequence::create(
        CCDelayTime::create(0.3f),
        CCCallFunc::create(this, callfunc_selector(OGDBLoading::startLoopAnimation)),
        nullptr
    ));

    if (m_textLabel) {
        m_textLabel->setVisible(true);
        animateText();
    }
}

void OGDBLoading::startLoopAnimation() {
    schedule(schedule_selector(OGDBLoading::updateCirclePosition));
}

void OGDBLoading::updateCirclePosition(float dt) {
    static float angleOffset = 0.f;
    angleOffset += (360.f / m_loopTime) * dt;
    const float radius = 30.f;
    const float baseAngle = 90.f;
    const size_t count = m_dots.size();

    for (size_t i = 0; i < count; ++i) {
        float angle = angleOffset + baseAngle + (360.f / count) * i;
        float rad = CC_DEGREES_TO_RADIANS(angle);
        float x = radius * cosf(rad);
        float y = radius * sinf(rad);

        auto dot = m_dots[i];
        dot->setPosition({ x, y });

        float scale = 0.7f + 0.3f * sinf(rad);
        dot->setScale(scale);
        dot->setOpacity(static_cast<GLubyte>(128 + 127 * sinf(rad)));
    }
}

void OGDBLoading::animateText() {
    if (!m_textLabel) return;
    m_textLabel->stopAllActions();
    m_textLabel->runAction(CCRepeatForever::create(
        CCSequence::create(
            CCSpawn::create(
                CCScaleTo::create(0.6f, 0.45f),
                CCFadeTo::create(0.6f, 150),
                nullptr
            ),
            CCSpawn::create(
                CCScaleTo::create(0.6f, 0.5f),
                CCFadeTo::create(0.6f, 255),
                nullptr
            ),
            nullptr
        )
    ));
}

void OGDBLoading::animateOut() {
    stopLoopAnimation();

    for (auto dot : m_dots) {
        dot->runAction(CCSpawn::create(
            CCMoveTo::create(0.35f, ccp(0.f, 0.f)),
            CCFadeTo::create(0.35f, 0),
            nullptr
        ));
    }

    if (m_textLabel)
        m_textLabel->runAction(CCSpawn::create(
            CCFadeTo::create(0.35f, 0),
            CCScaleTo::create(0.35f, 0.4f),
            nullptr
        ));

    this->runAction(CCSequence::create(
        CCDelayTime::create(0.35f),
        CCCallFunc::create(this, callfunc_selector(OGDBLoading::hideSelf)),
        nullptr
    ));
}

void OGDBLoading::hideSelf() {
    setVisibleOptimized(false);
}

void OGDBLoading::stopLoopAnimation() {
    unschedule(schedule_selector(OGDBLoading::updateCirclePosition));
    for (auto dot : m_dots)
        dot->stopAllActions();
}

void OGDBLoading::updateText(std::string const& newText) {
    if (m_textLabel)
        m_textLabel->setString(newText.c_str());
}

void OGDBLoading::finished(bool showAnimation) {
    if (static_cast<CCBool*>(getUserObject("isHidden"))->getValue())
        return;

    if (showAnimation)
        animateOut();
    else
        setVisibleOptimized(false);
}

void OGDBLoading::setVisibleOptimized(bool visible) {
    if (visible) {
        setVisible(true);
        setUserObject("isHidden", CCBool::create(false));
    } else {
        setVisible(false);
        setUserObject("isHidden", CCBool::create(true));
    }
}
