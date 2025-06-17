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
            ret->m_textLabel = CCLabelBMFont::create("Loading...", "bigFont.fnt");
            ret->m_textLabel->setPositionY(-40.f);
            ret->m_textLabel->setVisible(false);
            ret->m_textLabel->setOpacity(0);
            ret->addChild(ret->m_textLabel);
        }

        if (ret->m_showAnimation)
            ret->animateIn();
        else {
            ret->setVisibleOptimized(true);
            for (auto dot : ret->m_dots)
                dot->setOpacity(255);
            if (ret->m_textLabel) {
                ret->m_textLabel->setVisible(true);
                ret->m_textLabel->setOpacity(255);
            }
            ret->startLoopAnimation();
        }

        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void OGDBLoading::createDots() {
    float spacing = 30.f;
    float totalWidth = 2 * spacing;
    float startX = -totalWidth / 2;

    for (int i = 0; i < 3; ++i) {
        auto dot = CCSprite::createWithSpriteFrameName("uiDot_001.png");
        dot->setPosition({ startX + i * spacing, 0.f });
        dot->setScale(0.8f);
        dot->setOpacity(0);
        this->addChild(dot);
        m_dots.push_back(dot);
    }
}

void OGDBLoading::animateIn() {
    setVisibleOptimized(true);

    for (auto dot : m_dots) {
        dot->stopAllActions();
        dot->setOpacity(0);
    }

    for (size_t i = 0; i < m_dots.size(); ++i) {
        auto dot = m_dots[i];
        dot->runAction(CCSequence::create(
            CCDelayTime::create(0.05f * i),
            CCFadeTo::create(0.2f, 255),
            nullptr
        ));
    }

    this->runAction(CCSequence::create(
        CCDelayTime::create(m_animationTime),
        CCCallFunc::create(this, callfunc_selector(OGDBLoading::onAnimationInFinished)),
        nullptr
    ));
}

void OGDBLoading::onAnimationInFinished() {
    if (m_textLabel) {
        m_textLabel->setVisible(true);
        m_textLabel->runAction(CCFadeTo::create(0.2f, 255));
    }
    startLoopAnimation();
}

void OGDBLoading::startLoopAnimation() {
    for (size_t i = 0; i < m_dots.size(); ++i) {
        auto dot = m_dots[i];
        dot->runAction(CCRepeatForever::create(
            CCSequence::create(
                CCFadeTo::create(0.25f, 128),
                CCFadeTo::create(0.25f, 255),
                CCDelayTime::create(0.1f * i),
                nullptr
            )
        ));
    }
}

void OGDBLoading::stopLoopAnimation() {
    for (auto dot : m_dots)
        dot->stopAllActions();
}

void OGDBLoading::animateOut() {
    stopLoopAnimation();

    for (size_t i = 0; i < m_dots.size(); ++i) {
        auto dot = m_dots[i];
        dot->runAction(CCSequence::create(
            CCDelayTime::create(0.05f * i),
            CCFadeTo::create(0.2f, 0),
            nullptr
        ));
    }

    if (m_textLabel)
        m_textLabel->runAction(CCFadeTo::create(m_animationTime, 0));

    this->runAction(CCSequence::create(
        CCDelayTime::create(m_animationTime),
        CCCallFunc::create(this, callfunc_selector(OGDBLoading::hideCompletely)),
        nullptr
    ));
}

void OGDBLoading::hideCompletely() {
    setVisibleOptimized(false);
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
        this->setVisible(true);
        this->setUserObject("isHidden", CCBool::create(false));
    } else {
        this->setVisible(false);
        this->setUserObject("isHidden", CCBool::create(true));
    }
}
