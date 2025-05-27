#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class ColorPickPopupDelegateCallback : public ColorPickPopupDelegate {
    ccColor4B* m_colorRef;
    std::function<void(ccColor4B const&, std::string const&)> m_callback;

public:
    ColorPickPopupDelegateCallback(
        ccColor4B* colorRef,
        std::function<void(ccColor4B const&, std::string const&)> callback = nullptr
    )
    : m_colorRef(colorRef), m_callback(callback) {}

    void updateColor(ccColor4B const& color) override {

        std::string percentStr = fmt::format("{}%", static_cast<int>((color.a / 255.0f) * 100));

        if (m_callback)
             m_callback(color, percentStr);
    }
};