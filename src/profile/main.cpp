#include "main.hpp"

using namespace geode::prelude;

class $modify(ProfilePage) {

    struct Fields {
        
        int m_profileMode = 0;
        bool m_fixIconsPosition = false;
        // Theme OGDB
        cocos2d::ccColor3B m_bgCommentsColor = {0,0,0};
        bool m_commentsDefault = false;
        GLubyte m_bgCommentsOpacity = 90; // Unused for now
        cocos2d::ccColor3B m_bgIconsColor = {0,0,0};
        bool m_iconsDefault = false;
        GLubyte m_bgIconsOpacity = 90; // Unused for now

        int m_lastUserID = 0;
        cocos2d::CCNode* m_containerBGCustomClip = nullptr;

        bool m_isLoadedProfile = false;

        std::string m_bgSprite_char = "transparent.png"_spr;
        std::string m_bgSprite_charID = "special_gradient";
        int m_bgSprite_animateID = 0;

        std::string m_bgBorder_char = "border_b_00.png"_spr;
        std::string m_bgBorder_charID = "border_c9_rounded_00";
        int m_bgBorder_animateID = 0;

        std::string m_bgSub_char = "dark_b_02.png"_spr;
        std::string m_bgSub_charID = "sub_c9_dark_02";
        int m_bgSub_animateID = 0;

        std::string m_bgIcons_char = "dark_b_02.png"_spr;
        std::string m_bgIcons_charID = "sub_c9_dark_02";
        int m_bgIcons_animateID = 0;

        std::string m_bgComments_char = "dark_b_02.png"_spr;
        std::string m_bgComments_charID = "sub_c9_dark_02";
        int m_bgComments_animateID = 0;
    };

	class OGDBProfile {
	public:
		static void updateNodeComment(CCScale9Sprite* nodeCommentBGCustom,
        CCNode* commentListLayerNode,
        CustomListView* listViewData) {
            
			nodeCommentBGCustom->setPosition(commentListLayerNode->getPosition());
            nodeCommentBGCustom->setZOrder(commentListLayerNode->getZOrder() - 1);
            nodeCommentBGCustom->setContentSize(listViewData->getContentSize());
            nodeCommentBGCustom->setVisible(true);
            commentListLayerNode->setVisible(true);
		}

        static void updateNodeBGProfile(CCScale9Sprite* nodeIconBGCustom,
        CCNode* nodePlayerMenu) {
            nodePlayerMenu->setVisible(true);
            nodeIconBGCustom->setPosition(nodePlayerMenu->getPosition() - CCPoint(0, 1.5));
            nodeIconBGCustom->setContentSize(nodePlayerMenu->getContentSize() + CCSize(15, 13));
            nodeIconBGCustom->setVisible(true);
        }

        static void fixIconsPos(cocos2d::CCNode* layer) {
            CCNode* nodeGJListLayer = dynamic_cast<CCNode*>(layer->getChildByIDRecursive("player-menu"));
            if(!nodeGJListLayer) return;
            nodeGJListLayer->setPosition(nodeGJListLayer->getPosition() + ccp(1.5f, 0));
            
            if (auto GJLayoutNode = nodeGJListLayer->getLayout()) {
                
                // Idk why getLayout pass if when getLayout is nullptr -_-
        
                // auto rowLayout = dynamic_cast<RowLayout*>(GJLayoutNode); 

                // Ok then create my own RowLayout :trollface:
                auto rowLayout = RowLayout::create();
                rowLayout->setGap(-0.75f);
                nodeGJListLayer->setLayout(rowLayout);
            }
        }

    //     static void getProfileDataFromOGDBAPI(cocos2d::CCNode* CCLayerMain, CCSize ImageSize, CCPoint ImagePosition) {
            
    //         std::string url = 
    //         std::string loadingText = "Loading background image...";
    //         log::info("Loading Image from API...");
    //         // auto customImgSprite = OGDBImage::Loader::loadFromURL(url, CCLayerMain->getContentSize(), false);

    //         auto customImgSprite = LazySprite::create(CCLayerMain->getContentSize(), true);
    //         customImgSprite->setScale(1.0f);
    //         customImgSprite->setAnchorPoint({0.f,0.f});

    //         customImgSprite->setLoadCallback([customImgSprite,ImageSize](Result<> res) {

