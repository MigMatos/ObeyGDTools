#pragma once

#include <Geode/Geode.hpp>

#include "../utils/OGDBAutoImage.hpp"

#include "../presets/OGDBPresetProfile.hpp"
#include "../presets/assets/CustomBackgroundSprite.hpp"
#include "../presets/assets/CustomBorderSprite.hpp"
#include "../presets/assets/CustomSubbackground.hpp"
// new in 1.0-b
#include "../presets/assets/CustomIconsBackground.hpp"
#include "../presets/assets/CustomCommentsBackground.hpp"

#include "../utils/FLSelectorLayer.hpp"

// Extra
#include <Geode/ui/Popup.hpp>
#include <Geode/binding/ColorChannelSprite.hpp>
// 
#include <Geode/ui/ColorPickPopup.hpp>
#include <Geode/loader/Mod.hpp>
// Rewrite in 1.0-alpha
#include "../../settings/OGDBSetting.hpp"
#include "../../utils/delegates/customColorCallback.hpp"

using namespace geode::prelude;

class CustomizeProfilePopup : public Popup<> {
protected:
    int bgProfileID = 0; // Unused in 0.9 version
    bool hasUncommitChanges = false;
    bool isBlockedSaveButton = true;

    ColorChannelSprite* colorSprite1 = nullptr;
    ColorChannelSprite* colorSprite2 = nullptr;
    ColorChannelSprite* colorSprite3 = nullptr;
    ColorChannelSprite* colorSprite4 = nullptr;
    ColorChannelSprite* colorSprite5 = nullptr;
    ColorChannelSprite* colorSprite6 = nullptr;

    ccColor4B colorPicked1 = {255,255,255,255};
    ccColor4B colorPicked2 = {255,255,255,255};
    ccColor4B colorPicked3 = {255,255,255,255};
    ccColor4B colorPicked4 = {255,255,255,255};
    ccColor4B colorPicked5 = {255,255,255,255};
    ccColor4B colorPicked6 = {255,255,255,255};

    CCLabelBMFont* percent1 = nullptr;
    CCLabelBMFont* percent2 = nullptr;
    CCLabelBMFont* percent3 = nullptr;
    CCLabelBMFont* percent4 = nullptr;
    CCLabelBMFont* percent5 = nullptr;
    CCLabelBMFont* percent6 = nullptr;

    CCMenuItemToggler* toggle1 = nullptr;
    CCMenuItemToggler* toggle2 = nullptr;
    CCMenuItemToggler* toggle3 = nullptr;
    CCMenuItemToggler* toggle4 = nullptr;

    bool defaultToggle1 = true;
    bool defaultToggle2 = true;
    bool defaultToggle3 = true;
    bool defaultToggle4 = false;

    CCMenuItemSpriteExtra* saveButton = nullptr;

    // Items
    
    std::string m_bgSprite_char = "transparent.png"_spr;
    std::string m_bgSprite_charID = "special_gradient";
    int m_bgSprite_animateID = 0;
    
    std::string m_borderSprite_char = "border_b_00.png"_spr;
    std::string m_borderSprite_charID = "border_c9_rounded_00";
    int m_borderSprite_animateID = 0;

    std::string m_bgSubSprite_char = "sub_c9_dark_02";
    std::string m_bgSubSprite_charID = "dark_b_02.png"_spr;
    int m_bgSubSprite_animateID = 0;

    std::string m_bgIconSprite_char = "dark_b_01.png"_spr;
    std::string m_bgIconSprite_charID = "sub_c9_dark_01";
    int m_bgIconSprite_animateID = 0;

    std::string m_bgCommentSprite_char = "dark_b_01.png"_spr;
    std::string m_bgCommentSprite_charID = "sub_c9_dark_01";
    int m_bgCommentSprite_animateID = 0;

    int colorPersistentToggle1 = 0;
    int colorPersistentToggle2 = 0;
    int colorPersistentToggle3 = 0;
    int colorPersistentToggle4 = 0;
    int colorPersistentToggle5 = 0;
    int colorPersistentToggle6 = 0;

    CCMenu* buttonsColorsPer1 = nullptr;
    CCMenu* buttonsColorsPer2 = nullptr;
    CCMenu* buttonsColorsPer3 = nullptr;
    CCMenu* buttonsColorsPer4 = nullptr;
    CCMenu* buttonsColorsPer5 = nullptr;
    CCMenu* buttonsColorsPer6 = nullptr;

    CCNode* m_bgSprite_charNode = CCNode::create();

    ButtonSprite* spriteBtnBGPreset = ButtonSprite::create("Select Preset BGs", "bigFont.fnt", "GJ_button_01.png", 0.825f);
    ButtonSprite* spriteBtnShowProfile = ButtonSprite::create("Show Profile", "bigFont.fnt", "GJ_button_01.png", 0.825f);
    
    ButtonSprite* spriteBtnBGProfile = ButtonSprite::create("Edit", "bigFont.fnt", "GJ_button_01.png", 0.825f);
    ButtonSprite* spriteBtnBorder = ButtonSprite::create("Edit", "bigFont.fnt", "GJ_button_01.png", 0.825f);
    ButtonSprite* spriteBtnBGSublayer = ButtonSprite::create("Edit", "bigFont.fnt", "GJ_button_01.png", 0.825f);
    ButtonSprite* spriteBtnBGIcon = ButtonSprite::create("Edit", "bigFont.fnt", "GJ_button_01.png", 0.825f);
    ButtonSprite* spriteBtnBGComment = ButtonSprite::create("Edit", "bigFont.fnt", "GJ_button_01.png", 0.825f);

    ButtonSprite* saveButtonSprite = ButtonSprite::create("Save Settings", 150, 0, 0.8f, true);
    ButtonSprite* exitButtonSprite = ButtonSprite::create("Back", 50, 0, 0.8f, true);

