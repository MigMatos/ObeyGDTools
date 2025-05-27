#pragma once

#include <string>
#include <map>
#include <vector>
#include <Geode/Geode.hpp>

using namespace geode::prelude;


class CustomCommentsBackground {
protected:
    const char* subbg_id;
    const char* subbg_sprite;
    const char* subbg_name;
    CCRect subbg_rect;

    inline static std::map<std::string, CustomCommentsBackground*> subbackgroundRegistry;
    inline static std::vector<std::string> subbackgroundOrderList;

    CustomCommentsBackground(const char* id);

public:
    const char* getID() const;
    const char* getSprite() const;
    const CCRect& getRect() const;
    const char* getName() const;

    static CustomCommentsBackground* registerSubbackground(
        const char* id,
        const char* sprite,
        const CCRect& rect,
        const char* name
    );

    static CustomCommentsBackground* get(const char* id);
    static const std::map<std::string, CustomCommentsBackground*>& getAllSubbackgrounds();
    static const std::vector<std::string>& getOrderedKeys();
    static void init();
};