cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS" OR IOS)
  set(CMAKE_OSX_ARCHITECTURES "arm64")
else()
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
endif()


# set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

project(obeygdbot VERSION 1.0.0)

add_library(${PROJECT_NAME} SHARED
    src/main.cpp
    src/utils/utils.cpp
    # src/utils/utils.hpp
    src/utils/bg.cpp
    # src/utils/bg.hpp
    src/ui/settings.cpp
    # src/ui/settings.hpp

    # src/ui/popups/CustomizeProfilePopup.hpp
    # Profiles
    # src/profile/main.hpp
    src/profile/main.cpp
    src/profile/commentCell.cpp
    # more settings
    # src/settings/SettingsEnum.hpp

    # src/settings/OGDBSettingCell.hpp
    src/settings/OGDBSettingCell.cpp
    # src/settings/OGDBSetting.hpp
    src/settings/OGDBSetting.cpp
    # more utils
    # src/utils/FLSelectorLayer.hpp
    # images
    src/utils/OGDBAutoImage.cpp
    src/utils/OGDBAutoImage.hpp
    # web
    # src/utils/OGDBRequests.hpp
    # cache
    src/utils/OGDBCache.cpp
    src/utils/OGDBCache.hpp

    # src/utils/delegates/customColorCallback.hpp
    # Preassets
    src/presets/OGDBPresetProfile.cpp
    # src/presets/OGDBPresetProfile.hpp
    src/presets/assets/CustomBackgroundSprite.cpp
    # src/presets/assets/CustomBackgroundSprite.hpp
    src/presets/assets/CustomSubbackground.cpp
    # src/presets/assets/CustomSubbackground.hpp
    src/presets/assets/CustomBorderSprite.cpp
    # src/presets/assets/CustomBorderSprite.hpp
    #
    src/presets/assets/CustomIconsBackground.cpp
    # src/presets/assets/CustomIconsBackground.hpp
    src/presets/assets/CustomCommentsBackground.cpp
    # src/presets/assets/CustomCommentsBackground.hpp
    
    

    # Add any extra C++ source files here
)

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

setup_geode_mod(${PROJECT_NAME})