    bool setup() override {
        this->setTitle("Customize Profile");

        colorPicked1 = OGDBSetting::get("PROFILE_BACKGROUND_COLOR")->getValue<ccColor4B>();
        colorPicked2 = OGDBSetting::get("PROFILE_BACKGROUND_COLOR_2")->getValue<ccColor4B>();
        colorPicked3 = OGDBSetting::get("PROFILE_BACKGROUND_SUBLAYER_COLOR")->getValue<ccColor4B>();
        colorPicked4 = OGDBSetting::get("PROFILE_BACKGROUND_BORDER_COLOR")->getValue<ccColor4B>();
        colorPicked5 = OGDBSetting::get("PROFILE_BACKGROUND_ICONS_COLOR")->getValue<ccColor4B>();
        colorPicked6 = OGDBSetting::get("PROFILE_BACKGROUND_COMMENTS_COLOR")->getValue<ccColor4B>();

        colorPersistentToggle1 = OGDBSetting::get("PROFILE_BACKGROUND_COLOR_PERSISTENT")->getValue<int>();
        colorPersistentToggle2 = OGDBSetting::get("PROFILE_BACKGROUND_COLOR_2_PERSISTENT")->getValue<int>();
        colorPersistentToggle3 = OGDBSetting::get("PROFILE_BACKGROUND_SUBLAYER_COLOR_PERSISTENT")->getValue<int>();
        colorPersistentToggle4 = OGDBSetting::get("PROFILE_BACKGROUND_COLOR_PERSISTENT")->getValue<int>();
        colorPersistentToggle5 = OGDBSetting::get("PROFILE_BACKGROUND_ICONS_COLOR_PERSISTENT")->getValue<int>();
        colorPersistentToggle6 = OGDBSetting::get("PROFILE_BACKGROUND_COMMENTS_COLOR_PERSISTENT")->getValue<int>();

        defaultToggle1 = OGDBSetting::get("PROFILE_ENABLE_BG_CUSTOM_IMAGE")->getValue<bool>();
        
        // ---------- //
        m_bgSprite_char = OGDBSetting::get("PROFILE_BACKGROUND_CUSTOM_SPRITE")->getValue<std::string>();
        m_bgSprite_charID = OGDBSetting::get("PROFILE_BACKGROUND_CUSTOM_SPRITE_ID")->getValue<std::string>();
        m_bgSprite_animateID = OGDBSetting::get("PROFILE_BACKGROUND_CUSTOM_ANIMATE_ID")->getValue<int>();
        //
        m_borderSprite_char = OGDBSetting::get("PROFILE_BORDER_CUSTOM_SPRITE")->getValue<std::string>();
        m_borderSprite_charID = OGDBSetting::get("PROFILE_BORDER_CUSTOM_SPRITE_ID")->getValue<std::string>();
        m_borderSprite_animateID = OGDBSetting::get("PROFILE_BORDER_CUSTOM_ANIMATE_ID")->getValue<int>();
        //
        m_bgSubSprite_char = OGDBSetting::get("PROFILE_BACKGROUND_SUBLAYER_CUSTOM_SPRITE")->getValue<std::string>();
        m_bgSubSprite_charID = OGDBSetting::get("PROFILE_BACKGROUND_SUBLAYER_CUSTOM_SPRITE_ID")->getValue<std::string>();
        m_bgSubSprite_animateID = OGDBSetting::get("PROFILE_BACKGROUND_SUBLAYER_CUSTOM_ANIMATE_ID")->getValue<int>();
        //
        m_bgIconSprite_char = OGDBSetting::get("PROFILE_BACKGROUND_ICON_CUSTOM_SPRITE")->getValue<std::string>();
        m_bgIconSprite_charID = OGDBSetting::get("PROFILE_BACKGROUND_ICON_CUSTOM_SPRITE_ID")->getValue<std::string>();
        m_bgIconSprite_animateID = OGDBSetting::get("PROFILE_BACKGROUND_ICON_CUSTOM_ANIMATE_ID")->getValue<int>();
        //
        m_bgCommentSprite_char = OGDBSetting::get("PROFILE_BACKGROUND_COMMENT_CUSTOM_SPRITE")->getValue<std::string>();
        m_bgCommentSprite_charID = OGDBSetting::get("PROFILE_BACKGROUND_COMMENT_CUSTOM_SPRITE_ID")->getValue<std::string>();
        m_bgCommentSprite_animateID = OGDBSetting::get("PROFILE_BACKGROUND_COMMENT_CUSTOM_ANIMATE_ID")->getValue<int>();
        // ---------- //

        startViewImage();

        //
        auto bgViewerC9 = CCScale9Sprite::create("dark_b_00.png"_spr);
        auto bgViewerlabel = CCLabelBMFont::create("Background preview", "goldFont.fnt");
        bgViewerlabel->setPosition({363.f,162.f});
        bgViewerlabel->setScale(0.25);
        m_mainLayer->addChild(bgViewerlabel);
        if(m_bgSprite_charNode) {
            bgViewerC9->setScale(0.5f); // 
            bgViewerC9->setPosition(m_bgSprite_charNode->getPosition());
            bgViewerC9->setAnchorPoint(m_bgSprite_charNode->getAnchorPoint());
            bgViewerC9->setContentSize((m_bgSprite_charNode->getContentSize() * 2) + CCSize(10,10));
            bgViewerC9->setScale(0.5f);
        }
        bgViewerC9->setOpacity(165);
        m_mainLayer->addChild(bgViewerC9);

        auto bgViewerBtnsC9 = CCScale9Sprite::create("dark_b_00.png"_spr);
        bgViewerBtnsC9->setScale(0.5f); // 
        bgViewerBtnsC9->setAnchorPoint({0.5f,0.5f});
        bgViewerBtnsC9->setPosition(132.5f,215.f);
        bgViewerBtnsC9->setContentSize({300.f, 115.f});
        bgViewerBtnsC9->setOpacity(165);
        m_mainLayer->addChild(bgViewerBtnsC9);
        // -



        auto winSize = CCDirector::sharedDirector()->getWinSize();
        float xStart = 10.f, yStart = 170.f, yStep = 22.5f;

        auto makeColorPicker = [&](int tag_color, float y, const char* title, ccColor4B& color, ColorChannelSprite** outSprite, CCLabelBMFont** outPercent, CCMenu** buttonsColors) {
            auto label = CCLabelBMFont::create(title, "bigFont.fnt");
            float baseScale = 0.45f;
            label->setScale(baseScale);
            label->setAnchorPoint({0.f,0.5f});
            label->setPosition({xStart + 75, y});
            m_mainLayer->addChild(label);

            float realWidth = label->getContentSize().width * baseScale;

            // log::info("REAL INFO {} / {} / {}", realWidth, label->getContentSize().width, baseScale);

            if (label->getContentSize().width > 215.f) {
                float newScale = (215.f / label->getContentSize().width) - baseScale;
                if (newScale < 0.25f) newScale = 0.25f;
                label->setScale(newScale);
            }

            auto sprite = ColorChannelSprite::create();
            sprite->setColor(ccc3(color.r, color.g, color.b));
            *outSprite = sprite;
            

            auto btn = CCMenuItemSpriteExtra::create(sprite, this, menu_selector(CustomizeProfilePopup::openColorPopup));
            // btn->setUserObject(reinterpret_cast<int>(tag_color));
            btn->setTag(tag_color);
            btn->setScale(0.6f);
            btn->m_baseScale = 0.6f;


            auto menu = CCMenu::createWithItem(btn);
            menu->setAnchorPoint({0.535f, 0.5f});
            menu->setPosition({xStart + 80, y});
            menu->setLayout(AxisLayout::create()
                ->setAxisAlignment(AxisAlignment::Center)
                ->setCrossAxisAlignment(AxisAlignment::Center)
                ->setCrossAxisLineAlignment(AxisAlignment::Start)
                ->setGap(0.f)
                ->setAutoScale(false));
            m_mainLayer->addChild(menu);

            auto percent = CCLabelBMFont::create(fmt::format("{}%", static_cast<int>((color.a / 255.f) * 100)).c_str(), "goldFont.fnt");
            percent->setPosition({(xStart + 78) - (sprite->getScaledContentWidth() / 2), (y + 9) - (sprite->getScaledContentHeight() / 2)});
            percent->setScale(0.275f);
            percent->setAnchorPoint({0.5f,0.f});
            *outPercent = percent;
            m_mainLayer->addChild(percent);

            // Set Auto Color btns
            // Col1
            CCMenuItemSpriteExtra* color1Btn = CCMenuItemSpriteExtra::create(
                ButtonSprite::create("Col1", "bigFont.fnt", "GJ_button_04.png", 0.5f),
                this, menu_selector(CustomizeProfilePopup::setDefaultColorPlayers)
            );
            color1Btn->setTag((tag_color * 10) + 1);
            this->setFullScale(color1Btn, 0.4f);

            // auto colorDefaultBtns = createMenuWithItem(color1Btn, CCPoint({(label->getPositionX() + label->getScaledContentWidth() + 2.5f), y}), ccp(0.f, 0.5f));
            
            auto colorDefaultBtns = createMenuWithItem(
                color1Btn,
                CCPoint(label->getPositionX() + label->getScaledContentWidth() + 2.5f, y),
                ccp(0.f, 0.5f)
            );
            colorDefaultBtns->setLayout(static_cast<AxisLayout*>(colorDefaultBtns->getLayout())->setGap(5.f)->setAxisAlignment(AxisAlignment::Start));

            // Col2
            CCMenuItemSpriteExtra* color2Btn = CCMenuItemSpriteExtra::create(
                ButtonSprite::create("Col2", "bigFont.fnt", "GJ_button_04.png", 0.5f),
                this, menu_selector(CustomizeProfilePopup::setDefaultColorPlayers)
            );
            color2Btn->setTag((tag_color * 10) + 2);
            this->setFullScale(color2Btn, 0.4f);
            colorDefaultBtns->addChild(color2Btn);


            // Glow
            CCMenuItemSpriteExtra* colorGlowBtn = CCMenuItemSpriteExtra::create(
                ButtonSprite::create("Glow", "bigFont.fnt", "GJ_button_04.png", 0.5f),
                this, menu_selector(CustomizeProfilePopup::setDefaultColorPlayers)
            );
            colorGlowBtn->setTag((tag_color * 10) + 3);
            this->setFullScale(colorGlowBtn, 0.4f);
            colorDefaultBtns->addChild(colorGlowBtn);

            *buttonsColors = colorDefaultBtns;

            colorDefaultBtns->updateLayout();

            m_mainLayer->addChild(colorDefaultBtns);

        };

        makeColorPicker(1, yStart - yStep * 0, "BG Color 1", colorPicked1, &colorSprite1, &percent1, &buttonsColorsPer1);
        makeColorPicker(2, yStart - yStep * 1, "BG Color 2", colorPicked2, &colorSprite2, &percent2, &buttonsColorsPer2);
        makeColorPicker(3, yStart - yStep * 2, "BG Sublayer Color", colorPicked3, &colorSprite3, &percent3, &buttonsColorsPer3);
        makeColorPicker(4, yStart - yStep * 3, "Border Color", colorPicked4, &colorSprite4, &percent4, &buttonsColorsPer4);
        makeColorPicker(5, yStart - yStep * 4, "BG Color Icons", colorPicked5, &colorSprite5, &percent5, &buttonsColorsPer5);
        makeColorPicker(6, yStart - yStep * 5, "BG Color Comments", colorPicked6, &colorSprite6, &percent6, &buttonsColorsPer6);

        auto makeToggle = [&](int tag_id, float y, const char* title, CCMenuItemToggler** outToggle, bool defaultState) {
            auto on = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
            auto off = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
            auto toggle = CCMenuItemToggler::create(off, on, this, menu_selector(CustomizeProfilePopup::onToggleChanged));
            toggle->toggle(defaultState);
            toggle->setTag(tag_id);
            toggle->setScale(0.5f);
            *outToggle = toggle;

            

            auto menu = CCMenu::createWithItem(toggle);
            menu->setAnchorPoint({0.53f, 0.5f});
            menu->setPosition({xStart + 265, y});
            menu->setLayout(AxisLayout::create()
                ->setAxisAlignment(AxisAlignment::Center)
                ->setCrossAxisAlignment(AxisAlignment::Center)
                ->setCrossAxisLineAlignment(AxisAlignment::Start)
                ->setGap(0.f)
                ->setAutoScale(false));
            m_mainLayer->addChild(menu);

            auto label = CCLabelBMFont::create(title, "bigFont.fnt");
            label->setScale(0.45);
            label->setAnchorPoint({0, 0.5f});
            label->setPosition({xStart + 265, y});
            m_mainLayer->addChild(label);
        };

        // makeToggle(1, (yStart - 45) - yStep * 0, "Player color 1", &toggle1, defaultToggle1);
        // makeToggle(2, (yStart - 45) - yStep * 1, "Player color 2", &toggle2, defaultToggle2);
        // makeToggle(3, (yStart - 45) - yStep * 2, "Player glow", &toggle3, defaultToggle3);
        // makeToggle(4, (yStart - 45) - yStep * 3, "BG Custom Image", &toggle4, defaultToggle4);

        // Profile Select button
        
        CCMenuItemSpriteExtra* showProfileBtn = CCMenuItemSpriteExtra::create(
            spriteBtnShowProfile,
            this, menu_selector(CustomizeProfilePopup::onShowOwnProfile)
        );

        this->setFullScale(showProfileBtn, 0.525f);
        m_buttonMenu->addChild(createMenuWithItem(showProfileBtn, {135, 226}, {0.5f, 0.5f}));

        // BG Select button


        CCMenuItemSpriteExtra* selectPresetBtn = CCMenuItemSpriteExtra::create(
            spriteBtnBGPreset,
            this, menu_selector(CustomizeProfilePopup::onSelectBGPresetProfile)
        );
        this->setFullScale(selectPresetBtn, 0.43f);
        m_buttonMenu->addChild(createMenuWithItem(selectPresetBtn, {135, 202.5}, {0.5f, 0.5f}));


        // ----
        float offsetBtnMenu = 25.5f;
        float baseScaleBtnMenu = 0.45f;

        CCMenuItemSpriteExtra* selectBGProfileBtn = CCMenuItemSpriteExtra::create(
            spriteBtnBGProfile,
            this, menu_selector(CustomizeProfilePopup::onSelectBGProfile)
        );
        this->setFullScale(selectBGProfileBtn, baseScaleBtnMenu);
        m_buttonMenu->addChild(createMenuWithItem(selectBGProfileBtn, {xStart + offsetBtnMenu,  yStart - yStep * 0}, {0.5f, 0.5f}));

        CCMenuItemSpriteExtra* selectSublayerBtn = CCMenuItemSpriteExtra::create(
            spriteBtnBGSublayer,
            this, menu_selector(CustomizeProfilePopup::onSelectSubBGProfile)
        );
        this->setFullScale(selectSublayerBtn, baseScaleBtnMenu);
        // selectSublayerBtn->setTag(0);
        m_buttonMenu->addChild(createMenuWithItem(selectSublayerBtn, {xStart + offsetBtnMenu,  yStart - yStep * 2}, {0.5f, 0.5f}));
        
        CCMenuItemSpriteExtra* selectBorderBtn = CCMenuItemSpriteExtra::create(
            spriteBtnBorder,
            this, menu_selector(CustomizeProfilePopup::onSelectBorderProfile)
        );
        this->setFullScale(selectBorderBtn, baseScaleBtnMenu);
        m_buttonMenu->addChild(createMenuWithItem(selectBorderBtn, {xStart + offsetBtnMenu,  yStart - yStep * 3}, {0.5f, 0.5f}));

        CCMenuItemSpriteExtra* selectIconBtn = CCMenuItemSpriteExtra::create(
            spriteBtnBGIcon,
            this, menu_selector(CustomizeProfilePopup::onSelectIconBGProfile)
        );
        // selectIconBtn->setTag(1);
        this->setFullScale(selectIconBtn, baseScaleBtnMenu);
        m_buttonMenu->addChild(createMenuWithItem(selectIconBtn, {xStart + offsetBtnMenu,  yStart - yStep * 4}, {0.5f, 0.5f}));

        CCMenuItemSpriteExtra* selectCommentBtn = CCMenuItemSpriteExtra::create(
            spriteBtnBGComment,
            this, menu_selector(CustomizeProfilePopup::onSelectCommentBGProfile)
        );
        // selectCommentBtn->setTag(2);
        this->setFullScale(selectCommentBtn, baseScaleBtnMenu);
        m_buttonMenu->addChild(createMenuWithItem(selectCommentBtn, {xStart + offsetBtnMenu,  yStart - yStep * 5}, {0.5f, 0.5f}));

        // Save Button
        saveButton = CCMenuItemSpriteExtra::create(saveButtonSprite, 
            this,
            menu_selector(CustomizeProfilePopup::onSave)
        );
        this->setFullScale(saveButton, 0.6f);
        
        saveButton->setEnabled(false);

        auto exitButton = CCMenuItemSpriteExtra::create(exitButtonSprite, 
            this,
            menu_selector(CustomizeProfilePopup::onClose)
        );
        this->setFullScale(exitButton, 0.6f);

        auto CCMenuBottomItems = createMenuWithItem(saveButton, {218, 17.5}, {0.5f, 0.5f});
        CCMenuBottomItems->addChild(exitButton);
        CCMenuBottomItems->setLayout(static_cast<AxisLayout*>(CCMenuBottomItems->getLayout())->setGap(15.f)->setAxisReverse(true));
        CCMenuBottomItems->updateLayout();

        m_buttonMenu->addChild(CCMenuBottomItems);

        this->setBlockedSaveBtn(false);
        return true;
    }

