#include "CustomIconsBackground.hpp"

CustomIconsBackground::CustomIconsBackground(const char* id)
    : subbg_id(id), subbg_sprite(""), subbg_rect(CCRectZero), subbg_name("") {}

const char* CustomIconsBackground::getID() const { return subbg_id; }
const char* CustomIconsBackground::getSprite() const { return subbg_sprite; }
const CCRect& CustomIconsBackground::getRect() const { return subbg_rect; }
const char* CustomIconsBackground::getName() const { return subbg_name; }

CustomIconsBackground* CustomIconsBackground::registerSubbackground(
    const char* id,
    const char* sprite,
    const CCRect& rect,
    const char* name
) {
    if (subbackgroundRegistry.count(id)) return nullptr;

    auto* bg = new CustomIconsBackground(id);
    bg->subbg_sprite = sprite;
    bg->subbg_rect = rect;
    bg->subbg_name = name;

    subbackgroundRegistry[id] = bg;
    subbackgroundOrderList.push_back(id);
    return bg;
}

CustomIconsBackground* CustomIconsBackground::get(const char* id) {
    auto it = subbackgroundRegistry.find(id);
    return it != subbackgroundRegistry.end() ? it->second : nullptr;
}

const std::map<std::string, CustomIconsBackground*>& CustomIconsBackground::getAllSubbackgrounds() {
    return subbackgroundRegistry;
}

const std::vector<std::string>& CustomIconsBackground::getOrderedKeys() {
    return subbackgroundOrderList;
}

void CustomIconsBackground::init() {
    // Special
    registerSubbackground("special_none", "transparent.png"_spr, CCRectMake(0, 0, 0, 0), "None");

    registerSubbackground("sub_c9_light_02", "light_b_02.png"_spr, CCRectMake(0, 0, 0, 0), "Light");
    registerSubbackground("sub_c9_light_01", "light_b_00.png"_spr, CCRectMake(0, 0, 0, 0), "Border Light");
    registerSubbackground("sub_c9_light_00", "light_b_01.png"_spr, CCRectMake(0, 0, 0, 0), "Partially light");

    registerSubbackground("sub_c9_dark_02", "dark_b_02.png"_spr, CCRectMake(0, 0, 0, 0), "Dark");
    registerSubbackground("sub_c9_dark_01", "dark_b_00.png"_spr, CCRectMake(0, 0, 0, 0), "Border Dark");
    registerSubbackground("sub_c9_dark_00", "dark_b_01.png"_spr, CCRectMake(0, 0, 0, 0), "Partially Dark");

    registerSubbackground("sub_c9_darkstar_00", "darkstar_b_00.png"_spr, CCRectMake(0, 0, 0, 0), "DarkStar");
    // registerSubbackground("sub_c9_rainbow_25", "rainbow_b_00.png"_spr, CCRectMake(0, 0, 0, 0), "Rainbow");
    registerSubbackground("sub_c9_murasakiskies_25", "murasakiskies_b_00.png"_spr, CCRectMake(0, 0, 0, 0), "Murasaki Skies");
}
