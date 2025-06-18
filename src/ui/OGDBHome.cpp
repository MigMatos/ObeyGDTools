#include "OGDBHome.hpp"

bool OGDBHome::init() {
    if (!CCLayer::init()) return false;

    auto bg = OGDBBackground::create();
    this->addChild(bg, -999);

    createNavbar();
    createScrollLayer();
    createExtraButtons();

    fetchData("Home");

    // ------------// 

    auto winSize = CCDirector::sharedDirector()->getWinSize();


    m_loadingObj = OGDBLoading::create();
    m_loadingObj->setPosition(ccp(winSize.width / 2, winSize.height / 2));
    this->addChild(m_loadingObj, 999);

    // ------------// 

    setKeypadEnabled(true);

    return true;
}

OGDBHome* OGDBHome::create() {
    auto ret = new OGDBHome();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void OGDBHome::createNavbar() {
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto navbar = CCLayerColor::create({ 0, 0, 0, 220 }, winSize.width, 30.f);
    navbar->setPosition({ 0, winSize.height - (navbar->getContentSize().height) });
    this->addChild(navbar, 1);


    // Logo image
    auto logoSpr = CCSprite::create("ogdb_logo.png"_spr);
    if (logoSpr) {
        logoSpr->setScale(0.5f);
        logoSpr->setAnchorPoint({ 0, 0.5f });
        logoSpr->setPosition({ 10, winSize.height - 15.f });

        this->addChild(logoSpr, 2);
    }


    // Login / Account
    auto m_navbarAccount = CCMenu::create();
    m_navbarAccount->setPosition(CCPointZero);
    CCMenuItemSpriteExtra* accountBtn = nullptr;
    if (false) {
        accountBtn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("My account", "chatFont.fnt", "OT_button_01.png"_spr, 1.f),
            this,
            menu_selector(OGDBHome::onAccount)
        );
        
    } else {
        accountBtn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("Login", "chatFont.fnt", "OT_button_01.png"_spr, 1.f),
            this,
            menu_selector(OGDBHome::onLogin)
        );
    }
    accountBtn->setScale(0.85f);
    accountBtn->m_baseScale = 0.85f;
    // ---------------- //

    accountBtn->setAnchorPoint({ 1.f, 0.5f }); 
    accountBtn->setPosition({ winSize.width - 10.f, winSize.height - 15.f });
    m_navbarAccount->addChild(accountBtn);
    this->addChild(m_navbarAccount, 2);

    // Center
    auto m_navbarMenuOptions = CCMenu::create();
    m_navbarMenuOptions->setPosition(CCPointZero);


    auto homeBtn = CCMenuItemFont::create("Home", this, menu_selector(OGDBHome::onNavOption));
    homeBtn->setScale(0.55f);

    auto musicBtn = CCMenuItemFont::create("Music", this, menu_selector(OGDBHome::onNavOption));
    musicBtn->setScale(0.55f);

    homeBtn->setTag(1);
    musicBtn->setTag(2);



    // float centerX = winSize.width / 2;
    // homeBtn->setPosition({ centerX - 40.f, winSize.height - 15.f });
    // musicBtn->setPosition({ centerX + 40.f, winSize.height - 15.f });

    m_navbarMenuOptions->addChild(homeBtn);
    m_navbarMenuOptions->addChild(musicBtn);

    m_navbarMenuOptions->setLayout(SimpleAxisLayout::create(Axis::Row)
        ->setMainAxisAlignment(MainAxisAlignment::Center)
        ->setCrossAxisAlignment(CrossAxisAlignment::Start)
        ->setMainAxisDirection(AxisDirection::LeftToRight)
        ->setCrossAxisDirection(AxisDirection::TopToBottom)
        ->setGap(20.f)
        // ->setMinRelativeScale(0.f)
        // ->setMaxRelativeScale(0.f)
    );

    m_navbarMenuOptions->updateLayout();
    m_navbarMenuOptions->setAnchorPoint({0.f,0.f});
    m_navbarMenuOptions->setPosition({0,-5.f});

    this->addChild(m_navbarMenuOptions, 3);
    
}

void OGDBHome::createScrollLayer() {
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    m_scrollLayer = ScrollLayer::create({ 0, 0, winSize.width, winSize.height - 50.f });
    m_scrollLayer->setPosition({ 0, 0 });
    m_scrollLayer->m_contentLayer->setLayout(ColumnLayout::create()
        ->setAxisReverse(true)
        ->setAutoGrowAxis(m_scrollLayer->getContentHeight())
        ->setCrossAxisOverflow(false)
        ->setAxisAlignment(AxisAlignment::End)
        ->setCrossAxisAlignment(AxisAlignment::Center)
        ->setGap(0)
    );
    
    this->addChild(m_scrollLayer, 0);
}

void OGDBHome::updateScrollContent(const std::string& category) {
    m_scrollLayer->m_contentLayer->removeAllChildren();


    bool hasNews = true;

    if (!hasNews) {
        auto label = CCLabelBMFont::create("No news available", "bigFont.fnt");
        label->setPosition(m_scrollLayer->getContentSize() / 2);
        m_scrollLayer->addChild(label);
        return;
    }

    for (int i = 0; i < 3; i++) {
        auto container = CCLayerColor::create({ 50, 50, 50, 180 }, 300.f, 80.f);
        container->setPosition({ 10.f, 100.f * i + 10.f });

        auto title = CCLabelBMFont::create(("Title " + std::to_string(i)).c_str(), "bigFont.fnt");
        title->setScale(0.5f);
        title->setAnchorPoint({ 0, 1 });
        title->setPosition({ 10.f, 70.f });
        container->addChild(title);

        auto desc = CCLabelBMFont::create("This is a short description.", "goldFont.fnt");
        desc->setScale(0.4f);
        desc->setAnchorPoint({ 0, 0 });
        desc->setPosition({ 10.f, 10.f });
        container->addChild(desc);

        m_scrollLayer->m_contentLayer->addChild(container);
    }

    m_scrollLayer->moveToTop();
    m_scrollLayer->m_contentLayer->updateLayout();
}