    void onSave(CCObject*) {
        Mod::get()->setSavedValue<ccColor4B>("PROFILE_BACKGROUND_COLOR", colorPicked1);
        Mod::get()->setSavedValue<ccColor4B>("PROFILE_BACKGROUND_COLOR_2", colorPicked2);
        Mod::get()->setSavedValue<ccColor4B>("PROFILE_BACKGROUND_SUBLAYER_COLOR", colorPicked3);
        Mod::get()->setSavedValue<ccColor4B>("PROFILE_BACKGROUND_BORDER_COLOR", colorPicked4);
        Mod::get()->setSavedValue<ccColor4B>("PROFILE_BACKGROUND_ICONS_COLOR", colorPicked5);
        Mod::get()->setSavedValue<ccColor4B>("PROFILE_BACKGROUND_COMMENTS_COLOR", colorPicked6);

        // Mod::get()->setSavedValue<bool>("PROFILE_ENABLE_COLOR_PLAYER", !toggle1->isToggled());
        // Mod::get()->setSavedValue<bool>("PROFILE_ENABLE_COLOR_PLAYER_2", !toggle2->isToggled());
        // Mod::get()->setSavedValue<bool>("PROFILE_ENABLE_COLOR_PLAYER_GLOW", !toggle3->isToggled());
        // Mod::get()->setSavedValue<bool>("PROFILE_ENABLE_BG_CUSTOM_IMAGE", !toggle4->isToggled());

        Mod::get()->setSavedValue("PROFILE_BACKGROUND_CUSTOM_SPRITE", std::string(m_bgSprite_char));
        Mod::get()->setSavedValue("PROFILE_BACKGROUND_CUSTOM_SPRITE_ID", std::string(m_bgSprite_charID));
        Mod::get()->setSavedValue<int>("PROFILE_BACKGROUND_CUSTOM_ANIMATE_ID", m_bgSprite_animateID);

        Mod::get()->setSavedValue("PROFILE_BORDER_CUSTOM_SPRITE", std::string(m_borderSprite_char));
        Mod::get()->setSavedValue("PROFILE_BORDER_CUSTOM_SPRITE_ID", std::string(m_borderSprite_charID));
        Mod::get()->setSavedValue<int>("PROFILE_BORDER_CUSTOM_ANIMATE_ID", m_borderSprite_animateID);

        Mod::get()->setSavedValue("PROFILE_BACKGROUND_SUBLAYER_CUSTOM_SPRITE", std::string(m_bgSubSprite_char));
        Mod::get()->setSavedValue("PROFILE_BACKGROUND_SUBLAYER_CUSTOM_SPRITE_ID", std::string(m_bgSubSprite_charID));
        Mod::get()->setSavedValue<int>("PROFILE_BACKGROUND_SUBLAYER_CUSTOM_ANIMATE_ID", m_bgSubSprite_animateID);

        Mod::get()->setSavedValue("PROFILE_BACKGROUND_ICON_CUSTOM_SPRITE", std::string(m_bgIconSprite_char));
        Mod::get()->setSavedValue("PROFILE_BACKGROUND_ICON_CUSTOM_SPRITE_ID", std::string(m_bgIconSprite_charID));
        Mod::get()->setSavedValue<int>("PROFILE_BACKGROUND_ICON_CUSTOM_ANIMATE_ID", m_bgIconSprite_animateID);
        // -----
        Mod::get()->setSavedValue("PROFILE_BACKGROUND_COMMENT_CUSTOM_SPRITE", std::string(m_bgCommentSprite_char));
        Mod::get()->setSavedValue("PROFILE_BACKGROUND_COMMENT_CUSTOM_SPRITE_ID", std::string(m_bgCommentSprite_charID));
        Mod::get()->setSavedValue<int>("PROFILE_BACKGROUND_COMMENT_CUSTOM_ANIMATE_ID", m_bgCommentSprite_animateID);
        

        // defaultToggle1 = !toggle1->isOn();
        // defaultToggle2 = toggle2->isOn();
        // defaultToggle3 = toggle3->isOn();
        // defaultToggle4 = toggle4->isOn();
        hasUncommitChanges = false;
        this->setBlockedSaveBtn(false);
    }

