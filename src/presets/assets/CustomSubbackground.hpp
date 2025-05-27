#pragma once

#include <string>
#include <map>
#include <vector>
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class CustomSubbackground {
private:
    const char* subbg_id;
    const char* subbg_sprite;
    CCRect subbg_rect;
    const char* subbg_name;

    static inline std::map<std::string, CustomSubbackground*> subbackgroundRegistry;
    static inline std::vector<std::string> subbackgroundOrderList;

public:
    CustomSubbackground(const char* id);

    const char* getID() const;
    const char* getSprite() const;
    const CCRect& getRect() const;
    const char* getName() const;

    static CustomSubbackground* registerSubbackground(
        const char* id,
        const char* sprite,
        const CCRect& rect,
        const char* name
    );

    static CustomSubbackground* get(const char* id);
    static const std::map<std::string, CustomSubbackground*>& getAllSubbackgrounds();
    static const std::vector<std::string>& getOrderedKeys();
    static void init();
};
