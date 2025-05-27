#pragma once

#include <string>
#include <map>
#include <vector>
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class CustomBackgroundSprite {
private:
    const char* sprite_id;
    const char* sprite_name;
    const char* sprite_path;

    static inline std::map<std::string, CustomBackgroundSprite*> spriteRegistry;
    static inline std::vector<std::string> spriteOrderList;

public:
    CustomBackgroundSprite(const char* id);

    const char* getID() const;
    const char* getName() const;
    const char* getSprite() const;

    static CustomBackgroundSprite* registerSprite(
        const char* id,
        const char* name,
        const char* spritePath
    );

    static CustomBackgroundSprite* get(const char* id);
    static const std::map<std::string, CustomBackgroundSprite*>& getAllSprites();
    static const std::vector<std::string>& getOrderedKeys();

    static void init();
};
