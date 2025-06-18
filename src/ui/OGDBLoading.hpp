#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class OGDBLoading : public CCNode {
protected:
    bool m_showText = false;
    bool m_showAnimation = true;
    float m_loopTime = 2.5f;
    std::vector<CCSprite*> m_dots;
    CCLabelBMFont* m_textLabel = nullptr;

public:
    static OGDBLoading* create(bool showText = true, bool showAnimation = true);
    void start();
    void updateText(std::string const& newText);
    void finished(bool showAnimation = true);

    // -------------- //
    void createDots();
    void animateIn();
    void transitionToCircle();
    void startLoopAnimation();
    void animateText();
    void animateOut();
    void hideSelf();
    void stopLoopAnimation();
    void setVisibleOptimized(bool visible);
    void updateCirclePosition(float dt);
};