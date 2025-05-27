#pragma once

#include "OGDBSettingCell.hpp"

// Code based on the globed mod cell but more personalized and with my typos

OGDBSettingCell* OGDBSettingCell::create(
    void* settingStorage, SettingEnum::Type type,
    const char* tagStore, const std::any& tagStoreDefault, const std::any& tagStoreValues,
    const char* name, const char* desc, const char* reqTxt,
    float width, float height
) {
    auto ret = new OGDBSettingCell();
    if (ret && ret->init(settingStorage, type, tagStore, tagStoreDefault, tagStoreValues, name, desc, reqTxt, width, height)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool OGDBSettingCell::init(
    void* settingStorage, SettingEnum::Type type,
    const char* tagStore, const std::any& tagStoreDefault, const std::any& tagStoreValues,
    const char* name, const char* desc, const char* reqTxt,
    float width, float height
) {
    if (!CCLayer::init())
        return false;

    m_settingStorage = settingStorage;
    m_settingType = type;
    m_titleText = name;
    m_descText = desc;
    m_requiredText = reqTxt;
    m_width = width;
    m_height = height;

    m_tag = tagStore;
    float globalOffSet = 10.f;
    float m_offsetYTitle = 5;

    // requiredText
    if (m_requiredText && m_requiredText[0] != '\0') {
        auto iconReqTxt = CCSprite::createWithSpriteFrameName("exMark_001.png");
        iconReqTxt->setScale(0.7f);
        m_labelRequiredText = CCLabelBMFont::create(
            m_requiredText, 
            "bigFont.fnt"
        );
        m_labelRequiredText->setColor({255,48,48}); // Red
        
        auto menuReqTxt = CCNode::create();
        menuReqTxt->setLayout(SimpleAxisLayout::create(Axis::Row)
            ->setMainAxisAlignment(MainAxisAlignment::Start)
            ->setCrossAxisScaling(AxisScaling::None)
            ->setMainAxisScaling(AxisScaling::None)
            ->setMainAxisDirection(AxisDirection::LeftToRight)
            ->setMinRelativeScale(false)
            ->setMaxRelativeScale(false)
            ->setGap(15.f)
        );
        // Start req
        menuReqTxt->setPosition({10.f, m_offsetYTitle});
        // set for name
        menuReqTxt->addChild(iconReqTxt);
        menuReqTxt->addChild(m_labelRequiredText);
        menuReqTxt->setScale(0.3f);
        menuReqTxt->updateLayout();
        this->addChild(menuReqTxt);
    } else {
        m_offsetYTitle = 0;
    }
    
    // name
    const char* m_fontlabelName = "bigFont.fnt";
    float m_offsetXTitle = 10.f;
    CCPoint setAnchorTitle = CCPoint(0.f, 0.5f);
    if(m_settingType == SettingEnum::Type::Title) {
        m_offsetXTitle = (m_width / 2);
        m_fontlabelName = "goldFont.fnt";
        setAnchorTitle = CCPoint(0.5f, 0.5f);
    }

    m_labelName = CCLabelBMFont::create(m_titleText, m_fontlabelName);
    m_labelName->setScale(0.5f);
    m_labelName->setAnchorPoint(setAnchorTitle);
    m_offsetYTitle = ((m_height / 2) + (m_offsetYTitle * 0.8));
    m_labelName->setPosition({m_offsetXTitle, m_offsetYTitle});
    this->addChild(m_labelName);

    // desc
    if (m_descText && m_descText[0] != '\0') {
        auto icon = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
        icon->setScale(0.35f);

        auto menuItem = CCMenuItemSpriteExtra::create(icon, this, menu_selector(OGDBSettingCell::onDescriptionAlert));
        float offsetX = m_labelName->getScaledContentSize().width + 5.f;
        menuItem->setPosition({10.f + offsetX, m_height / 2 + m_labelName->getScaledContentSize().height / 2 - 5.f});

        auto menu = CCMenu::create();
        menu->setPosition(CCPointZero);
        menu->addChild(menuItem);
        this->addChild(menu);
    }

    

    switch (m_settingType) {

         // ---- Bool ----

        case SettingEnum::Type::Bool : {
            auto spriteOff = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
            auto spriteOn = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");

            m_checkbox = CCMenuItemToggler::create(
                spriteOff, spriteOn,
                this, menu_selector(OGDBSettingCell::onCheckboxToggled)
            );
            if(m_settingStorage) {
                bool tagStoreDefaultRef = OGDBUtils::anyToRef<bool>(tagStoreDefault);
                bool m_togglecheckbox = Mod::get()->getSavedValue<bool>(m_tag, tagStoreDefaultRef);
                m_checkbox->toggle(m_togglecheckbox); 
                // log::info("Set {} to toggle mode -> {}", m_tag, m_togglecheckbox);
            }

            m_checkbox->setScale(0.5f);
            m_checkbox->setPosition({m_width - 47.5f, m_height / 2});
            m_checkbox->setID(m_tag);

            // log::info("TEST CREATING -> X: {}, Y: {}", m_width, m_height);

            auto menu = CCMenu::create();
            menu->setContentSize({m_width, m_height});
            menu->setAnchorPoint({0.f, 0.f});
            menu->setPosition(CCPointZero);
            menu->setLayout(AxisLayout::create(Axis::Row)
                ->setAxisReverse(true)
                ->setAxisAlignment(AxisAlignment::End)
                ->setCrossAxisAlignment(AxisAlignment::End)
                ->setGap(5.f)
                ->setAutoScale(false)
                ->setGrowCrossAxis(false)
                ->setCrossAxisOverflow(true)
            );
            
            menu->addChild(m_checkbox);
            this->addChild(menu);

            break;
        } 

        // ---- Options ----

        case SettingEnum::Type::Options : {
            m_optionsList = std::any_cast<std::vector<std::string>>(tagStoreValues);
            m_selectedIndex = 0;

            if(m_settingStorage) {
                int tagStoreDefaultRef = OGDBUtils::anyToRef<int>(tagStoreDefault);
                m_selectedIndex = Mod::get()->getSavedValue<int>(m_tag, tagStoreDefaultRef);
            }

            if (m_selectedIndex < 0 || m_selectedIndex >= static_cast<int>(m_optionsList.size()))
                m_selectedIndex = 0;

            auto leftBtn = CCMenuItemSpriteExtra::create(
                CCSprite::createWithSpriteFrameName("navArrowBtn_001.png"),
                this,
                menu_selector(OGDBSettingCell::onOptionChanged)
            );
            leftBtn->setTag(0);
            leftBtn->setScale(0.3f);
            leftBtn->m_baseScale = 0.3f;

            leftBtn->setRotation(180);
            leftBtn->setPosition({0, m_height / 2});

            auto rightBtn = CCMenuItemSpriteExtra::create(
                CCSprite::createWithSpriteFrameName("navArrowBtn_001.png"),
                this,
                menu_selector(OGDBSettingCell::onOptionChanged)
            );
            rightBtn->setTag(1);
            rightBtn->setScale(0.3f);
            rightBtn->m_baseScale = 0.3f;
            
            rightBtn->setPosition({0, m_height / 2});

            m_optionLabel = CCLabelBMFont::create("Unknown", "bigFont.fnt");
            
            // m_optionLabel->setScale(0.4f);
            // m_optionLabel->setPosition({m_width / 2, m_height / 2});

            updateOptionLabel();

            auto menu = OGDBSettingCell::createDefaultSpriteCCMenu(globalOffSet);
            menu->addChild(leftBtn);
            menu->addChild(rightBtn);
            menu->updateLayout();
            OGDBUtils::centerNodeBetweenXY(m_optionLabel, leftBtn, rightBtn);

            this->addChild(menu);
            this->addChild(m_optionLabel);

            break;
        }

        case SettingEnum::Type::Slider : {
            m_optionsListAny = OGDBUtils::anyToRef<std::vector<std::any>>(tagStoreValues);
            
            if (tagStoreDefault.type() == typeid(float)) {
                m_sliderIsInt = false;
            }
            float tagStoreDefaultRef = OGDBUtils::anyToRef<float>(tagStoreDefault, true);
            if(m_settingStorage) {
                m_sliderValue = Mod::get()->getSavedValue<float>(m_tag, tagStoreDefaultRef);
            }

            m_optionLabel = CCLabelBMFont::create("?", "bigFont.fnt");

            m_optionLabel->setPosition({m_width - (m_width / 9.f) , m_height / 2});
            

            m_slider = Slider::create(this, menu_selector(OGDBSettingCell::onSliderChange), 0.45f);
            m_slider->setAnchorPoint({0.f,0.f});
		    m_slider->setPosition({( m_width - (m_width / 4.65f) + ((m_slider->m_groove->getContentSize().width * 0.4f) / 2) ), m_height / 3.5f });
            
            OGDBUtils::setSliderWidth(m_slider, 120.f);

            m_slider->setValue(OGDBSettingCell::normalizeToPercent(m_sliderValue, OGDBUtils::anyToRef<float>(m_optionsListAny[0], true), OGDBUtils::anyToRef<float>(m_optionsListAny[1], true)));
            
            OGDBSettingCell::onSliderTextChange(m_sliderValue, OGDBUtils::anyToRef<std::string>(m_optionsListAny[3]) );

            // --- bars
            m_decreaseBtn = CCMenuItemSpriteExtra::create(
                CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"),
                this,
                menu_selector(OGDBSettingCell::onSliderOptionChanged )
            );
            m_decreaseBtn->setTag(0);
            m_decreaseBtn->setScale(0.3f);
            m_decreaseBtn->m_baseScale = 0.3f;
            

            m_increaseBtn = CCMenuItemSpriteExtra::create(
                CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"),
                this,
                menu_selector(OGDBSettingCell::onSliderOptionChanged )
            );
            m_increaseBtn->setTag(1);
            m_increaseBtn->setScale(0.3f);
            m_increaseBtn->m_baseScale = 0.3f;
            m_increaseBtn->setRotation(180);
            
            // --

            m_rowValues = OGDBUtils::anyToRef<float>(m_optionsListAny[2], true);

            float offsetRows = 0.f;
            
            auto menu = OGDBSettingCell::createDefaultSpriteCCMenu(globalOffSet);
            menu->addChild(m_decreaseBtn);
            menu->addChild(m_increaseBtn);
            menu->updateLayout();

            if(m_rowValues <= 0.5f) {
                menu->setVisible(false);
            }
            offsetRows = m_slider->m_groove->getContentSize().height / 2.5;
            

            menu->setPosition({menu->getPositionX(), menu->getPositionY() + offsetRows});

            // leftBtn->setPosition({0, m_height });
            // rightBtn->setPosition({0,  m_height });

            OGDBUtils::centerNodeBetweenXY(m_optionLabel, m_decreaseBtn, m_increaseBtn);
            m_optionLabel->setPosition({m_optionLabel->getPositionX(), m_optionLabel->getPositionY() + offsetRows }); // bug?

            this->addChild(menu);

            auto menuScrollBar = CCMenu::create();
            menuScrollBar->setContentSize({m_width, m_height});
            menuScrollBar->setAnchorPoint({0.f, 0.f});
            menuScrollBar->setPosition(CCPointZero);
            menuScrollBar->addChild(m_slider);

            

            this->addChild(menuScrollBar);
            this->addChild(m_optionLabel);
            break;
        }

        case SettingEnum::Type::Color4B: {

            break;
        }

        case SettingEnum::Type::Text: {

            break;
        }

        case SettingEnum::Type::Setting_Profile_Custom : {
            auto buttonSprite = ButtonSprite::create(
                "Open",
                "bigFont.fnt",  
                "GJ_button_05.png",    
                1.0f                       
            );

            auto settingBtn = CCMenuItemSpriteExtra::create(
                buttonSprite,
                this,
                menu_selector(OGDBSettingCell::onCustom_PopupProfileCustom)
            );
            settingBtn->setScale(0.4f);
            settingBtn->m_baseScale = 0.4f;

            settingBtn->setPosition({0, m_height / 2});

            auto menu = OGDBSettingCell::createDefaultSpriteCCMenu(globalOffSet + 20);
            menu->addChild(settingBtn);
            menu->updateLayout();

            this->addChild(menu);

            break;
        }
    }

    // this->setContentSize({m_width, m_height});
    return true;
}

// --- Slider --- // 

void OGDBSettingCell::onSliderTextChange(float value, std::string surfix) {
    float min = OGDBUtils::anyToRef<float>(m_optionsListAny[0], true);
    float max = OGDBUtils::anyToRef<float>(m_optionsListAny[1], true);

    std::string displayValue;
    if (m_sliderIsInt) {
        displayValue = std::to_string(static_cast<int>(value));
    } else {
        char buffer[16];
        snprintf(buffer, sizeof(buffer), "%.2f", value);
        displayValue = buffer;
    }
    displayValue += surfix;
    
    m_optionLabel->setString(displayValue.c_str());
    OGDBUtils::autoScaleToFit(m_optionLabel, 45.f, 47.f);
}

void OGDBSettingCell::onSliderChange(CCObject* sender) {
    float selectedValue = m_slider->getThumb()->getValue();
    float selectedConvertedValue = clampf(selectedValue, 0.0f, 1.0f) * (OGDBUtils::anyToRef<float>(m_optionsListAny[1], true) - OGDBUtils::anyToRef<float>(m_optionsListAny[0], true)) ;
    if(m_settingStorage) {
        if(m_sliderIsInt) Mod::get()->setSavedValue<int>(m_tag, selectedConvertedValue);
        else Mod::get()->setSavedValue<float>(m_tag, selectedConvertedValue);
    }
    OGDBSettingCell::onSliderTextChange(selectedConvertedValue, OGDBUtils::anyToRef<std::string>(m_optionsListAny[3]) );
    m_sliderValue = selectedConvertedValue;

     this->updateSliderButtons();
}

void OGDBSettingCell::onSliderOptionChanged(CCObject* sender) {
    auto btn = static_cast<CCMenuItemSpriteExtra*>(sender);
    int tag = btn->getTag();

    float min = OGDBUtils::anyToRef<float>(m_optionsListAny[0], true);
    float max = OGDBUtils::anyToRef<float>(m_optionsListAny[1], true);
    float labelStep = m_rowValues;

    float newValue = m_sliderValue + (tag <= 0 ? -labelStep : labelStep);
    newValue = std::clamp(newValue, min, max);
    m_sliderValue = newValue;

    float percent = (newValue - min) / (max - min);
    m_slider->getThumb()->setValue(percent);

    this->onSliderChange(nullptr);
}

void OGDBSettingCell::updateSliderButtons() {
    float min = OGDBUtils::anyToRef<float>(m_optionsListAny[0], true);
    float max = OGDBUtils::anyToRef<float>(m_optionsListAny[1], true);


    bool disabled = m_sliderValue <= min;
    m_decreaseBtn->setEnabled(!disabled);
    m_decreaseBtn->setOpacity(disabled ? 150 : 255);
    

    bool disabled2 = m_sliderValue >= max;
    m_increaseBtn->setEnabled(!disabled2);
    m_increaseBtn->setOpacity(disabled2 ? 150 : 255);
    
}
// ---- Global ---

float OGDBSettingCell::normalizeToPercent(float value, float min, float max) {
    if (min == max) return 0.0f; 
    return clampf((value - min) / (max - min), 0.0f, 1.0f);
}

void OGDBSettingCell::onDescriptionAlert(CCObject* sender) {
    if (m_descText == nullptr || m_descText == "") return;
    FLAlertLayer::create(
        nullptr,
        m_titleText,
        m_descText,
        "OK",
        nullptr,
        380,
        false,
        100,
        0.8f
    )->show(); 
}

// ---- Bool  ---

void OGDBSettingCell::onCheckboxToggled(CCObject* sender) {
    if (m_settingType == SettingEnum::Type::Bool) {
        auto toggler = static_cast<CCMenuItemToggler*>(sender);
        bool enabled = !toggler->isToggled();
        // std::string id = toggler->getID();

        if(m_settingStorage) {
            Mod::get()->setSavedValue(m_tag, enabled);
            // log::info("Toggle Saved Config '{}' set to {}", m_tag, enabled);
        } 
        else {
            // log::info("Toggle '{}' set to {}", m_tag, enabled);
        }
    }
}

// ---- Options ---

void OGDBSettingCell::updateOptionLabel() {
    if (!m_optionLabel) return;
    if (m_selectedIndex >= 0 && m_selectedIndex < static_cast<int>(m_optionsList.size())) {
        m_optionLabel->setString(m_optionsList[m_selectedIndex].c_str());
    } else {
        m_optionLabel->setString("Unknown");
    }
    OGDBUtils::autoScaleToFit(m_optionLabel, 45.f, 47.f);
}

void OGDBSettingCell::onOptionChanged(CCObject* sender) {
    int tag = static_cast<CCMenuItemSpriteExtra*>(sender)->getTag();

    if (tag <= 0) {
        m_selectedIndex = (m_selectedIndex - 1 + m_optionsList.size()) % m_optionsList.size();
    } else {
        m_selectedIndex = (m_selectedIndex + 1) % m_optionsList.size();
    }

    updateOptionLabel();

    if (m_settingStorage) {
        Mod::get()->setSavedValue(m_tag, m_selectedIndex);
    }
}

// Unused in 1.0 version due bad optimization hehe
// void OGDBSettingCell::onOptionLeft(CCObject* sender) {
//     m_selectedIndex = (m_selectedIndex - 1 + m_optionsList.size()) % m_optionsList.size();
//     updateOptionLabel();
//     if (m_settingStorage) {
//         Mod::get()->setSavedValue(m_tag, m_selectedIndex);
//     }

// }

// void OGDBSettingCell::onOptionRight(CCObject* sender) {
//     m_selectedIndex = (m_selectedIndex + 1) % m_optionsList.size();
//     updateOptionLabel();
//     if (m_settingStorage) {
//         Mod::get()->setSavedValue(m_tag, m_selectedIndex);
//     }

// }

// More

CCMenu* OGDBSettingCell::createDefaultSpriteCCMenu(float globalOffSet) {
    auto menu = CCMenu::create();
    menu->setPosition(CCPointZero);

    menu->setContentSize({ m_width - globalOffSet, m_height });
    menu->setAnchorPoint({ 0.f, 0.f });
    menu->setPosition(CCPointZero);

    // i love u simpleaxislayout
    menu->setLayout(
        SimpleAxisLayout::create(Axis::Row)
            ->setMainAxisAlignment(MainAxisAlignment::End)
            ->setMainAxisDirection(AxisDirection::LeftToRight)
            ->setGap(65.f)
    );

    return menu;
}

// --- Custom --- // Profile Custom
void OGDBSettingCell::onCustom_PopupProfileCustom(CCObject* sender) {
    CustomizeProfilePopup::create()->show();
}