    void setFullScale(CCMenuItemSpriteExtra*& item, float scaleItem = 0.7f) {
        
        item->m_baseScale = scaleItem;
        item->setScale(scaleItem);
    }

    CCMenu* createMenuWithItem(
        CCMenuItem* item,
        CCPoint position = {0.f, 0.f},
        CCPoint anchor = {0.5f, 0.5f}
    ) {
        auto menu = CCMenu::createWithItem(item);
        menu->setAnchorPoint(anchor);
        menu->setPosition(position);
        menu->setLayout(AxisLayout::create()
            ->setAxisAlignment(AxisAlignment::Center)
            ->setCrossAxisAlignment(AxisAlignment::Center)
            ->setCrossAxisLineAlignment(AxisAlignment::Start)
            ->setGap(0.f)
            ->setAutoScale(false));
        return menu;
    }

    void setBlockedSaveBtn(bool isEnabled) {
        if(isEnabled) {
            saveButtonSprite->setColor(ccWHITE);
            saveButtonSprite->setOpacity(255);
            saveButton->setEnabled(true);
        } else {
            saveButtonSprite->setColor(ccGRAY);
            saveButtonSprite->setOpacity(155);
            saveButton->setEnabled(false);
        }
    }

    void startViewImage() {
        if (m_bgSprite_charNode) {
            
            m_bgSprite_charNode->removeFromParent();
            m_bgSprite_charNode = nullptr;
        }

        log::info("Loaded: {} {}", m_bgSprite_char, m_bgSprite_charID);
        
        m_bgSprite_charNode = OGDBAutoImage::setAutoImage(110.f, 72.5f, m_bgSprite_char, m_bgSprite_charID, m_bgSprite_animateID, colorPicked1, colorPicked2);
        m_bgSprite_charNode->setPosition({360,205});
        m_bgSprite_charNode->setID("ogdb-autoimage-node"_spr);
        m_mainLayer->addChild(m_bgSprite_charNode, 1);
    }