    //             if (!res) {
    //                 log::error("OGDBImage: failed to load {}", res.unwrapErr());
    //             } else {
    //                 log::info("OGDBImage: Loading image...");
    //                 log::info("Data image: {}", customImgSprite);
    //                 customImgSprite->setAnchorPoint({0.f,0.f});
    //                 customImgSprite->setPosition({0,0});

    //                 auto origSize = customImgSprite->getContentSize();
    //                 float scaleX = ImageSize.width  / origSize.width;
    //                 float scaleY = ImageSize.height / origSize.height;
    //                 float scale  = std::max(scaleX, scaleY);
    //                 customImgSprite->setScale(scale);
    //             }
    //         });

    //         customImgSprite->loadFromUrl(url, geode::LazySprite::Format::kFmtJpg , true);
            

    //         auto clippingNode = cocos2d::CCClippingNode::create(cocos2d::CCLayerColor::create({0,0,0,0}, ImageSize.width, ImageSize.height));
    //         clippingNode->setInverted(false);
    //         clippingNode->addChild(customImgSprite);
            
            
    //         log::info("add to Child...");
    //         CCLayerMain->addChild(clippingNode);

    //     }
	};

	void setupCommentsBrowser(cocos2d::CCArray * a1)
	{
		ProfilePage::setupCommentsBrowser(a1);
        
        if(this->m_fields->m_profileMode == 0) return;

        auto layer = this->m_mainLayer;

        auto commentListLayerNode = static_cast<CCNode*>(layer->getChildByID("GJCommentListLayer"));
        commentListLayerNode->setZOrder(1);
        auto listNode = commentListLayerNode->getChildByID("list-view");

        // Scroll fix
        if(Scrollbar* scrollBarCommentsList = dynamic_cast<Scrollbar*>(layer->getChildByID("ogdb-commentlist-scrollbar"_spr))){
            auto scrollBarLayer = ScrollLayer::create(CCSize(100, 100));
            scrollBarCommentsList->setTarget(scrollBarLayer);
            scrollBarCommentsList->setVisible(false);
        }
        if(commentListLayerNode) commentListLayerNode->setVisible(false);
        // --- //

        if (auto listViewData = dynamic_cast<CustomListView*>(listNode)) {
            

            // Comment Lists
            if(!this->m_fields->m_commentsDefault) {
                CCScale9Sprite* nodeCommentBGCustom = static_cast<CCScale9Sprite*>(layer->getChildByID("ogdb-commentlist-background"_spr));
                OGDBUtils::CleanListLayer(layer, listViewData->getContentSize().width, listViewData->getContentSize().height);
                OGDBProfile::updateNodeComment(nodeCommentBGCustom, commentListLayerNode, listViewData);
            }

            // Init Icon-Background
            CCNode* nodePlayerMenu = static_cast<CCNode*>(layer->getChildByID("player-menu"));
            if(!this->m_fields->m_iconsDefault) {
                CCScale9Sprite* nodeIconBGCustom = static_cast<CCScale9Sprite*>(layer->getChildByID("ogdb-icon-background"_spr));
                OGDBProfile::updateNodeBGProfile(nodeIconBGCustom, nodePlayerMenu);

                // Fix icons when reload/comments reload
                if(this->m_fields->m_fixIconsPosition) {
                    OGDBProfile::fixIconsPos(layer);
                }

            } else {
                nodePlayerMenu->setVisible(true);
            }

            // Scroll
            if (Scrollbar* scrollBarCommentsList = static_cast<Scrollbar*>(layer->getChildByID("ogdb-commentlist-scrollbar"_spr))){
                CCScrollLayerExt* scrollBarLayer = dynamic_cast<CCScrollLayerExt*>(listViewData->getChildren()->objectAtIndex(0));
                scrollBarCommentsList->setTarget(scrollBarLayer);
                scrollBarCommentsList->ignoreAnchorPointForPosition(true);
                scrollBarCommentsList->setPosition(commentListLayerNode->getPosition() + CCPoint(listViewData->getContentSize().width+1.5f, 0));
                scrollBarCommentsList->setContentSize(commentListLayerNode->getContentSize());

                // Scrollbar
                int childrenCommentsCount = dynamic_cast<CCContentLayer*>(scrollBarLayer->getChildren()->objectAtIndex(0))->getChildrenCount();
                if(childrenCommentsCount >= 2) scrollBarCommentsList->setVisible(true);
            }
            
        } else if (CCScale9Sprite* nodeCommentBGCustom_Alt = dynamic_cast<CCScale9Sprite*>(layer->getChildByID("ogdb-commentlist-background"_spr))) {
            // commentBG fix
            nodeCommentBGCustom_Alt->setVisible(false);

            commentListLayerNode->setVisible(false);

            
        }
	}

