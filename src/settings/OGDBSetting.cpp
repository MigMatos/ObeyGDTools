#pragma once

#include "OGDBSetting.hpp"

OGDBSetting::OGDBSetting(const char* id)
    : setting_id(id) {}

const char* OGDBSetting::getID() const {
    return setting_id;
}

const char* OGDBSetting::getName() const {
    return setting_name;
}

const char* OGDBSetting::getDescription() const {
    return setting_desc;
}

const char* OGDBSetting::getRequiredText() const {
    return setting_required_text;
}

bool OGDBSetting::getVisible() const {
    return setting_is_visible;
}

SettingEnum::Type OGDBSetting::getType() const {
    return setting_type;
}

std::any OGDBSetting::getDefaultValue() const {
    return setting_default_value;
}

std::any OGDBSetting::getDefaultOptions() const {
    return setting_default_options;
}

void OGDBSetting::setName(const char* name) {
    setting_name = name;
}

void OGDBSetting::setDescription(const char* desc) {
    setting_desc = desc;
}

void OGDBSetting::setRequiredText(const char* setting_required_txt) {
    setting_required_text = setting_required_txt;
}

void OGDBSetting::setType(SettingEnum::Type type) {
    setting_type = type;
}

void OGDBSetting::setVisible(bool isVisible) {
    setting_is_visible = isVisible;
}

template<typename T>
OGDBSetting* OGDBSetting::registerSetting(
    const char* id, 
    const char* name, 
    const char* requiredtxt,
    const char* desc,
    T defaultValue,
    const std::any defaultOptions,
    SettingEnum::Type type,
    bool isHidden
) {
    if (settingsRegistry.count(id)) return nullptr;

    auto* setting = new OGDBSetting(id);
    setting->setting_name = name;
    setting->setting_desc = desc;
    setting->setting_required_text = requiredtxt ? requiredtxt : "";
    setting->setting_default_value = defaultValue;
    setting->setting_default_options = defaultOptions;
    setting->setting_type = type;
    // new in 1.0 version
    setting->setting_is_visible = isHidden;
    settingsRegistry[id] = setting; // Map without order
    settingsKeyOrdenList.push_back(id); // Order
    return setting;
}

OGDBSetting* OGDBSetting::get(const char* id) {
    auto it = settingsRegistry.find(id);
    return it != settingsRegistry.end() ? it->second : nullptr;
}

const std::map<std::string, OGDBSetting*>& OGDBSetting::getAllSettings() {
    return settingsRegistry;
}

const std::vector<std::string>& OGDBSetting::getOrderKeySettings() {
    return settingsKeyOrdenList;
}

