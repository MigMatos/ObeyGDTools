#pragma once

#include <Geode/Geode.hpp>
// SimpleAxis is better LMAO
#include <Geode/ui/SimpleAxisLayout.hpp>

#include <any>
#include <fmt/core.h>
#include <cmath>
#include "../utils/utils.hpp"

// Rewrite in 1.0-alpha
#include "SettingsEnum.hpp" 
#include "../ui/popups/CustomizeProfilePopup.hpp"
// ---- //

using namespace geode::prelude;



class OGDBSettingCell : public CCLayer {
public:

    static OGDBSettingCell* create(
        void* settingStorage,
        SettingEnum::Type type,
        const char* tagStore,
        const std::any& tagStoreDefault,
        const std::any& tagStoreValues,
        const char* name,
        const char* desc,
        const char* reqTxt,
        float width = 280.f,
        float height = 40.f
    );

    bool init(
        void* settingStorage,
        SettingEnum::Type type,
        const char* tagStore,
        const std::any& tagStoreDefault,
        const std::any& tagStoreValues,
        const char* name,
        const char* desc,
        const char* reqTxt,
        float width,
        float height
    );
    

private:
    void* m_settingStorage = nullptr;
    SettingEnum::Type m_settingType;
    const char* m_descText = nullptr;
    const char* m_titleText = "Information";
    const char* m_requiredText = "";
    const char* m_tag = "";
    // Limits m_limits;

    float m_width = 240.f;
    float m_height = 40.f;

    cocos2d::CCLabelBMFont* m_labelRequiredText = nullptr;
    cocos2d::CCLabelBMFont* m_labelName = nullptr;
    void onDescriptionAlert(cocos2d::CCObject* sender);
    float normalizeToPercent(float value, float min, float max);

    // Bool
    CCMenuItemToggler* m_checkbox = nullptr;
    void onCheckboxToggled(cocos2d::CCObject* sender);

    // Slider

    Slider* m_slider = nullptr;
    std::vector<std::any> m_optionsListAny;
    bool m_sliderIsInt = true;
    float m_rowValues = 0.f;
    float m_sliderValue = 1.f;

    void onSliderTextChange(float value, std::string surfix = "");
    void onSliderChange(CCObject* sender);
    void onSliderOptionChanged(CCObject* sender);
    void updateSliderButtons();
    CCMenuItemSpriteExtra* m_decreaseBtn;
    CCMenuItemSpriteExtra* m_increaseBtn;

    // Options
    int m_selectedIndex = 0;
    std::vector<std::string> m_optionsList;
    CCLabelBMFont* m_optionLabel = nullptr;

    void updateOptionLabel();
    void onOptionChanged(CCObject* sender);
    // void onOptionLeft(CCObject* sender);
    // void onOptionRight(CCObject* sender);

    CCMenu* createDefaultSpriteCCMenu(float globalOffSet);
    
    //
    void onCustom_PopupProfileCustom(CCObject* sender);
};