    void loadPageFromUserInfo(GJUserScore * userData) {

        ProfilePage::loadPageFromUserInfo(userData);
        
        int m_profileMode = OGDBSetting::get("PROFILES_MODE")->getValue<int>();
        this->m_fields->m_profileMode = m_profileMode;
        if(this->m_fields->m_profileMode == 0) return;

        // Settings

        // int m_commentBGType = OGDBSetting::get("PROFILES_COMMENTSBACKGROUNDTYPE")->getValue<int>();
        // -----
        // int m_iconsBGType = OGDBSetting::get("PROFILES_ICONSBACKGROUNDTYPE")->getValue<int>(); 
        // ----- global
        bool m_scrollbarEnabled = OGDBSetting::get("PROFILES_COMMENTSENABLESCROLLBAR")->getValue<bool>(); 
        bool m_fixIconsPosition = OGDBSetting::get("PROFILES_ICONFIXPOSITION")->getValue<bool>();

        this->m_fields->m_fixIconsPosition = m_fixIconsPosition;

        // OGDBProfile::fixIconsPos(layer);

        // ---------- 

		int color1 = userData->m_color1;
		int color2 = userData->m_color2;
        int color3 = userData->m_color3;

		auto winSize = CCDirector::sharedDirector()->getWinSize();
		auto layer = this->m_mainLayer;
		auto BGSize = CCSize(435, 285);
        auto borderSize = CCSize(440, 290);
        auto brownBG = (CCSprite*)layer->getChildByID("background");

        brownBG->setVisible(false);
        auto BGPositionX = brownBG->getPositionX() - 217; // 1 -> 68
        auto BGPositionY = brownBG->getPositionY() - 142.5; // 0.5 -> 17.5
        auto BGBPositionX = brownBG->getPositionX();
        auto BGBPositionY = brownBG->getPositionY();
        // brownBG->setZOrder();
        auto BGPOrderZ = brownBG->getZOrder();
        
        // ---------- //

        // auto containerBGClip = CCNode::create();
        // containerBGClip->setContentSize(borderSize);
        // containerBGClip->setPosition(BGBPositionX, BGBPositionY);
        // containerBGClip->setAnchorPoint({0.5f, 0.5f});
        // containerBGClip->setID("ogdb-custom-background"_spr);
        // containerBGClip->setVisible(true);
        // containerBGClip->setZOrder(-10);

        /*
            ok porting my ObeyGDBot code to Geode
            this is the closest possible code for best compatibility :3
        */ 

        // auto label = cocos2d::CCLabelBMFont::create(
        //     "Loading background...",
        //     "bigFont.fnt"
        // );
        // label->setScale(0.425);
        // label->setPosition({225,100});
        // label->setZOrder(-1);
        // label->setOpacity(150);
        
        // auto fadeOut = cocos2d::CCFadeTo::create(1.0f, 30);
        // auto fadeIn = cocos2d::CCFadeTo::create(1.0f, 150);
        // auto seq = cocos2d::CCSequence::create(fadeOut, fadeIn, nullptr);
        // auto repeat = cocos2d::CCRepeatForever::create(seq);
        // label->runAction(repeat);
        
        // containerBGClip->addChild(label, -1);

        // this->m_fields->m_containerBGCustomClip = containerBGClip;
        


        // if (this->m_ownProfile == true) {
        //     layer->addChild(this->m_fields->m_containerBGCustomClip, BGPOrderZ - 1);
        //     // OGDBProfile::getProfileDataFromOGDBAPI(this->m_fields->m_containerBGCustomClip, borderSize, brownBG->getPosition());

        // }
        // if(!this->m_fields->m_containerBGCustomClip) 

        // -------- //
        // -------- //
        // -------- //

        if (this->m_fields->m_isLoadedProfile) return;
        else this->m_fields->m_isLoadedProfile = true;
		if (layer->getChildByTag(3) != nullptr) return;

        
        if(m_scrollbarEnabled) {
            auto scrollBarLayer = ScrollLayer::create(CCSize(100, 100));
            auto scrollView = Scrollbar::create(scrollBarLayer);
            scrollView->setID("ogdb-commentlist-scrollbar"_spr);
            scrollView->setVisible(false);
            layer->addChild(scrollView, 2);
        }

        CCNode* nodeGJListLayerBG = static_cast<CCNode*>(layer->getChildByID("icon-background"));
        nodeGJListLayerBG->setZOrder(1);
        CCSize sizeGJListLayer = nodeGJListLayerBG->getContentSize();

        bool m_isADVCustom = OGDBSetting::get("PROFILES_ENABLE_ADV_CUSTOM")->getValue<bool>();

		ccColor4B col1 = OGDBUtils::ToColor4B(GameManager::sharedState()->colorForIdx(color1));
		ccColor4B col2 = OGDBUtils::ToColor4B(GameManager::sharedState()->colorForIdx(color2));
        ccColor4B col3 = OGDBUtils::ToColor4B(GameManager::sharedState()->colorForIdx(color3));


        auto getColor = [&](int idx) -> ccColor4B {
            switch (idx) {
                case 1: return col1;
                case 2: return col2;
                case 3: return col3;
                default: return OGDBUtils::ToColor4B(ccWHITE);
            }
        };

        CCNode* bgProfile = nullptr;
        CCLayer* bgProfileInverted = nullptr;

        ccColor4B m_bgProfile_color = OGDBSetting::get("PROFILE_BACKGROUND_COLOR")->getValue<ccColor4B>();
        ccColor4B m_bgProfile_color_2 = OGDBSetting::get("PROFILE_BACKGROUND_COLOR_2")->getValue<ccColor4B>();

        int m_bgProfile_persistentColor = OGDBSetting::get("PROFILE_BACKGROUND_COLOR_PERSISTENT")->getValue<int>();
        int m_bgProfile_persistentColor_2 = OGDBSetting::get("PROFILE_BACKGROUND_COLOR_2_PERSISTENT")->getValue<int>();
        
        if(m_bgProfile_persistentColor != 0) m_bgProfile_color = getColor(m_bgProfile_persistentColor);
        if(m_bgProfile_persistentColor_2 != 0) m_bgProfile_color_2 = getColor(m_bgProfile_persistentColor_2);

        // --- E
        bool m_invertColor = OGDBSetting::get("PROFILES_SWAPBACKGROUNDCOLOR")->getValue<bool>();
        bool m_profileAnimated = OGDBSetting::get("PROFILES_ANIMATEBACKGROUND")->getValue<bool>(); 
        // ---

        // ------------------ //
        auto backgroundSpritesKeys = CustomBackgroundSprite::getOrderedKeys();
        int m_bgKey = OGDBSetting::get("PROFILES_BACKGROUNDTYPE")->getValue<int>();
        if(m_bgKey >= backgroundSpritesKeys.size() || (m_profileMode == 1 && m_isADVCustom) ) m_bgKey = 1;
        const std::string& bgSelectedKey = backgroundSpritesKeys[m_bgKey];
        CustomBackgroundSprite* bgSprite = CustomBackgroundSprite::get(bgSelectedKey.c_str());
        this->m_fields->m_bgSprite_char = bgSprite->getSprite();
        this->m_fields->m_bgSprite_charID = bgSprite->getID();

        if(m_profileMode == 2 && m_isADVCustom) {
            this->m_fields->m_bgSprite_char = OGDBSetting::get("PROFILE_BACKGROUND_CUSTOM_SPRITE")->getValue<std::string>();
            this->m_fields->m_bgSprite_charID = OGDBSetting::get("PROFILE_BACKGROUND_CUSTOM_SPRITE_ID")->getValue<std::string>();
            this->m_fields->m_bgSprite_animateID = OGDBSetting::get("PROFILE_BACKGROUND_CUSTOM_ANIMATE_ID")->getValue<int>();
        } else {
            m_bgProfile_color = col1;
            m_bgProfile_color_2 = col2;
            if(m_invertColor) {
                m_bgProfile_color = col2;
                m_bgProfile_color_2 = col1;
            }
            if(m_profileAnimated) this->m_fields->m_bgSprite_animateID = 1; // UNIQUE
        }
        bgProfile = OGDBAutoImage::setAutoImage(BGSize.width, BGSize.height, 
            this->m_fields->m_bgSprite_char, this->m_fields->m_bgSprite_charID,
            this->m_fields->m_bgSprite_animateID,
            m_bgProfile_color, m_bgProfile_color_2
        );

        bgProfile->setID("ogdb-custom-bg"_spr);
        bgProfile->ignoreAnchorPointForPosition(true);
        bgProfile->setPosition(BGPositionX, BGPositionY);
        bgProfile->setZOrder(BGPOrderZ-3);
        layer->addChild(bgProfile);

        // ------------------ //

        ccColor4B m_border_color = OGDBSetting::get("PROFILE_BACKGROUND_BORDER_COLOR")->getValue<ccColor4B>();

        int m_border_persistentColor = OGDBSetting::get("PROFILE_BACKGROUND_BORDER_COLOR_PERSISTENT")->getValue<int>();
        
        if(m_border_persistentColor != 0) m_border_color = getColor(m_border_persistentColor);

        // Borders

        auto bordersSpritesKeys = CustomBorderSprite::getOrderedKeys();
        int m_borderType = OGDBSetting::get("PROFILES_BORDERTYPE")->getValue<int>();
        //-
        bool m_tintCorner = OGDBSetting::get("PROFILES_ENABLECOLORBORDERS")->getValue<bool>();
        // _
        if( m_borderType >= bordersSpritesKeys.size() || (m_profileMode == 1 && m_isADVCustom) ) m_borderType = 1;
        const std::string& borderSelectedKey = bordersSpritesKeys[m_borderType];
        CustomBorderSprite* borderSprite = CustomBorderSprite::get(borderSelectedKey.c_str());
        this->m_fields->m_bgBorder_char = borderSprite->getSprite();
        this->m_fields->m_bgBorder_charID = borderSprite->getID();

        if(m_profileMode == 2 && m_isADVCustom) {
            this->m_fields->m_bgBorder_char = OGDBSetting::get("PROFILE_BORDER_CUSTOM_SPRITE")->getValue<std::string>();
            this->m_fields->m_bgBorder_charID = OGDBSetting::get("PROFILE_BORDER_CUSTOM_SPRITE_ID")->getValue<std::string>();
            this->m_fields->m_bgBorder_animateID = OGDBSetting::get("PROFILE_BORDER_CUSTOM_ANIMATE_ID")->getValue<int>();
        } else {
            if(!m_tintCorner) m_border_color = OGDBUtils::ToColor4B(ccWHITE);
            else m_border_color = col3;
        }

        auto borderProfile = OGDBAutoImage::setAutoImage(borderSize.width, borderSize.height, 
                this->m_fields->m_bgBorder_char, this->m_fields->m_bgBorder_charID,
                this->m_fields->m_bgBorder_animateID,
                m_border_color, m_border_color
        );

        borderProfile->setID("ogdb-custom-border"_spr);
        borderProfile->setPosition(BGBPositionX , BGBPositionY);
        layer->addChild(borderProfile, BGPOrderZ);

        // ----------------------------- //

        const cocos2d::ccColor4B b_subLdarkColor  = {0, 0, 0, 70};
        const cocos2d::ccColor4B b_subLlightColor = {255, 255, 255, 70};

        // --------- SUBLAYER --------- //

        ccColor4B m_bgSub_color = OGDBSetting::get("PROFILE_BACKGROUND_SUBLAYER_COLOR")->getValue<ccColor4B>();

        int m_bgSub_persistentColor = OGDBSetting::get("PROFILE_BACKGROUND_SUBLAYER_COLOR_PERSISTENT")->getValue<int>();
        
        if(m_bgSub_persistentColor != 0) m_bgSub_color = getColor(m_bgSub_persistentColor);

        auto m_bgSubSpritesKeys = CustomSubbackground::getOrderedKeys();
        int m_bgSubType = OGDBSetting::get("PROFILES_BACKGROUNDSUBLAYERTYPE")->getValue<int>();

        if( m_bgSubType >= m_bgSubSpritesKeys.size() || (m_profileMode == 1 && m_isADVCustom) ) m_bgSubType = 1;
        const std::string& m_bgSubSelectedKey = m_bgSubSpritesKeys[m_bgSubType];
        CustomSubbackground* m_bgSubSprite = CustomSubbackground::get(m_bgSubSelectedKey.c_str());
        this->m_fields->m_bgSub_char = m_bgSubSprite->getSprite();
        this->m_fields->m_bgSub_charID = m_bgSubSprite->getID();

        if(m_profileMode == 2 && m_isADVCustom) {
            this->m_fields->m_bgSub_char = OGDBSetting::get("PROFILE_BACKGROUND_SUBLAYER_CUSTOM_SPRITE")->getValue<std::string>();
            this->m_fields->m_bgSub_charID = OGDBSetting::get("PROFILE_BACKGROUND_SUBLAYER_CUSTOM_SPRITE_ID")->getValue<std::string>();
            this->m_fields->m_bgSub_animateID = OGDBSetting::get("PROFILE_BACKGROUND_SUBLAYER_CUSTOM_ANIMATE_ID")->getValue<int>();
        } else { 
            // -- SPECIAL --
            if((!OGDBUtils::isDarkColorCombination(m_bgProfile_color, m_bgProfile_color_2) && m_bgSubType == 1) || m_bgSubType == 2) {
                m_bgSubSprite = CustomSubbackground::get("sub_c9_dark_02");
                m_bgSub_color = b_subLdarkColor;
            } else {
                m_bgSubSprite = CustomSubbackground::get("sub_c9_light_02");
                m_bgSub_color = b_subLlightColor;
            }
            
            this->m_fields->m_bgSub_char = m_bgSubSprite->getSprite();
            this->m_fields->m_bgSub_charID = m_bgSubSprite->getID();
            this->m_fields->m_bgSub_animateID = 0;
            // ------------
        }

        auto m_bgSubImage = OGDBAutoImage::setAutoImage(BGSize.width, BGSize.height, 
                this->m_fields->m_bgSub_char, this->m_fields->m_bgSub_charID,
                this->m_fields->m_bgSub_animateID,
                m_bgSub_color, m_bgSub_color
        );

        // Special
        if(this->m_fields->m_bgSub_animateID == 0) m_bgSubImage->setScale(0.982f);
        // _

        m_bgSubImage->setID("ogdb-custom-bg-sublayer"_spr);
        m_bgSubImage->setPosition(BGBPositionX , BGBPositionY);
        layer->addChild(m_bgSubImage, BGPOrderZ-2);

        // ------------------ //
        
        // CLEAN ICON BG
        OGDBUtils::CleanListLayer(this->m_mainLayer, sizeGJListLayer.width, sizeGJListLayer.height);

        // --------- ICON BACKGROUND --------- //

        ccColor4B m_bgIcons_color = OGDBSetting::get("PROFILE_BACKGROUND_ICONS_COLOR")->getValue<ccColor4B>();
        int m_bgIcons_persistentColor = OGDBSetting::get("PROFILE_BACKGROUND_ICONS_COLOR_PERSISTENT")->getValue<int>();

        if (m_bgIcons_persistentColor != 0)
            m_bgIcons_color = getColor(m_bgIcons_persistentColor);

        auto m_bgIconsSpritesKeys = CustomSubbackground::getOrderedKeys();
        int m_bgIconsType = OGDBSetting::get("PROFILES_ICONSBACKGROUNDTYPE")->getValue<int>();

        if (m_bgIconsType >= m_bgIconsSpritesKeys.size() || (m_profileMode == 1 && m_isADVCustom))
            m_bgIconsType = 1;

        const std::string& m_bgIconsSelectedKey = m_bgIconsSpritesKeys[m_bgIconsType];
        CustomSubbackground* m_bgIconsSprite = CustomSubbackground::get(m_bgIconsSelectedKey.c_str());
        this->m_fields->m_bgIcons_char = m_bgIconsSprite->getSprite();
        this->m_fields->m_bgIcons_charID = m_bgIconsSprite->getID();

        if (m_profileMode == 2 && m_isADVCustom) {
            this->m_fields->m_bgIcons_char = OGDBSetting::get("PROFILE_BACKGROUND_ICON_CUSTOM_SPRITE")->getValue<std::string>();
            this->m_fields->m_bgIcons_charID = OGDBSetting::get("PROFILE_BACKGROUND_ICON_CUSTOM_SPRITE_ID")->getValue<std::string>();
            this->m_fields->m_bgIcons_animateID = OGDBSetting::get("PROFILE_BACKGROUND_ICON_CUSTOM_ANIMATE_ID")->getValue<int>();
        } else {
            if ((!OGDBUtils::isDarkColorCombination(m_bgProfile_color, m_bgProfile_color_2) && m_bgIconsType == 1) || m_bgIconsType == 2) {
                m_bgIconsSprite = CustomSubbackground::get("sub_c9_dark_02");
                m_bgIcons_color = b_subLdarkColor;
            } else {
                m_bgIconsSprite = CustomSubbackground::get("sub_c9_light_02");
                m_bgIcons_color = b_subLlightColor;
            }

            this->m_fields->m_bgIcons_char = m_bgIconsSprite->getSprite();
            this->m_fields->m_bgIcons_charID = m_bgIconsSprite->getID();
            this->m_fields->m_bgIcons_animateID = 0;
        }

        CCNode* nodeGJListLayer = static_cast<CCNode*>(layer->getChildByID("player-menu"));
        nodeGJListLayer->setZOrder(3);
        nodeGJListLayer->setVisible(false);

        auto m_bgIconsImage = OGDBAutoImage::setAutoImage(
            nodeGJListLayer->getContentSize().width + 15.f,
            nodeGJListLayer->getContentSize().height + 13.f,
            this->m_fields->m_bgIcons_char, this->m_fields->m_bgIcons_charID,
            this->m_fields->m_bgIcons_animateID,
            m_bgIcons_color, m_bgIcons_color
        );

        m_bgIconsImage->setID("ogdb-icon-background"_spr);
        m_bgIconsImage->setPosition(nodeGJListLayer->getPosition() - CCPoint(0, 1.5));
        m_bgIconsImage->setScale(0.95f);
        m_bgIconsImage->setVisible(false);
        layer->addChild(m_bgIconsImage, nodeGJListLayerBG->getZOrder());

        // --------- COMMENT BACKGROUND --------- //

        auto commentListLayerNode = static_cast<CCNode*>(layer->getChildByID("GJCommentListLayer"));
        if(commentListLayerNode) commentListLayerNode->setVisible(false);

        ccColor4B m_bgComments_color = OGDBSetting::get("PROFILE_BACKGROUND_COMMENTS_COLOR")->getValue<ccColor4B>();
        int m_bgComments_persistentColor = OGDBSetting::get("PROFILE_BACKGROUND_COMMENTS_COLOR_PERSISTENT")->getValue<int>();

        if (m_bgComments_persistentColor != 0)
            m_bgComments_color = getColor(m_bgComments_persistentColor);

        auto m_bgCommentsSpritesKeys = CustomSubbackground::getOrderedKeys();
        
        int m_bgCommentsType = OGDBSetting::get("PROFILES_COMMENTSBACKGROUNDTYPE")->getValue<int>();

        if (m_bgCommentsType >= m_bgCommentsSpritesKeys.size() || (m_profileMode == 1 && m_isADVCustom))
            m_bgCommentsType = 1;

        const std::string& m_bgCommentsSelectedKey = m_bgCommentsSpritesKeys[m_bgCommentsType];
        CustomSubbackground* m_bgCommentsSprite = CustomSubbackground::get(m_bgCommentsSelectedKey.c_str());
        this->m_fields->m_bgComments_char = m_bgCommentsSprite->getSprite();
        this->m_fields->m_bgComments_charID = m_bgCommentsSprite->getID();

        if (m_profileMode == 2 && m_isADVCustom) {
            this->m_fields->m_bgComments_char = OGDBSetting::get("PROFILE_BACKGROUND_COMMENT_CUSTOM_SPRITE")->getValue<std::string>();
            this->m_fields->m_bgComments_charID = OGDBSetting::get("PROFILE_BACKGROUND_COMMENT_CUSTOM_SPRITE_ID")->getValue<std::string>();
            this->m_fields->m_bgComments_animateID = OGDBSetting::get("PROFILE_BACKGROUND_COMMENT_CUSTOM_ANIMATE_ID")->getValue<int>();
        } else {
            if ((!OGDBUtils::isDarkColorCombination(m_bgProfile_color, m_bgProfile_color_2) && m_bgCommentsType == 1) || m_bgCommentsType == 2) {
                m_bgCommentsSprite = CustomSubbackground::get("sub_c9_dark_02");
                m_bgComments_color = b_subLdarkColor;
            } else {
                m_bgCommentsSprite = CustomSubbackground::get("sub_c9_light_02");
                m_bgComments_color = b_subLlightColor;
            }

            this->m_fields->m_bgComments_char = m_bgCommentsSprite->getSprite();
            this->m_fields->m_bgComments_charID = m_bgCommentsSprite->getID();
            this->m_fields->m_bgComments_animateID = 0;
        }

        auto m_bgCommentsImage = OGDBAutoImage::setAutoImage(
            340.f, 100.f,
            // nodeGJListLayer->getContentSize().width + 15.f,
            // nodeGJListLayer->getContentSize().height + 50.f,
            this->m_fields->m_bgComments_char, this->m_fields->m_bgComments_charID,
            this->m_fields->m_bgComments_animateID,
            m_bgComments_color, m_bgComments_color
        );

        m_bgCommentsImage->ignoreAnchorPointForPosition(true);
        m_bgCommentsImage->setID("ogdb-commentlist-background"_spr);
        m_bgCommentsImage->setVisible(false);

        layer->addChild(m_bgCommentsImage, BGPOrderZ - 8);

        // 


        // subBGlayertype

        // cocos2d::ccColor4B subLayerTypeColor = {0, 0, 0, 0}; // default transparent

        // switch (m_bgSublayerType) {
        //     case 1:
        //         subLayerTypeColor = OGDBUtils::isDarkColorCombination(col1, col2) ? b_subLlightColor : b_subLdarkColor;
        //         break;
        //     case 2:
        //         subLayerTypeColor = b_subLdarkColor;
        //         break;
        //     case 3:
        //         subLayerTypeColor = b_subLlightColor;
        //         break;
        // }

        // auto subLayerSubClass = CCLayerColor::create(subLayerTypeColor);
        // subLayerSubClass->setContentSize(BGSize);
        // subLayerSubClass->setID("ogdb-custom-bg-sublayer"_spr);
        // subLayerSubClass->setPosition(BGPositionX, BGPositionY);
        // subLayerSubClass->setScale(0.985f);
        // subLayerSubClass->setZOrder(BGPOrderZ-2);
        // layer->addChild(subLayerSubClass);

        // Switch main
        // auto getCC4BColorSwitcher = [&](int type) -> cocos2d::ccColor4B {
        //     switch (type) {
        //         case 2: return OGDBUtils::isDarkColorCombination(col1, col2) ? b_subLlightColor : b_subLdarkColor;
        //         case 3: return b_subLdarkColor;
        //         case 4: return b_subLlightColor;
        //         default: return {0,0,0,0};
        //     }
        // };

        // auto iconsBGTypeColor = getCC4BColorSwitcher(m_iconsBGType);
        // if(m_iconsBGType != 1) OGDBUtils::CleanListLayer(this->m_mainLayer, sizeGJListLayer.width, sizeGJListLayer.height);
        // this->m_fields->m_iconsDefault = (m_iconsBGType == 1);
        // this->m_fields->m_bgIconsColor = { iconsBGTypeColor.r, iconsBGTypeColor.g, iconsBGTypeColor.b };
        // //_
        // auto commentsBGTypeColor = getCC4BColorSwitcher(m_commentBGType);
        // this->m_fields->m_commentsDefault = (m_commentBGType == 1);
        // this->m_fields->m_bgCommentsColor = { commentsBGTypeColor.r, commentsBGTypeColor.g, commentsBGTypeColor.b };



        // --

        //  GJCommentListLayer
        // auto commentlistBGCont = CCScale9Sprite::create("square02b_001.png");
        // commentlistBGCont->ignoreAnchorPointForPosition(true);
        // commentlistBGCont->setColor(this->m_fields->m_bgCommentsColor);
        // commentlistBGCont->setOpacity(this->m_fields->m_bgCommentsOpacity);
        // commentlistBGCont->setID("ogdb-commentlist-background"_spr);
        // commentlistBGCont->setVisible(false);
        // layer->addChild(commentlistBGCont,-10);
    


    }

};