void OGDBSetting::init() {
    // GENERAL

    registerSetting("GENERAL_SHOWEXPERIMENTALOPTIONS", "Show experiments",
        nullptr,
        "<cg>Displays experimental options.</c>\nThese options may <cr>contain bugs</c> or be <cr>removed in the future.</c>\n<cy>Disabling this option will disable all experiments.</c>",
        true,
        nullptr
    );

    registerSetting("GENERAL_TITLE_1", "- Web options -",
        nullptr,"",0,nullptr,
        SettingEnum::Type::Title
    ); // ---- Title ----

    registerSetting("GENERAL_CACHEMODE", "Cache Mode",
        nullptr,
        "<cg>Select a mode</c> that <cy>globally affects profile customization</c>.\n\n"
        "- <cp>Disabled</c>: Profile customization is completely turned off.\n"
        "- <cp>Local</c>: Customization applies locally to all users.\n"
        "- <cp>Online</c>: Customization is fetched from ObeyGDTeam servers.\n",
        1,
        std::any(std::vector<std::string>{"Persistent", "Temporal"}),
        SettingEnum::Type::Options
    );

    registerSetting("GENERAL_CACHETIMEEXPIRE", "Expire Time (Minutes)",
        nullptr,
        "<cg>Set the time limit</c> (in minutes) for each cached item.\n"
        "Use with <cy>Persistent</c> mode for best results.",
        30.2f,
        std::any(std::vector<std::any>{0, 1440, 1.f, std::string(" M.")}),
        SettingEnum::Type::Slider
    );

    registerSetting("GENERAL_CACHESIZEMBLIMIT", "Cache Size (MB)",
        nullptr,
        "<cg>Set the max cache size</c> in megabytes.\n"
        "Set to <cy>0</c> to completely disable caching.\n"
        "<cr>Not recommended set 0 MB with Persistent mode</c> as it may cause instability or mod data corruption.",
        16,
        std::any(std::vector<std::any>{0, 256, 0 , std::string(" MB")}),
        SettingEnum::Type::Slider
    );

    // PROFILES

    registerSetting("PROFILES_TITLE_0", "- General options -",
        nullptr,"",0,nullptr,
        SettingEnum::Type::Title
    ); // ---- Title ----

    registerSetting("PROFILES_MODE", "Profiles customization mode",
        nullptr,
        "<cg>Select a mode</c> that <cy>globally effects profiles</c>.\n\n"
        "- <cp>Disabled</c>: Profile customization is completely disabled\n"
        "- <cp>Local</c>: Profile customization affects all users\n"
        "- <cp>Online</c>: Profile customization comes from ObeyGDTeam servers\n",
        2,
        std::any(std::vector<std::string>{"Disabled", "Local", "Online"}),
        SettingEnum::Type::Options
    );

    registerSetting("PROFILES_TITLE_1", "- Global profile options -",
        nullptr,"",0,nullptr,
        SettingEnum::Type::Title
    );

    registerSetting("PROFILES_ICONFIXPOSITION", "Fix icons position",
        nullptr,
        "<cg>Fixes the position of the icons</c> in the <cy>profile popup.</c>.\n"
        "This option might <cr>not work correctly</c> with <cp>mods</c> that <cy>modify the icon layer</c>.",
        true,
        nullptr
    );

    registerSetting("PROFILES_COMMENTSENABLESCROLLBAR", "Enable Scrollbar in profile comments",
        nullptr,
        "<cg>Displays a scrollbar</c> for comments in the <cy>profile popup</c>.",
        true,
        nullptr
    );

    registerSetting("PROFILES_TITLE_2", "- Your profile options -",
        nullptr,"",0,nullptr,
        SettingEnum::Type::Title
    ); // ---- Title ----

    registerSetting("PROFILES_TITLE_2_1", "- Online customization -",
        nullptr,"",0,nullptr,
        SettingEnum::Type::Title
    ); // ---- Title ----

    registerSetting("PROFILES_ENABLE_ADV_CUSTOM", "Enable advanced customization",
        "",
        "<cg>Enables</c> advanced customization in <cj>profiles</c>.\n\n"
        "- If enabled, it uses <cy>online/advanced profile settings</c>.\n"
        "- If disabled, it will use <cg>basic settings</c> instead.",
        true,
        nullptr
    );

    registerSetting("PROFILES_BACKGROUNDCUSTOM_BUTTON", "Customize background profile",
        "Only works if advanced customization is enabled",
        "",
        0,
        nullptr,
        SettingEnum::Type::Setting_Profile_Custom
    );

    registerSetting("PROFILES_TITLE_2_2", "- Basic/Offline customization -",
        nullptr,"",0,nullptr,
        SettingEnum::Type::Title
    ); // ---- Title ----

    // - BG
    registerSetting("PROFILES_TITLE_POPUP", "Pop-up options",
        nullptr,"",0,nullptr,
        SettingEnum::Type::Title
    ); // ---- Title ----

    registerSetting("PROFILES_BACKGROUNDTYPE", "Background type",
        nullptr,
        "<cg>Select the overall background style</c> for the profile popup.\n\n"
        "- <cp>None</c>: remove the background.\n"
        "- <cp>Simple</c>: solid single color.\n"
        "- <cp>Gradient</c>: two-color gradient.\n",
        // "- <cp>Default</c>: default brown background.\n"
        // "- <cp>Event</c>: Returns a event background, "
        // "if <cr>any event is notactive</c> it returns <cy>Gradient background</c> for default.\n"
        // "- <cp>Custom</c>: Returns a custom background image, "
        // "if there is <cr>no current active event</c> it returns the current <cy>event background</c> for default.\n",
        2,
        std::any(std::vector<std::string>{"None", "Simple", "Gradient"}),
        SettingEnum::Type::Options
    );
    
    registerSetting("PROFILES_SWAPBACKGROUNDCOLOR", "Invert background color",
        "Only works if the background type is 'Simple' & 'Gradient'",
        "<cg>Swaps primary</c> with <cj>secondary</c> color of the <cy>background profile popup</c>.",
        true,
        nullptr
    );

    registerSetting("PROFILES_ANIMATEBACKGROUND", "Animate background",
        "Only works if the background type is 'Gradient'",
        "<cg>Enables animation</c> for the <cy>popup background color</c>.",
        true,
        nullptr
    );

    registerSetting("PROFILES_BACKGROUNDSUBLAYERTYPE", "Sublayer type",
        nullptr,
        "<cg>Set a background sublayer type</c> that will be <cy>displayed in the profile popup</c>.\n\n"
        "- <cp>Auto</c> option automatically determines the sublayer type based on the <cy>background colors</c>.",
        1,
        std::any(std::vector<std::string>{"None", "Auto", "Dark", "Light"}),
        SettingEnum::Type::Options
    );


    // - BORDERS
    registerSetting("PROFILES_TITLE_BORDERS", "Border options",
        nullptr,"",0,nullptr,
        SettingEnum::Type::Title
    ); // ---- Title ----

    registerSetting("PROFILES_BORDERTYPE", "Border type",
        nullptr,
        "<cg>Set a border type</c> that will be <cy>displayed in the profile popup</c>.",
        1,
        std::any(std::vector<std::string>{"None", "Rounded", "Square"}),
        SettingEnum::Type::Options
    );

    registerSetting("PROFILES_ENABLECOLORBORDERS", "Enable custom border color",
        nullptr,
        "<cg>Enable custom color</c> to the <cy>borders of the profile popup</c>.",
        true,
        nullptr
    );

    // - Icons
    registerSetting("PROFILES_TITLE_ICONS", "Icons options",
        nullptr,"",0,nullptr,
        SettingEnum::Type::Title
    ); // ---- Title ----

    registerSetting("PROFILES_ICONSBACKGROUNDTYPE", "Profile icons background type",
        nullptr,
        "<cg>Set a icons background type</c> that will be <cy>displayed in the profile popup</c>.",
        2,
        std::any(std::vector<std::string>{"None", "Default", "Auto", "Dark", "Light"}),
        SettingEnum::Type::Options
    );

    // - Comments
    registerSetting("PROFILES_TITLE_COMMENTS", "Comments options",
        nullptr,"",0,nullptr,
        SettingEnum::Type::Title
    ); // ---- Title ----

    registerSetting("PROFILES_COMMENTSBACKGROUNDTYPE", "Profile comment background type",
        nullptr,
        "<cg>Select the background type</c> for the <cy>profile comment section</c>.",
        2,
        std::any(std::vector<std::string>{"None", "Default", "Auto", "Dark", "Light"}),
        SettingEnum::Type::Options
    );

    // - External profiles
    registerSetting("PROFILES_TITLE_EXTERNALUSERS", "- External profile options -",
        nullptr,"",0,nullptr,
        SettingEnum::Type::Title
    );

    registerSetting("PROFILES_DISABLEEXTERNALBACKGROUNDS", "Disable background",
        "Only works if profile mode is 'Online'",
        "<cg>Disable</c> <cy>external user backgrounds</c>.\n\n"
        "It will return the default background you selected for unlinked users.",
        false,
        nullptr
    );

    registerSetting("PROFILES_EXTERNALBACKGROUNDTYPE", "Default background",
        "Only works if profile mode is 'Online'",
        "<cg>Select the overall background style</c> for external users in the profile popup.\n\n"
        "- <cp>None</c>: remove the background.\n"
        "- <cp>Default</c>: default brown background.\n"
        "- <cp>Simple</c>: solid single color.\n"
        "- <cp>Gradient</c>: two-color gradient.\n"
        "- <cp>Event</c>: Returns a event background, "
        "if <cr>any event is notactive</c> it returns <cy>Gradient background</c> for default.\n"
        "- <cp>Custom</c>: Returns a custom background image, "
        "if there is <cr>no current active event</c> it returns the current <cy>event background</c> for default.\n",
        4,
        std::any(std::vector<std::string>{"None", "Simple", "Gradient", "Default", "Custom"}),
        SettingEnum::Type::Options
    );

    // - Hidden
    registerSetting("PROFILES_TITLE_HIDDEN_OPTIONS", "- Hidden Options -",
        nullptr,"",0,nullptr,
        SettingEnum::Type::Title,
        true
    );

    // Hidden
    registerSetting("PROFILE_ENABLE_BG_CUSTOM_IMAGE", "Enable Custom Background",
        "?",
        "<cg>-</c> <cy>-</c>.\n\n"
        "-",
        false,
        nullptr
    );

    registerSetting("PROFILE_BACKGROUND_COLOR", "Main Background Color",
        "", // hidden description for tooltip title
        "<cg>Sets</c> the <cy>main background color</c> of the profile.\n\n"
        "This is used when external backgrounds are disabled or not available.",
        ccColor4B({0, 0, 0, 190}),
        nullptr,
        SettingEnum::Type::Color4B,
        true
    );

    registerSetting("PROFILE_BACKGROUND_COLOR_2", "Secondary Background Color",
        "",
        "<cg>Sets</c> the <cy>secondary color</c> used for gradients or layered profile backgrounds.",
        ccColor4B({0, 0, 0, 190}),
        nullptr,
        SettingEnum::Type::Color4B,
        true
    );

    registerSetting("PROFILE_BACKGROUND_SUBLAYER_COLOR", "Sublayer Color",
        "",
        "<cg>Sets</c> the <cy>sublayer color</c>.",
        ccColor4B({0, 0, 0, 190}),
        nullptr,
        SettingEnum::Type::Color4B,
        true
    );

    registerSetting("PROFILE_BACKGROUND_BORDER_COLOR", "Border Color",
        "",
        "<cg>Defines</c> the <cy>color of borders</c> around the profile background.",
        ccColor4B({0, 0, 0, 190}),
        nullptr,
        SettingEnum::Type::Color4B,
        true
    );

    registerSetting("PROFILE_BACKGROUND_ICONS_COLOR", "Icon Background Color",
        "",
        "<cg>Sets</c> a <cy>color background</c> applied to profile icons (user, stars, etc.).",
        ccColor4B({0, 0, 0, 190}),
        nullptr,
        SettingEnum::Type::Color4B,
        true
    );

    registerSetting("PROFILE_BACKGROUND_COMMENTS_COLOR", "Comment Background Color",
        "",
        "<cg>Sets</c> the <cy>background color</c> for comment background in profile view.",
        ccColor4B({0, 0, 0, 190}),
        nullptr,
        SettingEnum::Type::Color4B,
        true
    );

    // ------------------------------------ //

    registerSetting("PROFILE_BACKGROUND_CUSTOM_SPRITE", "Custom Background Sprite",
        nullptr,
        "<cg>Sets</c> the <cy>image path</c> for the custom background.",
        std::string("transparent.png"_spr),
        nullptr,
        SettingEnum::Type::Text,
        true
    );

    registerSetting("PROFILE_BACKGROUND_CUSTOM_SPRITE_ID", "Custom Background Sprite ID",
        nullptr,
        "<cg>Sets</c> the <cy>identifier</c> used for the custom background.",
        std::string("special_gradient"),
        nullptr,
        SettingEnum::Type::Text,
        true
    );

    registerSetting("PROFILE_BACKGROUND_CUSTOM_ANIMATE_ID", "Custom Background Animation",
        nullptr,
        "<cg>Selects</c> the animation type for the background image.",
        0,
        std::any(std::vector<std::string>{"None", "Simple", "Left infinite"}),
        SettingEnum::Type::Options,
        true
    );
    //
    registerSetting("PROFILE_BORDER_CUSTOM_SPRITE", "Custom Border Sprite",
        nullptr,
        "<cg>Sets</c> the <cy>image path</c> for the border.",
        std::string("border_b_00.png"_spr),
        nullptr,
        SettingEnum::Type::Text,
        true
    );

    registerSetting("PROFILE_BORDER_CUSTOM_SPRITE_ID", "Custom Border Sprite ID",
        nullptr,
        "<cg>Sets</c> the <cy>identifier</c> used for the border.",
        std::string("border_c9_rounded_00"),
        nullptr,
        SettingEnum::Type::Text,
        true
    );

    registerSetting("PROFILE_BORDER_CUSTOM_ANIMATE_ID", "Custom Border Animation",
        nullptr,
        "<cg>Selects</c> the animation type for the border image.",
        0,
        std::any(std::vector<std::string>{"None", "Simple", "Left infinite"}),
        SettingEnum::Type::Options,
        true
    );
    //
    registerSetting("PROFILE_BACKGROUND_SUBLAYER_CUSTOM_SPRITE", "Custom Sublayer Sprite",
        nullptr,
        "<cg>Sets</c> the <cy>path</c> for the background sublayer.",
        std::string("dark_b_02.png"_spr),
        nullptr,
        SettingEnum::Type::Text,
        true
    );

    registerSetting("PROFILE_BACKGROUND_SUBLAYER_CUSTOM_SPRITE_ID", "Custom Sublayer Sprite ID",
        nullptr,
        "<cg>Sets</c> the <cy>identifier</c> for the background sublayer.",
        std::string("sub_c9_dark_02"),
        nullptr,
        SettingEnum::Type::Text,
        true
    );

    registerSetting("PROFILE_BACKGROUND_SUBLAYER_CUSTOM_ANIMATE_ID", "Sublayer Animation",
        nullptr,
        "<cg>Selects</c> the animation type for the sublayer image.",
        0,
        std::any(std::vector<std::string>{"None", "Simple", "Left infinite"}),
        SettingEnum::Type::Options,
        true
    );
    //
    registerSetting("PROFILE_BACKGROUND_ICON_CUSTOM_SPRITE", "Icon Background Sprite",
        nullptr,
        "<cg>Sets</c> the <cy>image path</c> for the icon background.",
        std::string("dark_b_01.png"_spr),
        nullptr,
        SettingEnum::Type::Text,
        true
    );

    registerSetting("PROFILE_BACKGROUND_ICON_CUSTOM_SPRITE_ID", "Icon Sprite ID",
        nullptr,
        "<cg>Sets</c> the <cy>identifier</c> used for the icon sprite.",
        std::string("sub_c9_dark_01"),
        nullptr,
        SettingEnum::Type::Text,
        true
    );

    registerSetting("PROFILE_BACKGROUND_ICON_CUSTOM_ANIMATE_ID", "Icon Background Animation",
        nullptr,
        "<cg>Selects</c> the animation type for the icon background.",
        0,
        std::any(std::vector<std::string>{"None", "Simple", "Left infinite"}),
        SettingEnum::Type::Options,
        true
    );
    //
    registerSetting("PROFILE_BACKGROUND_COMMENT_CUSTOM_SPRITE", "Comment Background Sprite",
        nullptr,
        "<cg>Sets</c> the <cy>image path</c> for the comment background.",
        std::string("dark_b_01.png"_spr),
        nullptr,
        SettingEnum::Type::Text,
        true
    );

    registerSetting("PROFILE_BACKGROUND_COMMENT_CUSTOM_SPRITE_ID", "Comment Sprite ID",
        nullptr,
        "<cg>Sets</c> the <cy>identifier</c> used for the comment background.",
        std::string("sub_c9_dark_01"),
        nullptr,
        SettingEnum::Type::Text,
        true
    );

    registerSetting("PROFILE_BACKGROUND_COMMENT_CUSTOM_ANIMATE_ID", "Comment Background Animation",
        nullptr,
        "<cg>Selects</c> the animation type for the comment background.",
        0,
        std::any(std::vector<std::string>{"None", "Simple", "Left infinite"}),
        SettingEnum::Type::Options,
        true
    );

    // --------------------------------------------- //

    registerSetting("PROFILE_BACKGROUND_COLOR_PERSISTENT", "Persistent Background Color",
        nullptr,
        "<cg>Defines</c> the background color mode for the profile popup background.",
        0,
        std::any(std::vector<std::string>{"None", "Color 1", "Color 2", "Color Glow"}),
        SettingEnum::Type::Options,
        true
    );

    registerSetting("PROFILE_BACKGROUND_COLOR_2_PERSISTENT", "Persistent Background Gradient Color",
        nullptr,
        "<cg>Defines</c> the gradient secondary color mode for the profile popup background.",
        0,
        std::any(std::vector<std::string>{"None", "Color 1", "Color 2", "Color Glow"}),
        SettingEnum::Type::Options,
        true
    );

    registerSetting("PROFILE_BACKGROUND_SUBLAYER_COLOR_PERSISTENT", "Persistent Sublayer Color",
        nullptr,
        "<cg>Defines</c> the color mode for the sublayer of the background.",
        0,
        std::any(std::vector<std::string>{"None", "Color 1", "Color 2", "Color Glow"}),
        SettingEnum::Type::Options,
        true
    );

    registerSetting("PROFILE_BACKGROUND_BORDER_COLOR_PERSISTENT", "Persistent Border Color",
        nullptr,
        "<cg>Defines</c> the border color mode for the profile background (may be redundant).",
        0,
        std::any(std::vector<std::string>{"None", "Color 1", "Color 2", "Color Glow"}),
        SettingEnum::Type::Options,
        true
    );

    registerSetting("PROFILE_BACKGROUND_ICONS_COLOR_PERSISTENT", "Persistent Icons Color",
        nullptr,
        "<cg>Defines</c> the color mode for icon backgrounds in the profile popup.",
        0,
        std::any(std::vector<std::string>{"None", "Color 1", "Color 2", "Color Glow"}),
        SettingEnum::Type::Options,
        true
    );

    registerSetting("PROFILE_BACKGROUND_COMMENTS_COLOR_PERSISTENT", "Persistent Comments Color",
        nullptr,
        "<cg>Defines</c> the color mode for comment backgrounds in the profile popup.",
        0,
        std::any(std::vector<std::string>{"None", "Color 1", "Color 2", "Color Glow"}),
        SettingEnum::Type::Options,
        true
    );




    // Extra options
    // ok no more settings in this version

    // Toggle background for comments history
    // registerSetting("PROFILES_HISTORY_SHOWCOMMENTBACKGROUND", "Enable background in comment history",
    //     "<cg>Displays a background layer</c> behind the <cy>comment history in the profile popup</c>.",
    //     true,
    //     nullptr
    // );

    // Background type for comments history
    // registerSetting("PROFILES_HISTORY_COMMENTBACKGROUNDTYPE", "History comment background type",
    //     "<cg>Select the background type</c> for the <cy>comment history section</c>.",
    //     2, // Default: "Dark"
    //     std::any(std::vector<std::string>{"None", "Dark", "Light", "Custom"}),
    //     SettingEnum::Type::Options
    // );

}


// Template instanciations

// getValue
template bool OGDBSetting::getValue<bool>() const;
template int OGDBSetting::getValue<int>() const;
template float OGDBSetting::getValue<float>() const;
template double OGDBSetting::getValue<double>() const;
template std::string OGDBSetting::getValue<std::string>() const;


// 
// En OGDBSetting.cpp
template OGDBSetting* OGDBSetting::registerSetting<int>(
    const char*, const char*, const char*, const char*, int, const std::any, SettingEnum::Type, bool);

template OGDBSetting* OGDBSetting::registerSetting<bool>(
    const char*, const char*, const char*, const char*, bool, const std::any, SettingEnum::Type, bool);

template OGDBSetting* OGDBSetting::registerSetting<float>(
    const char*, const char*, const char*, const char*, float, const std::any, SettingEnum::Type, bool);

template OGDBSetting* OGDBSetting::registerSetting<ccColor4B>(
    const char*, const char*, const char*, const char*, ccColor4B, const std::any, SettingEnum::Type, bool);

template OGDBSetting* OGDBSetting::registerSetting<std::string>(
    const char*, const char*, const char*, const char*, std::string, const std::any, SettingEnum::Type, bool);
