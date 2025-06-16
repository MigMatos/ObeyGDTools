#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;


/*
    I might make it an API, but I'd need help finishing things up with customizations, 
    optimizing it, or improving the code. If you'd like to help me, open an issue 
    explaining it, and I'll open a new repository!
*/

class FLSelectorCell : public CCLayer {
protected:
    CCLabelBMFont* m_label;
    CCMenuItemToggler* m_toggle;
    int m_id;
    std::string m_title;
    std::function<void(int, bool)> m_callback;

public:
    static FLSelectorCell* create(
        std::string const& title,
        int id,
        std::function<void(int, bool)> callback
    ) {
        auto ret = new FLSelectorCell();
        if (ret && ret->init(title, id, callback)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    bool init(std::string const& title, int id, std::function<void(int, bool)> callback) {
        if (!CCLayer::init()) return false;

        m_title = title;
        m_id = id;
        m_callback = callback;

        auto container = CCNode::create();
        container->setScale(0.5f); // Escala visual interna
        container->setPosition({ 0.f, 0.f });
        this->addChild(container);

        auto menu = CCMenu::create();
        menu->setPosition(CCPointZero);
        container->addChild(menu);

        m_toggle = CCMenuItemToggler::createWithStandardSprites(
            this,
            menu_selector(FLSelectorCell::onToggle),
            1.0f
        );
        m_toggle->setPosition({ 40.f, 25.f });
        menu->addChild(m_toggle);

        m_label = CCLabelBMFont::create(title.c_str(), "bigFont.fnt");
        m_label->setAnchorPoint({ 0, 0.5f });
        m_label->setPosition({ 65.f, 26.f });
        container->addChild(m_label);

        this->setContentSize({ 220.f * 0.5f, 50.f * 0.5f }); // Tamaño real adaptado al layout

        return true;
    }

    void onToggle(CCObject*) {
        bool selected = !m_toggle->isToggled();
        if (m_callback) {
            m_callback(m_id, selected);
        }
    }

    void setSelected(bool selected) {
        m_toggle->toggle(selected);
    }

    int getID() const { return m_id; }
    std::string getTitle() const { return m_title; }
    bool isSelected() const { return m_toggle->isToggled(); }
};

/* Selector Popup */

class FLSelectorLayer : public Popup<> {
protected:
    bool m_multi = false;
    int m_min = 1;
    int m_max = 1;

    std::string m_title = "Selector";
    const char* m_FLbgSprite_char = "square01_001.png";

    CCScale9Sprite* bg_listSize = CCScale9Sprite::create("dark_b_00.png"_spr);

    CCSize m_listSize = CCSize(320.f, 212.5f);
    CCPoint m_listPosition = CCPoint(25.f, 43.5f);
    CCPoint m_listAnchor = CCPoint(0.f, 0.f);

    ScrollLayer* m_listView = nullptr;
    Scrollbar* m_scrollBar = nullptr;

    std::map<int, FLSelectorCell*> m_cellMap;
    std::set<int> m_selected;

    CCLabelBMFont* m_emptyLabel = CCLabelBMFont::create("No options", "bigFont.fnt");

    std::function<void(std::vector<std::pair<int, std::string>>)> m_callback;

    bool setup() override {
        this->setTitle(m_title);

        bg_listSize->setContentSize(m_listSize + CCSize(0.f, 5.f));
        bg_listSize->setAnchorPoint(m_listAnchor);
        bg_listSize->setOpacity(200);
        bg_listSize->setPosition(25.f , 41.5f);
        bg_listSize->setZOrder(1);
        this->m_mainLayer->addChild(bg_listSize);


        m_emptyLabel->setPosition({185.f, 145.f});
        m_emptyLabel->setColor({255,45,45});
        m_emptyLabel->setScale(0.6f);
        this->m_mainLayer->addChild(m_emptyLabel, 10);

        m_listView = ScrollLayer::create(m_listSize);
        m_listView->setTouchEnabled(true);
        m_listView->m_contentLayer->setLayout(
            ColumnLayout::create()
                ->setAxisReverse(true)
                ->setAutoGrowAxis(m_listView->getContentHeight())
                ->setCrossAxisOverflow(false)
                ->setAxisAlignment(AxisAlignment::End)
                ->setCrossAxisAlignment(AxisAlignment::Start)
                ->setGap(0)
        );
        m_listView->moveToTop();
        m_listView->setAnchorPoint(m_listAnchor);
        m_listView->setPosition(m_listPosition);
        m_listView->setZOrder(2);
        m_mainLayer->addChild(m_listView);

        m_scrollBar = Scrollbar::create(dynamic_cast<CCScrollLayerExt*>(m_listView));
        m_scrollBar->setPosition({ 350 , m_listPosition.y });
        m_scrollBar->setAnchorPoint(m_listAnchor);
        m_scrollBar->ignoreAnchorPointForPosition(true);
        m_scrollBar->setTouchEnabled(true);
        m_mainLayer->addChild(m_scrollBar);

        auto btns = CCMenu::create();
        auto cancelBtn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("Cancel"),
            this,
            menu_selector(FLSelectorLayer::onCancel)
        );
        auto okBtn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("OK"),
            this,
            menu_selector(FLSelectorLayer::onOK)
        );
        btns->addChild(cancelBtn);
        btns->addChild(okBtn);
        btns->setPosition(185.f, 133.f);
        btns->setScale(0.75f);
        btns->setLayout(SimpleAxisLayout::create(Axis::Row)
            ->setMainAxisAlignment(MainAxisAlignment::Center)
            ->setCrossAxisAlignment(CrossAxisAlignment::End)
            ->setMainAxisDirection(AxisDirection::LeftToRight)
            ->setGap(25.f)
        );
        m_mainLayer->addChild(btns);

        updateEmptyLabel();
        return true;
    }

