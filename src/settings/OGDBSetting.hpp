#pragma once

#include <Geode/Geode.hpp>
#include <any>

#include "SettingsEnum.hpp" // Rewrite in 1.0-alpha


using namespace geode::prelude;

class OGDBSetting {
public:
    OGDBSetting(const char* id);

    template<typename T>
    void setValue(T value) {
        geode::Mod::get()->setSavedValue<T>(setting_id, value);
    }

    template<typename T>
    T getValue() const {
        if (geode::Mod::get()->hasSavedValue(setting_id))
            return geode::Mod::get()->getSavedValue<T>(setting_id);
        return std::any_cast<T>(setting_default_value);
    }

    const char* getID() const;
    const char* getName() const;
    const char* getDescription() const;
    const char* getRequiredText() const;
    SettingEnum::Type getType() const;
    bool getVisible() const;

    std::any getDefaultValue() const;
    std::any getDefaultOptions() const;

    void setName(const char* name);
    void setDescription(const char* desc);
    void setRequiredText(const char* setting_required_txt);
    void setType(SettingEnum::Type type);
    void setVisible(bool isVisible);

    static const std::map<std::string, OGDBSetting*>& getAllSettings();
    static const std::vector<std::string>& getOrderKeySettings();

    template<typename T>
    static OGDBSetting* registerSetting(const char* id, 
        const char* name, 
        const char* desc, 
        const char* requiredtxt,
        T defaultValue,
        const std::any defaultOptions, 
        SettingEnum::Type type = SettingEnum::Type::Bool,
        bool isHidden = false
    );

    

    static OGDBSetting* get(const char* id);
    static void init();

private:
    const char* setting_id = "";
    const char* setting_name = "";
    const char* setting_desc = "";
    const char* setting_required_text = "";

    bool setting_is_visible = false;

    std::any setting_default_value;
    std::any setting_default_options;

    SettingEnum::Type setting_type = SettingEnum::Type::Bool;

    static inline std::map<std::string, OGDBSetting*> settingsRegistry;
    static inline std::vector<std::string> settingsKeyOrdenList;

};