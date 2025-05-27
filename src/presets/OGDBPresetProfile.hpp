#pragma once

#include <string>
#include <map>
#include <vector>
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class OGDBPresetProfile {
public:
    enum class Type {
        Public,
        Hidden,
        Premium
    };

private:
    const char* preset_id;
    const char* preset_name;
    Type preset_type;

    const char* bg_asset;
    const char* bg_sub_asset;
    bool bg_animated;
    bool bg_blending;

    const char* bg_icon_asset;
    bool bg_icon_blending;

    const char* bg_comment_asset;
    bool bg_comment_blending;

    const char* bg_border_asset;

    cocos2d::ccColor4B color_bg;
    cocos2d::ccColor4B color_bg2;
    cocos2d::ccColor4B color_bg_sublayer;
    cocos2d::ccColor4B color_bg_border;
    cocos2d::ccColor4B color_bg_icons;
    cocos2d::ccColor4B color_bg_comments;

    static inline std::map<std::string, OGDBPresetProfile*> presetRegistry;
    static inline std::vector<std::string> presetOrderList;

public:
    OGDBPresetProfile(const char* id);

    const char* getID() const;
    const char* getName() const;
    Type getType() const;

    const char* getBGAsset() const;
    const char* getBGSubAsset() const;
    bool isBGAnimated() const;
    bool isBGBlending() const;

    const char* getBGIconAsset() const;
    bool isBGIconBlending() const;

    const char* getBGCommentAsset() const;
    bool isBGCommentBlending() const;

    const char* getBGBorderAsset() const;

    cocos2d::ccColor4B getColorBG() const;
    cocos2d::ccColor4B getColorBG2() const;
    cocos2d::ccColor4B getColorBGSublayer() const;
    cocos2d::ccColor4B getColorBGBorder() const;
    cocos2d::ccColor4B getColorBGIcons() const;
    cocos2d::ccColor4B getColorBGComments() const;

    static OGDBPresetProfile* registerPreset(
        const char* id,
        const char* name,
        Type type,
        const char* bgAsset,
        const char* bgSubAsset,
        bool bgAnimated,
        bool bgBlending,
        const char* bgIconAsset,
        bool bgIconBlending,
        const char* bgCommentAsset,
        bool bgCommentBlending,
        const char* bgBorderAsset,
        cocos2d::ccColor4B colorBG,
        cocos2d::ccColor4B colorBG2,
        cocos2d::ccColor4B colorBGSublayer,
        cocos2d::ccColor4B colorBGBorder,
        cocos2d::ccColor4B colorBGIcons,
        cocos2d::ccColor4B colorBGComments
    );

    static OGDBPresetProfile* get(const char* id);
    static const std::map<std::string, OGDBPresetProfile*>& getAllPresets();
    static const std::vector<std::string>& getOrderedKeys();
    static void init();
};