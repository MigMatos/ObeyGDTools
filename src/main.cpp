// Geode
#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/StatsCell.hpp>

// My funcs
#include "ui/settings.hpp"
#include "ui/OGDBHome.hpp"
#include "settings/OGDBSetting.hpp"
// E
#include "presets/OGDBPresetProfile.hpp"
#include "presets/assets/CustomBackgroundSprite.hpp"
#include "presets/assets/CustomBorderSprite.hpp"
#include "presets/assets/CustomSubbackground.hpp"
// N
#include "presets/assets/CustomCommentsBackground.hpp"
#include "presets/assets/CustomIconsBackground.hpp"

using namespace geode::prelude;


class $modify(MyMenuLayer, MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) {
			return false;
		}
		auto bntOGDBStt = CCMenuItemSpriteExtra::create(
			CCSprite::create("GJ_ogdtButton_001.png"_spr),
			this,
			menu_selector(OGDBHome::switchToOGDBHomeScene)
		);

		auto menu = this->getChildByID("bottom-menu");
		menu->addChild(bntOGDBStt);

		bntOGDBStt->setID("ogdb-settings"_spr);

		menu->updateLayout();

		return true;
	}
};

$on_mod(Loaded) {
    auto mod = Mod::get();
	OGDBSetting::init();
	OGDBPresetProfile::init();
	CustomBackgroundSprite::init();
	CustomBorderSprite::init();
	CustomSubbackground::init();
	CustomIconsBackground::init();
	CustomCommentsBackground::init();
    log::info("Mod {} version {} has been loaded.", mod->getID(), mod->getVersion());
}


class $modify(StatsCell) {

	struct Fields {
		bool m_wasInProfilePage = false;
	};
	

	void draw() {
		auto scene = CCDirector::get()->getRunningScene();

		
		//Border comments showing on transition fix
		if (scene->getChildByID("ProfilePage")) {
			this->m_fields->m_wasInProfilePage = true;
		}

		
		if (!this->m_fields->m_wasInProfilePage) {
			StatsCell::draw();
		}
	}
};
