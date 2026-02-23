#pragma once

#include <endstone/endstone.hpp>
#include <endstone/plugin/plugin.h>
#include <nlohmann/json.hpp>

class DynamicLights : public endstone::Plugin {
public:
    void onEnable() override;
    void onDisable() override;

    void loadConfig();

    void onJoin(endstone::PlayerJoinEvent &event);
    void onQuit(endstone::PlayerQuitEvent &event);
    void onMove(endstone::PlayerMoveEvent &event);
    void onItemHeld(endstone::PlayerItemHeldEvent &event);

    int getLightValue(std::string typeID);
    void sendLightOrAir(endstone::Player* player, endstone::Location location, int light_level);

private:
    std::map<endstone::UUID, std::vector<endstone::Location>> light_map = {};
    std::map<endstone::UUID, std::string> mainhand_map = {};
    std::map<endstone::UUID, std::string> offhand_map = {};
    nlohmann::json config;
};