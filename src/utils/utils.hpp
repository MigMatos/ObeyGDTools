#pragma once
#include <Geode/Geode.hpp>
#include <any>
#include <Geode/ui/LazySprite.hpp>
using namespace geode::prelude;


class OGDBUtils {
public:
	static cocos2d::ccColor4B ToColor4B(ccColor3B c, GLubyte transparentValue = 255);
	static cocos2d::ccColor4F ToColor4F(ccColor3B c, float transparentValue = 1.f);
	// 
	static bool isDarkColor(cocos2d::ccColor4B col);
	static bool isDarkColorCombination(cocos2d::ccColor4B col1, cocos2d::ccColor4B col2);
	//
	static void WIPFeature(CCObject*);

	static void autoScaleToFit(CCNode* node, float minSize, float maxSize);

	template<typename T>
	static T anyToRef(const std::any& value, bool forceFloat = false) {
		if constexpr (std::is_same_v<T, float>) {
			if (forceFloat) {
				if (value.type() == typeid(float))
					return std::any_cast<float>(value);
				if (value.type() == typeid(int))
					return static_cast<float>(std::any_cast<int>(value));
				if (value.type() == typeid(double))
					return static_cast<float>(std::any_cast<double>(value));
				throw std::bad_any_cast();
			}
		}
		return std::any_cast<T>(value);
	}

	static void centerNodeBetweenX(CCNode* node, CCNode* left, CCNode* right);
	static void centerNodeBetweenY(CCNode* node, CCNode* top, CCNode* bottom);
	static void centerNodeBetweenXY(CCNode* node, CCNode* left, CCNode* right);

	static void setSliderWidth(Slider* slider, float width);

	static void CleanListLayer(cocos2d::CCLayer* layer, int width, int height, bool useFirstChild = false);

	static bool checkGDAccount(bool showAlert = false, int accountID = -9999);

	static bool checkOGDBAccount(int accountID, bool showAlert = false);
};
