#include <Geode/Geode.hpp>
#include <Geode/modify/CommentCell.hpp>
#include "../utils/utils.hpp"
#include "../settings/OGDBSetting.hpp"

using namespace geode::prelude;

class $modify(CommentCell) {
	struct Fields {
		bool renderedInsideProfile = false;
		bool m_wasInProfilePage = false;
	};

	void loadFromComment(GJComment* comment) {

        // log::info("Loading cell info!");

		CommentCell::loadFromComment(comment);
		

		if (comment->m_levelID == 0 || comment->m_hasLevelID == 1) {

			auto commentBGColor = (CCSprite*)this->getChildren()->objectAtIndex(0);
			commentBGColor->setVisible(false);

			const int bgMode = OGDBSetting::get("PROFILES_BACKGROUNDSUBLAYERTYPE")->getValue<int>();
			const ccColor4B overlayDark  = { 0, 0, 0, 51 };
			const ccColor4B overlayLight = { 255, 255, 255, 8 };
			ccColor4B resolvedOverlay = { 0, 0, 0, 0 };


			switch (bgMode) {
				case 1:
					resolvedOverlay = OGDBUtils::isDarkColor(OGDBUtils::ToColor4B(comment->m_color)) ? overlayLight : overlayDark;
					break;
				case 2:
					resolvedOverlay = overlayDark;
					break;
				case 3:
					resolvedOverlay = overlayLight;
					break;
			}

			if (this->m_height <= 36) {
				auto overlayLayer = CCLayerColor::create(resolvedOverlay, 890.f, 85.f);
				overlayLayer->setPosition(
					this->getPositionX() + 170.f - 445.f,
					this->getPositionY() + 18.f - 42.5f
				);
				overlayLayer->setZOrder(-1);
				this->addChild(overlayLayer);
			} else {
				auto commentLayer = static_cast<CCLayer*>(this->getChildren()->objectAtIndex(1));
				CCObject* child = nullptr;
				CCARRAY_FOREACH(commentLayer->getChildren(), child) {
					if (auto colorLayer = dynamic_cast<cocos2d::extension::CCScale9Sprite*>(child)) {
						colorLayer->setColor({ resolvedOverlay.r, resolvedOverlay.g, resolvedOverlay.b });
						colorLayer->setOpacity(resolvedOverlay.a);
					}
                    else {
                    }
				}
			}
		}
	}

#ifdef GEODE_IS_ANDROID
	void draw() {
		auto currentScene = CCDirector::sharedDirector()->getRunningScene();
		if (currentScene->getChildByID("ProfilePage")) {
			this->m_fields->renderedInsideProfile = true;
		}

		if (!this->m_fields->renderedInsideProfile) {
			CommentCell::draw();
		}
	}
#endif
};