    void onSelectBGPresetProfile(CCObject*) {
        auto FLSelector = FLSelectorLayer::create();
        int bgPresetID = Mod::get()->getSavedValue<int>("PROFILE_BACKGROUND_PRESET_ID", -1);
        int indexID = 0;
        
        for (const auto& key : OGDBPresetProfile::getOrderedKeys()) {
            auto* preset = OGDBPresetProfile::get(key.c_str());
            if (preset) {
                FLSelector->addOption(preset->getName(), (bgPresetID == 0), indexID);
                indexID++;
            }
        }
        FLSelector->update();
        
       

        FLSelector->show();
    }

    void onSelectBGProfile(CCObject*) {
        auto FLSelector = FLSelectorLayer::create();
        // int selectedID = Mod::get()->getSavedValue<int>("PROFILE_BACKGROUND_SPRITE_ID", -1);
        int indexID = 0;

        for (const auto& key : CustomBackgroundSprite::getOrderedKeys()) {
            auto* preset = CustomBackgroundSprite::get(key.c_str());
            if (preset) {
                FLSelector->addOption(preset->getName(), (m_bgSprite_charID == preset->getID()), indexID);
                indexID++;
            }
        }

        FLSelector->update();

        FLSelector->setCallback([this](std::vector<std::pair<int, std::string>> const& selected) {
            for (auto const& [id, title] : selected) {
                const auto& keys = CustomBackgroundSprite::getOrderedKeys();
                if (id >= 0 && id < static_cast<int>(keys.size())) {
                    CustomBackgroundSprite* preset = CustomBackgroundSprite::get(keys[id].c_str());
                    if (preset) {
                        const char* spritePath = preset->getSprite();
                        const char* spriteID = preset->getID();
                        // log::info("BG selected: {}", spritePath);
                        m_bgSprite_char = spritePath;
                        // log::info("BG selected test {}", m_bgSprite_char);
                        m_bgSprite_charID = spriteID;
                        startViewImage();
                        checkIfModified();
                    }
                }
            }
        });

        FLSelector->show();
    }

