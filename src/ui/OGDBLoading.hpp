#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class OGDBLoading : public CCNode {
protected:
    bool m_showText = true;
    bool m_showAnimation = true;
    CCLabelBMFont* m_textLabel = nullptr;
    std::vector<CCSprite*> m_dots;
    float m_animationTime = 0.35f;

    void createDots();
    void animateIn();
    void animateOut();
    void startLoopAnimation();
    void stopLoopAnimation();
    void onAnimationInFinished();
    void hideCompletely();

public:
    static OGDBLoading* create(bool showText = true, bool showAnimation = true);

    void updateText(std::string const& newText);
    void finished(bool showAnimation = true);
    void setVisibleOptimized(bool visible);
};
