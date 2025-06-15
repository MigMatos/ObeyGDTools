#include "utils.hpp"




void OGDBUtils::WIPFeature(CCObject*) 
{
	FLAlertLayer::create("WIP", "This feature is <cr>not finished</c>, wait for future <cg>updates!</c>", "OK")->show();
    return;
}

cocos2d::ccColor4B OGDBUtils::ToColor4B(ccColor3B c, GLubyte transparentValue)
{
	return ccColor4B{ c.r, c.g, c.b, transparentValue };
}

cocos2d::ccColor4F OGDBUtils::ToColor4F(ccColor3B c, float transparentValue) {
    return cocos2d::ccColor4F{c.r / 255.f,c.g / 255.f,c.b / 255.f,transparentValue};
}

bool OGDBUtils::isDarkColorCombination(ccColor4B col1, ccColor4B col2) {
    int r = (col1.r + col2.r) / 2;
    int g = (col1.g + col2.g) / 2;
    int b = (col1.b + col2.b) / 2;
    float luminance = 0.299f * r + 0.587f * g + 0.114f * b;
    return luminance < 128.f;
}

bool OGDBUtils::isDarkColor(ccColor4B col) {
    float luminance = 0.299f * col.r + 0.587f * col.g + 0.114f * col.b;
    return luminance < 128.f;
}


void OGDBUtils::autoScaleToFit(CCNode* node, float minSize, float maxSize) {
        if (!node) return;

        auto size = node->getContentSize();
        if (size.width <= 0 || size.height <= 0) return;
        float baseSize = std::max(size.width, size.height);
        float scale = 1.0f;

        if (baseSize > maxSize) {
            scale = maxSize / baseSize;
        } else if (baseSize < minSize) {
            scale = minSize / baseSize;
        }

        node->setScale(scale);
}



void OGDBUtils::centerNodeBetweenX(CCNode* node, CCNode* left, CCNode* right) {
    float leftX = left->getPositionX() + left->getScaledContentSize().width / 2.f;
    float rightX = right->getPositionX() - right->getScaledContentSize().width / 2.f;

    // log::info("TEST CREATING POS -> X: {}, Y: {}", left->getPositionX(), right->getPositionX());
    // log::info("TEST CREATING CONTENT SIZE -> X: {}, Y: {}", left->getScaledContentSize().width, right->getScaledContentSize().width);
    // log::info("TEST CREATING -> X: {}, Y: {}", leftX, rightX);

    node->setPositionX((leftX + rightX) / 2.f);
}

void OGDBUtils::centerNodeBetweenY(CCNode* node, CCNode* top, CCNode* bottom) {
    float topY = top->getPositionY() - top->getScaledContentSize().height / 2.f;
    float bottomY = bottom->getPositionY() + bottom->getScaledContentSize().height / 2.f;
    node->setPositionY((topY + bottomY) / 2.f);
}

void OGDBUtils::centerNodeBetweenXY(CCNode* node, CCNode* left, CCNode* right) {
    float leftX = left->getPositionX() + left->getScaledContentSize().width / 2.f;
    float rightX = right->getPositionX() - right->getScaledContentSize().width / 2.f;
    float centerX = (leftX + rightX) / 2.f;

    float leftY = left->getPositionY() + left->getScaledContentSize().height / 2.f;
    float rightY = right->getPositionY() - right->getScaledContentSize().height / 2.f;
    float centerY = (leftY + rightY) / 2.f;

    node->setPosition({ centerX, centerY });
}

void OGDBUtils::setSliderWidth(Slider* slider, float width) {
    if (!slider || !slider->m_groove || !slider->m_touchLogic) return;

    float originalWidth = slider->m_width;
    if (originalWidth <= 0.0f) return; 
    float scale = width / originalWidth;
    scale = std::max(scale, 0.05f);
	slider->setScaleX(scale);

	auto m_TouchSlider = slider->m_touchLogic->m_thumb;
	m_TouchSlider->setContentSize({(m_TouchSlider->getContentSize().width / scale), m_TouchSlider->getContentSize().height});
	auto m_tNodos = m_TouchSlider->getChildren();

	if (m_tNodos) {
		CCObject* obj;
		CCARRAY_FOREACH(m_tNodos, obj) {
			auto nodo = static_cast<CCNode*>(obj);
			float childScaleX = nodo->getScaleX();
			nodo->setScaleX(childScaleX / scale);
		}
	}
}

void OGDBUtils::CleanListLayer(CCLayer* layer, int targetWidth, int targetHeight, bool useFirstChild) {
	if (!layer) return;

	CCLayer* workingLayer = layer;
	if (useFirstChild) {
		auto firstChild = layer->getChildren()->objectAtIndex(0);
		if (!firstChild || !dynamic_cast<CCLayer*>(firstChild)) return;

		workingLayer = static_cast<CCLayer*>(firstChild);
	}

	CCObject* obj = nullptr;
	CCARRAY_FOREACH(workingLayer->getChildren(), obj) {
        CCNode* node = (CCNode*)obj;
		// auto node = static_cast<CCNode*>(obj);
		// if (!node) continue;

		CCSize size = node->getContentSize();
		if (size.width == targetWidth && size.height == targetHeight) {
			node->setContentSize({ 0, 100 });
			node->setZOrder(1);

			auto children = node->getChildren();
			if (children && children->count() >= 4) {
				for (int i = 0; i < 4; i++) {
                    auto sprite = (CCSprite*)children->objectAtIndex(i);
				    sprite->setVisible(false);
					// if (auto sprite = dynamic_cast<CCSprite*>(children->objectAtIndex(i))) {
					// 	sprite->setVisible(false);
					// }
				}
			}

			node->setZOrder(2);
		}
	}
}

// Return true if user is logged in GD
bool OGDBUtils::checkGDAccount(bool showAlert, int accountID) {
    if(accountID <= -9999) accountID = GJAccountManager::sharedState()->m_accountID;
    if (accountID <= 0 && showAlert == true) {
        FLAlertLayer::create(
            "Login Required",
            "<cr>You need to be logged in</c> <cy>to a Geometry Dash account</c> to access this feature.",
            "OK"
        )->show();
        return false;
    }
    return true;
}

bool OGDBUtils::checkOGDBAccount(int accountID, bool showAlert) {
    if (accountID <= 0 && showAlert == true) {
        FLAlertLayer::create(
            "Login Required",
            "<cr>You need to be logged in</c> <cy>to a ObeyGDTeam account</c> to access this feature.",
            "OK"
        )->show();
        return false;
    }
    return true;
}