    void onSelectBorderProfile(CCObject*) {
        auto FLSelector = FLSelectorLayer::create();
        // int selectedID = Mod::get()->getSavedValue<int>("PROFILE_BACKGROUND_SPRITE_ID", -1);

        int indexID = 0;

        for (const auto& key : CustomBorderSprite::getOrderedKeys()) {
            auto* preset = CustomBorderSprite::get(key.c_str());
            if (preset) {
                FLSelector->addOption(preset->getName(), (m_borderSprite_charID == preset->getID()), indexID);
                indexID++;
            }
        }

        FLSelector->update();

        FLSelector->setCallback([this](std::vector<std::pair<int, std::string>> const& selected) {
            for (auto const& [id, title] : selected) {
                const auto& keys = CustomBorderSprite::getOrderedKeys();
                if (id >= 0 && id < static_cast<int>(keys.size())) {
                    CustomBorderSprite* preset = CustomBorderSprite::get(keys[id].c_str());
                    if (preset) {
                        const char* spritePath = preset->getSprite();
                        const char* spriteID = preset->getID();
                        const CCRect spriteRect = preset->getRect();
                        spriteBtnBorder->updateBGImage(spritePath);

                        m_borderSprite_char = spritePath;
                        m_borderSprite_charID = spriteID;
                        checkIfModified();
                    }
                }
            }
        });


        FLSelector->show();
    }

    // ------------------------------------------------------ //

    void onSelectIconBGProfile(CCObject*) {
        auto FLSelector = FLSelectorLayer::create();

        int indexID = 0;

        for (const auto& key : CustomIconsBackground::getOrderedKeys()) {
            auto* preset = CustomIconsBackground::get(key.c_str());
            if (preset) {
                FLSelector->addOption(preset->getName(), (m_bgIconSprite_charID == preset->getID()), indexID);
                indexID++;
            }
        }

        FLSelector->update();

        FLSelector->setCallback([this](std::vector<std::pair<int, std::string>> const& selected) {
            for (auto const& [id, title] : selected) {
                const auto& keys = CustomIconsBackground::getOrderedKeys();
                if (id >= 0 && id < static_cast<int>(keys.size())) {
                    CustomIconsBackground* preset = CustomIconsBackground::get(keys[id].c_str());
                    if (preset) {
                        const char* spritePath = preset->getSprite();
                        const char* spriteID = preset->getID();
                        const CCRect spriteRect = preset->getRect();

                        spriteBtnBGIcon->updateBGImage(spritePath);
                        m_bgIconSprite_char = spritePath;
                        m_bgIconSprite_charID = spriteID;

                        checkIfModified();
                    }
                }
            }
        });


        FLSelector->show();
    }