void OGDBHome::fetchData(const std::string& category) {
    // setIsLoaded(false, true);

    updateScrollContent(category);
    // setIsLoaded(true, true);
    
    // this->runAction(CCSequence::create(
    //     CCDelayTime::create(1.f), // Simulamos delay
    //     updateScrollContent(category);
    //     setIsLoaded(true, true);
    //     // CCCallFunc::create([this, category]() {
    //     //     updateScrollContent(category);
    //     //     setIsLoaded(true, true);
    //     // }),
    //     nullptr
    // ));
}

void OGDBHome::setIsLoaded(bool loaded, bool success) {
    if (!loaded) {
        m_statusLabel = CCLabelBMFont::create("Loading...", "bigFont.fnt");
        m_statusLabel->setPosition(CCDirector::sharedDirector()->getWinSize() / 2);
        this->addChild(m_statusLabel, 10);
    } else {
        if (m_statusLabel) {
            m_statusLabel->removeFromParent();
            m_statusLabel = nullptr;
        }

        if (!success) {
            m_statusLabel = CCLabelBMFont::create("Error loading news", "goldFont.fnt");
            m_statusLabel->setPosition(CCDirector::sharedDirector()->getWinSize() / 2);
            this->addChild(m_statusLabel, 10);
        }
    }
}

void OGDBHome::onLogin(CCObject*) {
    FLAlertLayer::create("Login", "Login clicked!", "OK")->show();
}

void OGDBHome::onAccount(CCObject*) {
    FLAlertLayer::create("Account", "Account clicked!", "OK")->show();
}

void OGDBHome::onNavOption(CCObject* sender) {
    auto tag = static_cast<CCMenuItem*>(sender)->getTag();
    if (tag == 1) {
        fetchData("Home");
    } else if (tag == 2) {
        fetchData("Music");
    }
}

void OGDBHome::createExtraButtons() {
    auto SettingBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_optionsBtn02_001.png"),
        this,
        menu_selector(OGDBSettingsLayer::switchToOGDBSettingScene)
    );
    SettingBtn->setAnchorPoint({ 0.f, 0.f });
    SettingBtn->setID("settings-btn"_spr);

    // ---- //

    auto backBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_undoBtn_001.png"),
        this,
        menu_selector(OGDBHome::backButtonCallback)
    );
    backBtn->setAnchorPoint({ 0.f, 0.f });
    backBtn->setID("back-btn"_spr);

    // ---- //

    auto backBtn2 = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("tier1Icon_001.png"),
        this,
        menu_selector(OGDBHome::test1)
    );
    backBtn2->setAnchorPoint({ 0.f, 0.f });
    backBtn2->setID("back-btn1"_spr);

    // ---- //

    auto backBtn3 = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("tier2Icon_001.png"),
        this,
        menu_selector(OGDBHome::test2)
    );
    backBtn3->setAnchorPoint({ 0.f, 0.f });
    backBtn3->setID("back-btn2"_spr);

    // ---- //

    auto backBtn4 = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_viewLevelsBtn_001.png"),
        this,
        menu_selector(OGDBHome::test3)
    );
    backBtn4->setAnchorPoint({ 0.f, 0.f });
    backBtn4->setID("back-btn3"_spr);

    // ---- //



    auto menu = CCMenu::create();

    menu->addChild(SettingBtn);
    menu->addChild(backBtn);

    menu->addChild(backBtn2);
    menu->addChild(backBtn3);
    menu->addChild(backBtn4);

    menu->setLayout(SimpleAxisLayout::create(Axis::Column)
        ->setMainAxisAlignment(MainAxisAlignment::Start)
        ->setCrossAxisAlignment(CrossAxisAlignment::Start)
        ->setMainAxisDirection(AxisDirection::BottomToTop)
        ->setCrossAxisDirection(AxisDirection::LeftToRight)
        ->setGap(15.f)
        // ->setMinRelativeScale(0.f)
        // ->setMaxRelativeScale(0.f)
    );

    menu->updateLayout();
    menu->setAnchorPoint({0.f,0.f});
    menu->setPosition({10.f,10.f});

    this->addChild(menu, 5);
}

void OGDBHome::test1(CCObject*) {
    m_loadingObj->start();
}

void OGDBHome::test2(CCObject*) {
    m_loadingObj->finished();
}

void OGDBHome::test3(CCObject*) {
    m_loadingObj->updateText("2%");
}


void OGDBHome::switchToOGDBHomeScene(CCObject*) {
    auto layer = OGDBHome::create(); 
    auto scene = CCScene::create();
    scene->addChild(layer);

    auto transition = CCTransitionFade::create(0.5f, scene);
    CCDirector::sharedDirector()->pushScene(transition);
}

void OGDBHome::keyBackClicked() {
    CCDirector::sharedDirector()->popSceneWithTransition(0.5f, PopTransition::kPopTransitionFade);
}

void OGDBHome::backButtonCallback(CCObject*) {
    keyBackClicked();
}
