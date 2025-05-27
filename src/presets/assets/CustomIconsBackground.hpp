#pragma once

#include <string>
#include <map>
#include <vector>
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class CustomIconsBackground {
protected:
    const char* subbg_id;
    const char* subbg_sprite;
    const char* subbg_name;
    CCRect subbg_rect;

    inline static std::map<std::string, CustomIconsBackground*> subbackgroundRegistry;
    inline static std::vector<std::string> subbackgroundOrderList;

    CustomIconsBackground(const char* id);

public:
    const char* getID() const;
    const char* getSprite() const;
    const CCRect& getRect() const;
    const char* getName() const;

    static CustomIconsBackground* registerSubbackground(
        const char* id,
        const char* sprite,
        const CCRect& rect,
        const char* name
    );

    static CustomIconsBackground* get(const char* id);
    static const std::map<std::string, CustomIconsBackground*>& getAllSubbackgrounds();
    static const std::vector<std::string>& getOrderedKeys();
    static void init();
};