#include "OGDBPresetProfile.hpp"

using namespace cocos2d;

OGDBPresetProfile::OGDBPresetProfile(const char* id)
    : preset_id(id), preset_name(""),
      preset_type(Type::Public),
      bg_asset(""), bg_sub_asset(""), bg_animated(false), bg_blending(false),
      bg_icon_asset(""), bg_icon_blending(false),
      bg_comment_asset(""), bg_comment_blending(false),
      bg_border_asset(""),
      color_bg({0, 0, 0, 255}),
      color_bg2({0, 0, 0, 255}),
      color_bg_sublayer({0, 0, 0, 255}),
      color_bg_border({0, 0, 0, 255}),
      color_bg_icons({0, 0, 0, 255}),
      color_bg_comments({0, 0, 0, 255}) {}

const char* OGDBPresetProfile::getID() const { return preset_id; }
const char* OGDBPresetProfile::getName() const { return preset_name; }
OGDBPresetProfile::Type OGDBPresetProfile::getType() const { return preset_type; }

const char* OGDBPresetProfile::getBGAsset() const { return bg_asset; }
const char* OGDBPresetProfile::getBGSubAsset() const { return bg_sub_asset; }
bool OGDBPresetProfile::isBGAnimated() const { return bg_animated; }
bool OGDBPresetProfile::isBGBlending() const { return bg_blending; }

const char* OGDBPresetProfile::getBGIconAsset() const { return bg_icon_asset; }
bool OGDBPresetProfile::isBGIconBlending() const { return bg_icon_blending; }

const char* OGDBPresetProfile::getBGCommentAsset() const { return bg_comment_asset; }
bool OGDBPresetProfile::isBGCommentBlending() const { return bg_comment_blending; }

const char* OGDBPresetProfile::getBGBorderAsset() const { return bg_border_asset; }

ccColor4B OGDBPresetProfile::getColorBG() const { return color_bg; }
ccColor4B OGDBPresetProfile::getColorBG2() const { return color_bg2; }
ccColor4B OGDBPresetProfile::getColorBGSublayer() const { return color_bg_sublayer; }
ccColor4B OGDBPresetProfile::getColorBGBorder() const { return color_bg_border; }
ccColor4B OGDBPresetProfile::getColorBGIcons() const { return color_bg_icons; }
ccColor4B OGDBPresetProfile::getColorBGComments() const { return color_bg_comments; }

OGDBPresetProfile* OGDBPresetProfile::registerPreset(
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
    ccColor4B colorBG,
    ccColor4B colorBG2,
    ccColor4B colorBGSublayer,
    ccColor4B colorBGBorder,
    ccColor4B colorBGIcons,
    ccColor4B colorBGComments
) {
    if (presetRegistry.count(id)) return nullptr;

    auto* preset = new OGDBPresetProfile(id);
    preset->preset_name = name;
    preset->preset_type = type;

    preset->bg_asset = bgAsset;
    preset->bg_sub_asset = bgSubAsset;
    preset->bg_animated = bgAnimated;
    preset->bg_blending = bgBlending;

    preset->bg_icon_asset = bgIconAsset;
    preset->bg_icon_blending = bgIconBlending;

    preset->bg_comment_asset = bgCommentAsset;
    preset->bg_comment_blending = bgCommentBlending;

    preset->bg_border_asset = bgBorderAsset;

    preset->color_bg = colorBG;
    preset->color_bg2 = colorBG2;
    preset->color_bg_sublayer = colorBGSublayer;
    preset->color_bg_border = colorBGBorder;
    preset->color_bg_icons = colorBGIcons;
    preset->color_bg_comments = colorBGComments;

    presetRegistry[id] = preset;
    presetOrderList.push_back(id);
    return preset;
}

OGDBPresetProfile* OGDBPresetProfile::get(const char* id) {
    auto it = presetRegistry.find(id);
    return it != presetRegistry.end() ? it->second : nullptr;
}

const std::map<std::string, OGDBPresetProfile*>& OGDBPresetProfile::getAllPresets() {
    return presetRegistry;
}

const std::vector<std::string>& OGDBPresetProfile::getOrderedKeys() {
    return presetOrderList;
}

void OGDBPresetProfile::init() {

    registerPreset("darkstar", "DarkStar", Type::Public,
        "darkstar_bg_00.png"_spr , "dark_b_03.png"_spr ,
        false, false,
        "darkstar_b_00.png"_spr , false,
        "darkstar_b_00.png"_spr , false,
        "border_00.png"_spr,
        {20, 20, 20, 255},
        {40, 40, 40, 255},
        {10, 10, 10, 255},
        {60, 60, 60, 255},
        {200, 200, 200, 255},
        {150, 150, 150, 255}
    );
    
}
