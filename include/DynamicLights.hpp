#pragma once

#include <endstone/endstone.hpp>
#include <endstone/plugin/plugin.h>

class DynamicLights : public endstone::Plugin {
public:
    void onEnable() override;
    void onDisable() override;
};