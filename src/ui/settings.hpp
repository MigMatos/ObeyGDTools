#pragma once

#include <Geode/Geode.hpp>
#include <Geode/utils/string.hpp>

#include <Geode/ui/BasedButtonSprite.hpp>
#include <Geode/ui/BasedButton.hpp>
#include <Geode/ui/ScrollLayer.hpp>
#include <Geode/ui/GeodeUI.hpp>

#include "../utils/utils.hpp"
#include "../utils/bg.hpp"

#include <Geode/ui/ListView.hpp>
#include <Geode/ui/Scrollbar.hpp>

#include <Geode/utils/base64.hpp>

// #include <Geode/binding/GJListLayer.hpp>


#include "../settings/OGDBSetting.hpp"
#include "../settings/OGDBSettingCell.hpp"

#include <any>

using namespace geode::prelude;

class OGDBSettingsLayer : public CCLayer{
    protected:
        std::unordered_map<int, Ref<CCNode>> storedTabs;
        std::unordered_map<int, Ref<cocos2d::CCArray>> settingCells;
        int currentTab = -1;
        geode::TabButton *tabBtn, *tabBtn1, *tabBtn2;
        cocos2d::CCClippingNode* tabsGradientNode = nullptr;
        std::array<CCLayerColor*, 3> tabBGs;

        cocos2d::CCLayerColor *mainCOptions;

        static constexpr float CELL_WIDTH = 358.0f;
        static constexpr float CELL_HEIGHT = 30.0f;
        CCNode* makeListLayer(int category);

        void registerOption();
        // ---

        bool init() override;
        void keyBackClicked() override;
        void backButtonCallback(CCObject*);

        // ---

        void onTab(cocos2d::CCObject* sender);
        void onTabById(int tag);
        void remakeList();


    public:
        static constexpr float TAB_SCALE = 0.9f;
        static constexpr int TAG_TAB_GENERAL = 0;
        static constexpr int TAG_TAB_PROFILES = 1;
        static constexpr int TAG_TAB_GDPS = 2;
        static constexpr float LIST_WIDTH = 358.f;
        static constexpr float LIST_HEIGHT = 220.f;

        static OGDBSettingsLayer* create();
        void switchToOGDBSettingsLayerButton(CCObject*);

        void onToggleOption(CCObject* sender);

};
