#pragma once

class SettingEnum {
public:
    enum class Type {
        Title,
        Bool,
        Options,
        // Special
        Slider,
        Text,
        Color4B,
        // Custom
        Setting_Profile_Custom
    };
};