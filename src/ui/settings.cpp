#include "settings.hpp"

std::string current;

OGDBSettingsLayer* OGDBSettingsLayer::create() {
    auto ret = new OGDBSettingsLayer();
    if (ret && ret->init()) {
        ret->autorelease();
        openSettingsPopup(Mod::get(),false);
    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool OGDBSettingsLayer::init(){
    if (!CCLayer::init())
        return false;
    //INIT 
    this->setID("OGDBSettingsLayer");

    // geode::utils::Timer<std::chrono::high_resolution_clock>::elapsed();


    //Size
    auto winSize = CCDirector::get()->getWinSize();
    auto bg = OGDBBackground::create();
    this->addChild(bg,-999);

    
    auto boxMenu = CCNode::create();
    boxMenu->setID("box-menu"); 
    auto boxSize = CCSizeMake(460, 270);
    boxMenu->setContentSize(boxSize);
    auto boxPos = ccp((winSize.width - boxSize.width) / 2, ((winSize.height - boxSize.height) / 2)-10);
    boxMenu->setPosition(boxPos);

    
    auto backgroundLayer = CCLayerColor::create(ccc4(0, 0, 0, 102), boxSize.width, boxSize.height);
    backgroundLayer->setAnchorPoint({0.0f, 0.0f});
    backgroundLayer->setPosition({0.0f, 0.0f});
    backgroundLayer->setScale(1);
    backgroundLayer->setID("boxMenu-background");
    boxMenu->addChild(backgroundLayer);

    
    auto border = CCScale9Sprite::create("GJ_square07.png", CCRectMake(0, 0, 80, 80));
    border->setContentSize(boxSize);
    border->setPosition(ccp(boxSize.width / 2, boxSize.height / 2));
    border->setID("boxMenu-border");
    boxMenu->addChild(border);

    // TL BG
    auto borderSqSize = CCSizeMake(420, 223);
    auto borderInset = 2.0f;
                
    auto containerBorder = CCNode::create();
    containerBorder->setContentSize(borderSqSize);
    containerBorder->setPosition(20.5f,15.f);
    containerBorder->setID("ogdb-square-border");
    containerBorder->setZOrder(10); 

    CCPoint whiteVerts[4] = {
        ccp(0, 0),
        ccp(borderSqSize.width, 0),
        ccp(borderSqSize.width, borderSqSize.height),
        ccp(0, borderSqSize.height)
    };

    auto whiteBorder = CCDrawNode::create();
    whiteBorder->drawPolygon(whiteVerts, 4, ccc4f(0, 0, 0, 0.5f), 1.0f, ccc4f(1, 1, 1, 1));
    containerBorder->addChild(whiteBorder);

    // CCPoint blackVerts[4] = {
    //     ccp(borderInset, borderInset),
    //     ccp(borderSqSize.width - borderInset, borderInset),
    //     ccp(borderSqSize.width - borderInset, borderSqSize.height - borderInset),
    //     ccp(borderInset, borderSqSize.height - borderInset)
    // };

    // auto blackBorder = CCDrawNode::create();
    // blackBorder->drawPolygon(blackVerts, 4, ccc4f(0, 0, 0, 0), 1.0f, ccc4f(0, 0, 0, 1));
    // containerBorder->addChild(blackBorder);
    boxMenu->addChild(containerBorder);

    mainCOptions = CCLayerColor::create(ccc4(0, 0, 0, 102), borderSqSize.width, borderSqSize.height);
    mainCOptions->setAnchorPoint({0.0f, 0.0f});
    mainCOptions->setPosition({0.0f, 0.0f});
    mainCOptions->setScale(1);
    mainCOptions->setID("ogdb-square-border-background");
    mainCOptions->setZOrder(1); 
    containerBorder->addChild(mainCOptions);
    

    auto titleLabel = CCLabelBMFont::create("ObeyGDTools Settings", "goldFont.fnt");
    titleLabel->setID("boxMenu-title");
    titleLabel->setPosition(ccp(boxSize.width / 2, boxSize.height - 16.5));
    titleLabel->setScale(0.575);
    

    boxMenu->addChild(titleLabel);


    // Menú
    auto menu = CCMenu::create();
    menu->setID("menu");
    menu->setZOrder(20);

    //Back btn
    auto backBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"),
        this,
        menu_selector(OGDBSettingsLayer::backButtonCallback)
    );
    backBtn->setPosition(-winSize.width / 2 + 25.f, winSize.height / 2 - 25.f);
    backBtn->setID("back-btn");
    menu->addChild(backBtn);
    /* --- */

    // TabsBtnMenu
    auto tabButtonMenu = CCMenu::create();
    tabButtonMenu->setID("tab-button-menu");
    tabButtonMenu->setZOrder(5);
    tabButtonMenu->setLayout(RowLayout::create()->setAutoScale(false)->setGap(10.f));
    tabButtonMenu->setPosition(winSize.width / 2, winSize.height / 2 + LIST_HEIGHT / 2 + 26.f);
    tabButtonMenu->setContentSize({LIST_WIDTH + 40.f, 0.f});
    
    // BtnMenu
    tabBtn = TabButton::create(TabBaseColor::Selected,TabBaseColor::Selected, "General",this,menu_selector(OGDBSettingsLayer::onTab));
    tabBtn->setScale(TAB_SCALE);
    tabBtn->setTag(TAG_TAB_GENERAL);

    tabBtn1 = TabButton::create(TabBaseColor::Selected,TabBaseColor::Selected, "Profiles",this,menu_selector(OGDBSettingsLayer::onTab));
    tabBtn1->setScale(TAB_SCALE);
    tabBtn1->setTag(TAG_TAB_PROFILES);

    tabBtn2 = TabButton::create(TabBaseColor::Selected,TabBaseColor::Selected, "GDPS",this,menu_selector(OGDBSettingsLayer::onTab));
    tabBtn2->setScale(TAB_SCALE);
    tabBtn2->setTag(TAG_TAB_GDPS);

    // Adding
    tabButtonMenu->addChild(tabBtn);
    tabButtonMenu->addChild(tabBtn1);
    tabButtonMenu->addChild(tabBtn2);

    // UpdateLayout
    tabButtonMenu->updateLayout();
    // Final adding
    this->addChild(tabButtonMenu);


    // Nodes
    std::array<geode::TabButton*, 3> tabButtons = { tabBtn, tabBtn1, tabBtn2 };
    std::array<CCSprite*, 3> tabMasks;
    std::array<CCClippingNode*, 3> tabNodes;

    for (size_t i = 0; i < tabButtons.size(); ++i) {
        auto tabNode = CCClippingNode::create();
        tabNode->setID("gradient-clipping-node");
        tabNode->setContentSize(tabButtonMenu->getContentSize());
        tabNode->setPosition(tabButtonMenu->getPosition());
        tabNode->setLayout(tabButtonMenu->getLayout());
        tabNode->setAnchorPoint(tabButtonMenu->getAnchorPoint());
        tabNode->setZOrder(4);
        tabNode->setTag(i);
        tabNode->setAlphaThreshold(0.7f);
        tabNodes[i] = tabNode; 
    }

    for (size_t i = 0; i < tabButtons.size(); ++i) {
        auto tabBG = CCLayerColor::create(ccc4(0, 0, 0, 127));
        tabBG->setContentSize(tabButtons[i]->getContentSize());
        tabBG->setPosition(tabButtons[i]->getPosition());
        tabBG->setScale(TAB_SCALE);
        tabBGs[i] = tabBG; 

        auto tabMask = CCSprite::create("geode.loader/tab-gradient-mask.png"); // Temporal asset, so APOLOGY geode *cry*
        tabMask->setScale(TAB_SCALE);
        tabMask->setPosition(tabButtons[i]->getPosition()); 
        tabMask->setAnchorPoint(tabButtons[i]->getAnchorPoint());
        tabMasks[i] = tabMask; 
    }

    for (size_t i = 0; i < tabNodes.size(); ++i) {
        for (size_t ie = 0; ie < tabBGs.size(); ++ie) {
            tabNodes[i]->addChild(tabBGs[ie]); 
        }
        tabNodes[i]->updateLayout();
        tabNodes[i]->setStencil(tabMasks[i]);
        this->addChild(tabNodes[i]);
    }

    // add menu (TODO)
    this->addChild(menu); 
    this->addChild(boxMenu, 10);
    this->remakeList();

    // Keys
    setKeypadEnabled(true);
    return true;
}

// Geode code LMAO
void OGDBSettingsLayer::onTab(CCObject* sender) {
    for (const auto& [_, tab] : storedTabs) {
        if (tab->getParent()) {
            tab->removeFromParent();
        }
    }

    if (!storedTabs.contains(sender->getTag())) {
        return;
    }

    this->onTabById(sender->getTag());
}

void OGDBSettingsLayer::onTabById(int tag) {
    currentTab = tag;
    mainCOptions->addChild(storedTabs[currentTab]);

    auto toggleTab = [this](CCMenuItemToggler* member) {
        auto tabSelected = member->getTag();
        auto isSelected = tabSelected == currentTab;
        member->toggle(isSelected);

        if(isSelected) {
            tabBGs[tabSelected]->setColor(ccc3(255, 255, 255));
            // log::info("Selected tab: {}", tabSelected);
        } else {
            tabBGs[tabSelected]->setColor(ccc3(0, 0, 0));
        }

        if(currentTab == TAG_TAB_GDPS && isSelected){
            OGDBUtils::WIPFeature(nullptr);
        }

    };
    
    toggleTab(tabBtn);
    toggleTab(tabBtn1);
    toggleTab(tabBtn2);

    cocos::handleTouchPriority(this, true);
}

// Based in Globed/GeodeSDK code
void OGDBSettingsLayer::remakeList() {
    if (currentTab != -1 && storedTabs.contains(currentTab)) {
        storedTabs[currentTab]->removeFromParent();
    }

    currentTab = TAG_TAB_GENERAL;

    storedTabs.clear();

    settingCells[TAG_TAB_GENERAL] = CCArray::create();
    settingCells[TAG_TAB_PROFILES] = CCArray::create();
    settingCells[TAG_TAB_GDPS] = CCArray::create();

    this->registerOption();

    storedTabs[TAG_TAB_GENERAL] = this->makeListLayer(TAG_TAB_GENERAL);
    storedTabs[TAG_TAB_PROFILES] = this->makeListLayer(TAG_TAB_PROFILES);
    storedTabs[TAG_TAB_GDPS] = this->makeListLayer(TAG_TAB_GDPS);


    this->onTab(tabBtn);
}

CCNode* OGDBSettingsLayer::makeListLayer(int category) {

    //INIT
    auto listViewSize = mainCOptions->getContentSize();
    auto countOptionsCell = settingCells[category]->count();

    // 
    auto listview = ListView::create(settingCells[category], CELL_HEIGHT, listViewSize.width, listViewSize.height);
    listview->setID(std::format("settings-list-view-{}", category));
    listview->setPrimaryCellColor(ccc3(0, 0, 10));
    listview->setSecondaryCellColor(ccc3(50, 50, 60));
    listview->setCellOpacity(70);
    listview->setCellBorderColor(ccc4(0, 0, 0, 0));
    listview->setPosition({ 0.f, 0.f });
    listview->setAnchorPoint({ 0.5f, 0.5f });
    listview->setVisible(true);

    

    auto listLayer = CCNode::create();
    listLayer->setZOrder(10);
    listLayer->setAnchorPoint({ 0.5f, 0.5f });
    listLayer->setID(std::format("settings-list-layer-{}", category));
    listLayer->setPosition(mainCOptions->getPosition());

    auto listLayerColor = ccc4(0, 0, 0, 10);
    auto bgListLayer = CCLayerColor::create(listLayerColor, listViewSize.width, listViewSize.height);
    bgListLayer->setAnchorPoint({ 0.5f, 0.5f });
    bgListLayer->setPosition({ 0.f, 0.f });
    listLayer->addChild(bgListLayer, -1);

    float labelNoOptScale = 0.55;
    float labelNoOptOffset = 30;
    auto labelNoOpt = TextArea::create("No <cr>options</c> available", "bigFont.fnt", labelNoOptScale, listViewSize.width - labelNoOptOffset, {0.f, 0.5f},15.f,false);
    
    auto labelNoOptSize = labelNoOpt->getContentSize();

    float posX = (listViewSize.width / 4) + (labelNoOptOffset / 3); 
    float posY = (listViewSize.height / 2) + (labelNoOptOffset / 3);
    
    labelNoOpt->setPosition({posX, posY});
    labelNoOpt->setAnchorPoint({ 0.0f, 0.f });
    labelNoOpt->setID("no-options-label");
    labelNoOpt->setVisible(false);

    if(countOptionsCell <= 0){
        listview->setVisible(false);
        labelNoOpt->setVisible(true);
    }
    
    listLayer->addChild(labelNoOpt, 2);
    listLayer->addChild(listview, 1);

    // Scroll

    auto scrollBarLayer = ScrollLayer::create(listViewSize - CCSize(0,10));

    auto scrollView = Scrollbar::create(scrollBarLayer);
    scrollView->setAnchorPoint({ 0.0f, 0.f });
    // scrollView->setPosition(listViewSize - CCSize(13.f, 5));

    listLayer->addChildAtPosition(
        scrollView, Anchor::Center, CCPoint(listViewSize - CCSize(10.f, listViewSize.height - 5))
    );
    

    return listLayer;
}


void OGDBSettingsLayer::registerOption() {
    auto& settingsRegistry = OGDBSetting::getAllSettings();
    auto& settingsListOrder = OGDBSetting::getOrderKeySettings();

    bool autoSavedConfig = true;
    int cat = 0;
    // OGDBSettingCell::Limits limits = {};

    for (const auto& id : settingsListOrder) {
        OGDBSetting* setting = settingsRegistry.at(id);
        
        const char* rawID = setting->getID();
        std::string m_settingID = setting->getID();
        std::any m_valueDefault = setting->getDefaultValue();
        std::any m_optionsDefault = setting->getDefaultOptions();

        auto cell = OGDBSettingCell::create(
            &autoSavedConfig,
            setting->getType(),
            rawID,
            m_valueDefault,
            m_optionsDefault,
            setting->getName(),
            setting->getDescription(),
            setting->getRequiredText(),
            // limits,
            mainCOptions->getContentSize().width,
            CELL_HEIGHT
        );

        // log::info("ID KEY: {}", m_settingID);

        if (m_settingID.rfind("GENERAL_", 0) == 0) cat = TAG_TAB_GENERAL;
        else if (m_settingID.rfind("PROFILES_", 0) == 0) cat = TAG_TAB_PROFILES;
        else if (m_settingID.rfind("GDPS_", 0) == 0) cat = TAG_TAB_GDPS;

        // log::info("CATEGORY ID: {}", cat);

        settingCells[cat]->addObject(cell);
    }
}


void OGDBSettingsLayer::onToggleOption(CCObject* sender) {

    auto toggler = static_cast<CCMenuItemToggler*>(sender);
    bool enabled = !toggler->isToggled();
    std::string id = toggler->getID();
    if (id.rfind("toggle-", 0) != 0) return;
    std::string key = id.substr(7);
    Mod::get()->setSavedValue(key, enabled);
    // log::info("Toggle '{}' set to {}", key, enabled);
}

void OGDBSettingsLayer::keyBackClicked() {
    CCDirector::sharedDirector()->popSceneWithTransition(0.5f, PopTransition::kPopTransitionFade);
}

void OGDBSettingsLayer::backButtonCallback(CCObject* object) {
    keyBackClicked();
}

void OGDBSettingsLayer::switchToOGDBSettingsLayerButton(CCObject* object) {

    auto layer = OGDBSettingsLayer::create();
    auto scene = CCScene::create();
    scene->addChild(layer);

    auto transition = CCTransitionFade::create(0.5f, scene);

    CCDirector::sharedDirector()->pushScene(transition);
    
}