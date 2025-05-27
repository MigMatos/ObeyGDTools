#include "CustomBorderSprite.hpp"

CustomBorderSprite::CustomBorderSprite(const char* id)
    : border_id(id), border_sprite(""), border_rect(CCRectZero), border_name("") {}

const char* CustomBorderSprite::getID() const { return border_id; }
const char* CustomBorderSprite::getSprite() const { return border_sprite; }
const CCRect& CustomBorderSprite::getRect() const { return border_rect; }
const char* CustomBorderSprite::getName() const { return border_name; }

CustomBorderSprite* CustomBorderSprite::registerBorder(
    const char* id,
    const char* sprite,
    const CCRect& rect,
    const char* name
) {
    if (borderRegistry.count(id)) return nullptr;

    auto* border = new CustomBorderSprite(id);
    border->border_sprite = sprite;
    border->border_rect = rect;
    border->border_name = name;

    borderRegistry[id] = border;
    borderOrderList.push_back(id);
    return border;
}

CustomBorderSprite* CustomBorderSprite::get(const char* id) {
    auto it = borderRegistry.find(id);
    return it != borderRegistry.end() ? it->second : nullptr;
}

const std::map<std::string, CustomBorderSprite*>& CustomBorderSprite::getAllBorders() {
    return borderRegistry;
}

const std::vector<std::string>& CustomBorderSprite::getOrderedKeys() {
    return borderOrderList;
}

void CustomBorderSprite::init() {
    // Special
    registerBorder("special_none", "transparent.png"_spr, CCRectMake(0, 0, 0, 0), "None");
    // Extra
    registerBorder("border_c9_rounded_00", "border_b_00.png"_spr, CCRectMake(0, 0, 0, 0), "Rounded");
    registerBorder("border_c9_squared_00", "border_b_01.png"_spr, CCRectMake(0, 0, 0, 0), "Square");
}
