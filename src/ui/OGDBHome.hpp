#pragma once

#include <Geode/Geode.hpp>
#include <any>

#include "../utils/utils.hpp"
#include "../utils/bg.hpp"
#include "settings.hpp"
#include "OGDBLoading.hpp"

using namespace geode::prelude;

class OGDBHome : public CCLayer {
protected:
    CCMenu* m_navbarMenu;
    CCLabelBMFont* m_statusLabel;
    ScrollLayer* m_scrollLayer;

    bool init() override;

    void createNavbar();
    void createScrollLayer();
    void updateScrollContent(const std::string& category);
    void fetchData(const std::string& category);
    void setIsLoaded(bool loaded, bool success);
    void createExtraButtons();

public:
    static OGDBHome* create();

    void onLogin(CCObject*);
    void onAccount(CCObject*);
    void onNavOption(CCObject*);

    void switchToOGDBHomeScene(CCObject*);

    void keyBackClicked() override;
    void backButtonCallback(CCObject* sender);
};