    void onCancel(CCObject*) {
        this->removeFromParentAndCleanup(true);
    }

    void onOK(CCObject*) {
        if (m_selected.size() < static_cast<size_t>(m_min)) {
            FLAlertLayer::create("Error", "Select at least one option.", "OK")->show();
            return;
        }

        std::vector<std::pair<int, std::string>> result;
        for (int id : m_selected) {
            auto cell = m_cellMap[id];
            if (cell)
                result.emplace_back(id, cell->getTitle());
        }

        if (m_callback)
            m_callback(result);

        this->removeFromParentAndCleanup(true);
    }

    void updateEmptyLabel() {
        if (m_emptyLabel) {
            m_emptyLabel->setVisible(m_cellMap.empty());
        }
    }

    void updateScrollbar() {
        if (m_listView && m_scrollBar && m_cellMap.size() >= 11) {
            m_scrollBar->setVisible(true);
        } else {
            m_scrollBar->setVisible(false);
        }
        m_scrollBar->setTarget(dynamic_cast<CCScrollLayerExt*>(m_listView));
    }

public:
    static FLSelectorLayer* create(
        bool multi = false,
        int min = 1,
        int max = -1,
        std::string const& title = "Selector",
        const char* FLbgSprite_char = "square01_001.png",
        CCRect const& bgRect = { 0.f, 0.f, 0.f, 0.f }
    ) {
        auto ret = new FLSelectorLayer();
        if (ret && ret->initAnchored(370.f, 290.f, FLbgSprite_char, bgRect)) {
            ret->m_multi = multi;
            ret->m_min = std::max(1, min);
            ret->m_max = (max >= ret->m_min) ? max : ret->m_min;
            ret->m_title = title;
            ret->m_FLbgSprite_char = FLbgSprite_char;
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    void setCallback(std::function<void(std::vector<std::pair<int, std::string>>)> const& cb) {
        m_callback = cb;
    }

    void addOption(std::string const& title, bool selected = false, int id = -1) {
        static int autoID = 1;
        if (id == -1) id = autoID++;

        auto cell = FLSelectorCell::create(title, id, [this](int cid, bool selected) {
            if (!m_multi) {
                for (auto& entry : m_cellMap) {
                    if (entry.first != cid)
                        entry.second->setSelected(false);
                }
                m_selected.clear();
            }

            if (selected) m_selected.insert(cid);
            else m_selected.erase(cid);
        });

        cell->setSelected(selected);
        if (selected) m_selected.insert(id);
        m_cellMap[id] = cell;

        if (m_listView) {
            int index = m_listView->m_contentLayer->getChildrenCount();
            if (index % 2 != 0) {
                auto bg = CCLayerColor::create(ccc4(0, 0, 0, 40), m_listSize.width, 25.f);
                bg->setAnchorPoint({ 0.f, 0.f });
                cell->addChild(bg, -1);
            }
            m_listView->m_contentLayer->addChild(cell);
            m_listView->m_contentLayer->updateLayout();
        }

        updateEmptyLabel();
        updateScrollbar();
    }

    void clearOptions() {
        if (m_listView) {
            m_listView->m_contentLayer->removeAllChildrenWithCleanup(true);
        }
        m_cellMap.clear();
        m_selected.clear();
        updateEmptyLabel();
        updateScrollbar();
    }

    void update() {
        if (m_listView) {
            m_listView->removeFromParent();
            m_listView = nullptr;
        }

        m_listView = ScrollLayer::create(m_listSize);
        m_listView->setTouchEnabled(true);
        m_listView->m_contentLayer->setLayout(
            ColumnLayout::create()
                ->setAxisReverse(true)
                ->setAutoGrowAxis(m_listView->getContentHeight())
                ->setCrossAxisOverflow(false)
                ->setAxisAlignment(AxisAlignment::End)
                ->setCrossAxisAlignment(AxisAlignment::Start)
                ->setGap(0)
        );
        
        // m_listView->setAnchorPoint(m_listAnchor);
        m_listView->setPosition(m_listPosition);
        m_listView->setZOrder(2);
        m_mainLayer->addChild(m_listView);

        int index = 0;
        for (auto& [id, cell] : m_cellMap) {
            // if (index % 2 != 0) {
            //     auto bg = CCLayerColor::create(ccc4(0, 0, 0, 15), 110.f, 25.f);
            //     bg->setAnchorPoint({ 0.f, 0.f });
            //     cell->addChild(bg, -1);
            // }
            m_listView->m_contentLayer->addChild(cell);
            ++index;
        }

        m_listView->m_contentLayer->updateLayout();

        m_listView->moveToTop();
        
        updateEmptyLabel();
        updateScrollbar();
    }
};