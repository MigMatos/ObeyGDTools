#pragma once

#include <string>
#include <map>
#include <vector>
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class CustomBorderSprite {
private:
    const char* border_id;
    const char* border_sprite;
    CCRect border_rect;
    const char* border_name;

    static inline std::map<std::string, CustomBorderSprite*> borderRegistry;
    static inline std::vector<std::string> borderOrderList;

public:
    CustomBorderSprite(const char* id);

    const char* getID() const;
    const char* getSprite() const;
    const CCRect& getRect() const;
    const char* getName() const;

    static CustomBorderSprite* registerBorder(
        const char* id,
        const char* sprite,
        const CCRect& rect,
        const char* name
    );

    static CustomBorderSprite* get(const char* id);
    static const std::map<std::string, CustomBorderSprite*>& getAllBorders();
    static const std::vector<std::string>& getOrderedKeys();
    static void init();
};
