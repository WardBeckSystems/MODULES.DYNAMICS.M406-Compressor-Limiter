#pragma once
#include "PluginProcessor.hpp"

namespace wbs::m406 {

class M406PluginEditor {
public:
    explicit M406PluginEditor(M406PluginProcessor& processor)
        : processor_(processor) {}

    void paint() const noexcept;
    void resized() noexcept;

    int getWidth() const noexcept { return 480; }
    int getHeight() const noexcept { return 360; }

private:
    M406PluginProcessor& processor_;
};

} // namespace wbs::m406