    // ------------------------------------------------------ //

    // ------------------------------------------------------ //

    void onSelectCommentBGProfile(CCObject*) {
        auto FLSelector = FLSelectorLayer::create();

        int indexID = 0;

        for (const auto& key : CustomCommentsBackground::getOrderedKeys()) {
            auto* preset = CustomCommentsBackground::get(key.c_str());
            if (preset) {
                FLSelector->addOption(preset->getName(), (m_bgCommentSprite_charID == preset->getID()), indexID);
                indexID++;
            }
        }

        FLSelector->update();

        FLSelector->setCallback([this](std::vector<std::pair<int, std::string>> const& selected) {
            for (auto const& [id, title] : selected) {
                const auto& keys = CustomCommentsBackground::getOrderedKeys();
                if (id >= 0 && id < static_cast<int>(keys.size())) {
                    CustomCommentsBackground* preset = CustomCommentsBackground::get(keys[id].c_str());
                    if (preset) {
                        const char* spritePath = preset->getSprite();
                        const char* spriteID = preset->getID();
                        const CCRect spriteRect = preset->getRect();

                        spriteBtnBGComment->updateBGImage(spritePath);
                        m_bgCommentSprite_char = spritePath;
                        m_bgCommentSprite_charID = spriteID;

                        checkIfModified();
                    }
                }
            }
        });


        FLSelector->show();
    }

    // ------------------------------------------------------ //

    /*
        Previously this was together with IconBG and commentBG 
        but for more customization it is now separate :)
    */


    void onSelectSubBGProfile(CCObject* ObjectSelect) { //


        auto FLSelector = FLSelectorLayer::create();

        int indexID = 0;

        for (const auto& key : CustomSubbackground::getOrderedKeys()) {
            auto* preset = CustomSubbackground::get(key.c_str());
            if (preset) {
                FLSelector->addOption(preset->getName(), (m_bgSubSprite_charID == preset->getID()), indexID);
                indexID++;
            }
        }

        FLSelector->update();

        FLSelector->setCallback([this](std::vector<std::pair<int, std::string>> const& selected) {
            for (auto const& [id, title] : selected) {
                const auto& keys = CustomSubbackground::getOrderedKeys();
                if (id >= 0 && id < static_cast<int>(keys.size())) {
                    CustomSubbackground* preset = CustomSubbackground::get(keys[id].c_str());
                    if (preset) {
                        const char* spritePath = preset->getSprite();
                        const char* spriteID = preset->getID();
                        const CCRect spriteRect = preset->getRect();

                        spriteBtnBGSublayer->updateBGImage(spritePath);
                        m_bgSubSprite_char = spritePath;
                        m_bgSubSprite_charID = spriteID;

                        spriteBtnBGComment->updateBGImage(spritePath);
                        m_bgCommentSprite_char = spritePath;
                        m_bgCommentSprite_charID = spriteID;

                        checkIfModified();
                    }
                }
            }
        });

    }
    

    void checkIfModified() {
        log::info("Init moddified");
        log::info("Loaded M: {} - {}", m_bgSprite_char, m_bgSprite_charID);
       
        /*
            ok this code is broken idk how create that XDXDXDXDXD imbad bruh

        */ 

        hasUncommitChanges = true;        
        this->setBlockedSaveBtn(hasUncommitChanges);
    }

    void onToggleChanged(CCObject* toggle) {
        auto actualToggle = static_cast<CCMenuItemToggler*>(toggle);
        bool isToggled = !actualToggle->isToggled();
        int tagToggle = actualToggle->getTag();
        log::info("MyTag selected {} / {}", tagToggle, isToggled);
        switch (tagToggle) {
            case 1: {
                defaultToggle1 = !actualToggle->isToggled();
            }
            case 2: {
                defaultToggle2 = !actualToggle->isToggled();
            }
            case 3: {
                defaultToggle3 = !actualToggle->isToggled();
            }
            case 4: {
                defaultToggle4 = !actualToggle->isToggled();
            }
            default:
                break;
        }
        checkIfModified();
    }

    void onShowOwnProfile(CCObject*) {
        ProfilePage::create(GJAccountManager::get()->m_accountID, false)->show();
    }

    // CCMenu& getButtonsPersistent(int groupIndex) {
    //     switch (groupIndex) {
    //         case 1: return buttonsColorsPer1;
    //         case 2: return buttonsColorsPer2;
    //         case 3: return buttonsColorsPer3;
    //         case 4: return buttonsColorsPer4;
    //         case 5: return buttonsColorsPer5;
    //         case 6: return buttonsColorsPer6;
    //         default: return buttonsColorsPer6;
    //     }
    // }

    int& getColorPersistent(int groupIndex) {
        switch (groupIndex) {
            case 1: return colorPersistentToggle1;
            case 2: return colorPersistentToggle2;
            case 3: return colorPersistentToggle3;
            case 4: return colorPersistentToggle4;
            case 5: return colorPersistentToggle5;
            case 6: return colorPersistentToggle6;
            default: return colorPersistentToggle6;
        }
    }

    ccColor4B& getColorPickerForGroup(int groupIndex) {
        switch (groupIndex) {
            case 1: return colorPicked1;
            case 2: return colorPicked2;
            case 3: return colorPicked3;
            case 4: return colorPicked4;
            case 5: return colorPicked5;
            case 6: return colorPicked6;
            default: return colorPicked6;
        }
    }

