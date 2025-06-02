#include "CustomBackgroundSprite.hpp"

CustomBackgroundSprite::CustomBackgroundSprite(const char* id)
    : sprite_id(id), sprite_name(""), sprite_path("") {}

const char* CustomBackgroundSprite::getID() const {
    return sprite_id;
}

const char* CustomBackgroundSprite::getName() const {
    return sprite_name;
}

const char* CustomBackgroundSprite::getSprite() const {
    return sprite_path;
}

CustomBackgroundSprite* CustomBackgroundSprite::registerSprite(
    const char* id,
    const char* name,
    const char* spritePath
) {
    if (spriteRegistry.count(id)) return nullptr;

    auto* sprite = new CustomBackgroundSprite(id);
    sprite->sprite_name = name;
    sprite->sprite_path = spritePath;

    spriteRegistry[id] = sprite;
    spriteOrderList.push_back(id);
    return sprite;
}

CustomBackgroundSprite* CustomBackgroundSprite::get(const char* id) {
    auto it = spriteRegistry.find(id);
    return it != spriteRegistry.end() ? it->second : nullptr;
}

const std::map<std::string, CustomBackgroundSprite*>& CustomBackgroundSprite::getAllSprites() {
    return spriteRegistry;
}

const std::vector<std::string>& CustomBackgroundSprite::getOrderedKeys() {
    return spriteOrderList;
}

void CustomBackgroundSprite::init() {
    // Special typos
    registerSprite("special_none", "None", "transparent.png"_spr);
    registerSprite("special_simple", "Simple background colored", "transparent.png"_spr);
    registerSprite("special_gradient", "Gradient background colored", "transparent.png"_spr);
    registerSprite("c9_default_00", "Default background", "default_square_00.png"_spr);
    registerSprite("event_background_00", "Event background", "transparent.png"_spr);
    // Extra
    registerSprite("bg_darkstar_00", "DarkStar", "darkstar_bg_00.png"_spr);

    registerSprite("bg_murasakiskies_00", "Murasaki Skies", "murasakiskies_bg_00.png"_spr);
    registerSprite("bg_luminightglade_00", "Luminight Glade", "luminightglade_bg_00.png"_spr);

    // registerSprite("bg_halloween_25", "Halloween (2025)", "halloween_bg_25.png"_spr);
    registerSprite("bg_rainbow_25", "Rainbow (2025)", "rainbow_bg_25.png"_spr);
}