    CCLabelBMFont* getPercentLabelForGroup(int groupIndex) {
        switch (groupIndex) {
            case 1: return percent1;
            case 2: return percent2;
            case 3: return percent3;
            case 4: return percent4;
            case 5: return percent5;
            case 6: return percent6;
            default: return percent6;
        }
    }

    ColorChannelSprite* getSpriteForGroup(int groupIndex) {
        switch (groupIndex) {
            case 1: return colorSprite1;
            case 2: return colorSprite2;
            case 3: return colorSprite3;
            case 4: return colorSprite4;
            /* 
                I had a stupid mistake in sprite 4, 
                in commemoration i post this comment for 
                spending all night correcting a declaration error.
            */
            case 5: return colorSprite5;
            case 6: return colorSprite6;
            default: return colorSprite6;
        }
    }

    void updateColorObject(ccColor4B& col, CCLabelBMFont* percentObj, ColorChannelSprite* spriteColor) {
        spriteColor->setColor({col.r, col.g , col.b});
        
        percentObj->setString(fmt::format("{}%", static_cast<int>((col.a / 255.0f) * 100)).c_str(), true);
    }

    void setDefaultColorPlayers(CCObject* sender) {
        auto actualNode = static_cast<CCMenuItemSpriteExtra*>(sender);
        int tag = actualNode->getTag();

        int groupIndex = tag / 10; 
        int colorType = tag % 10; 

        auto gm = GameManager::sharedState();

        auto color1 = OGDBUtils::ToColor4B(gm->colorForIdx(gm->m_playerColor));
        auto color2 = OGDBUtils::ToColor4B(gm->colorForIdx(gm->m_playerColor2));
        bool hasGlow = gm->m_playerGlow;
        auto glowColor = hasGlow
            ? OGDBUtils::ToColor4B(gm->colorForIdx(gm->m_playerGlowColor))
            : ccc4(255, 255, 255, 0); 

        log::info("Testing colors: {} - {} - {} - {}", color1, color2, hasGlow, glowColor);
        log::info("Tag color: {} - {} - {}", tag, groupIndex, colorType);

        cocos2d::ccColor4B& colorPicked = getColorPickerForGroup(groupIndex); 
        ColorChannelSprite* sprite = getSpriteForGroup(groupIndex);
        CCLabelBMFont* percentLabel = getPercentLabelForGroup(groupIndex);
        int& colorPersistent = getColorPersistent(groupIndex);
        // CCMenu& colorBtns = getButtonsPersistent(groupIndex);

        

        switch (colorType) {
            case 1:
                colorPersistent = (colorPersistent == 1 ? 0 : 1);
                
                
                // actualNode->setSprite(ButtonSprite::create(
                //     "Col1", 
                //     "bigFont.fnt", 
                //     colorPersistent == 1 ? "GJ_button_02.png" : "GJ_button_04.png", 
                //     0.5f
                // ));

                colorPicked = color1;
                updateColorObject(color1, percentLabel, sprite); 
                break;
            case 2:
                colorPersistent = (colorPersistent == 2 ? 0 : 2);

                // actualNode->setSprite(ButtonSprite::create(
                //     "Col2", 
                //     "bigFont.fnt", 
                //     colorPersistent == 2 ? "GJ_button_02.png" : "GJ_button_04.png", 
                //     0.5f
                // ));

                colorPicked = color2;
                updateColorObject(color2, percentLabel, sprite);
                break;
            case 3:
                colorPersistent = (colorPersistent == 3 ? 0 : 3);

                // actualNode->setSprite(ButtonSprite::create(
                //     "Glow", 
                //     "bigFont.fnt", 
                //     colorPersistent == 3 ? "GJ_button_02.png" : "GJ_button_04.png", 
                //     0.5f
                // ));

                colorPicked = glowColor;
                updateColorObject(glowColor, percentLabel, sprite);
                break;
            default:
                log::warn("Undefined color: {}", colorType);
                break;
        }

        if (groupIndex <= 2)
            startViewImage();
        checkIfModified();
    }

    void openColorPopup(CCObject* sender) {
        int colorPosition = sender->getTag(); // 1-6 only

        if (colorPosition < 1 || colorPosition > 6) {
            log::warn("Invalid colorPosition: {}", colorPosition);
            return;
        }

        ccColor4B& colorRef = getColorPickerForGroup(colorPosition);
        ColorChannelSprite* sprite = getSpriteForGroup(colorPosition);
        CCLabelBMFont* percent = getPercentLabelForGroup(colorPosition);

        auto popup = ColorPickPopup::create(colorRef);
        // popup->setColorTarget(sprite);

        auto callback = [this, colorPosition, &colorRef, sprite, percent](ccColor4B color, std::string const& percentStr) {
            colorRef = color;
            ccColor4B colorCopy = color;
            updateColorObject(colorCopy, percent, sprite);

            if (colorPosition <= 2)
                startViewImage();

            checkIfModified();
        };

        popup->setDelegate(new ColorPickPopupDelegateCallback(&colorRef, callback));
        popup->show();
    }

    void onClose(CCObject* sender) override {
        if(!hasUncommitChanges) {Popup::onClose(sender);} else {
            geode::createQuickPopup(
                "Unsaved Changes",
                "You have unsaved changes. Are you sure you want to exit without saving?",
                "Cancel", "Yes",
                [this, sender](auto, bool yes) {
                    if (yes) Popup::onClose(sender);
                }, true
            );
        }
    }

public:
    static CustomizeProfilePopup* create() {
        auto ret = new CustomizeProfilePopup();
        if (ret->initAnchored(460.f, 290.f, "geode.loader/GE_square02.png")) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }

    // ------------- //
};
