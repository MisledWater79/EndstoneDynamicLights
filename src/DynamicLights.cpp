#include "DynamicLights.hpp"
#include "BinStream.hpp"

#include <fmt/core.h>
#include <fstream>
#include <filesystem>

using json = nlohmann::json;

using namespace endstone;

void DynamicLights::onEnable() {
    getLogger().info("Misled Dynamic Lights Loading");

    loadConfig();

    registerEvent(&DynamicLights::onMove, *this);
    registerEvent(&DynamicLights::onItemHeld, *this);
    registerEvent(&DynamicLights::onJoin, *this);

    getLogger().info("Misled Dynamic Lights Enabled");
}

void DynamicLights::onDisable() {
    getLogger().info("Misled Dynamic Lights Disabled");
}

void DynamicLights::loadConfig() {
    if (std::filesystem::exists("plugins/DynamicLights/config.json")) {
        std::ifstream f("plugins/DynamicLights/config.json");
        config = json::parse(f);
        return;
    };
    getLogger().info("Using default config");
    config = json::parse(R"({
        "dynamic_lights": {
            "minecraft:dirt": 15
        }
    })");
}


void DynamicLights::onJoin(PlayerJoinEvent &event) {
    UUID u = event.getPlayer().getUniqueId();
    std::vector<Location> locations = {};

    light_map.insert({u, {}});
    mainhand_map.insert({u, "minecraft:air"});
    offhand_map.insert({u, "minecraft:air"});
}

void DynamicLights::onQuit(PlayerQuitEvent &event) {
    UUID u = event.getPlayer().getUniqueId();
    if (light_map.contains(u)) light_map.erase(u);
    if (mainhand_map.contains(u)) mainhand_map.erase(u);
    if (offhand_map.contains(u)) offhand_map.erase(u);
}

void DynamicLights::onMove(PlayerMoveEvent &event) {
    const Location from = event.getFrom();
    const Location to = event.getTo();
    Player* player = &event.getPlayer();
    UUID uuid = player->getUniqueId();

    if (from.getBlockX() == to.getBlockX()
        && from.getBlockY() == to.getBlockY()
        && from.getBlockZ() == to.getBlockZ()) return;

    for (Location loc : light_map.at(uuid)) {
        sendLightOrAir(player, loc, 0);
    }

    light_map.at(uuid).clear();

    int mainhand_level = getLightValue(mainhand_map.at(uuid));
    int offhand_level = getLightValue(offhand_map.at(uuid));

    if (mainhand_level == 0 && offhand_level == 0) return;

    if (mainhand_level >= offhand_level) sendLightOrAir(player, to, mainhand_level);
    else sendLightOrAir(player, to, offhand_level);

    light_map.at(uuid).push_back(to);
}

void DynamicLights::onItemHeld(PlayerItemHeldEvent &event) {
    Player* player = &event.getPlayer();
    UUID uuid = player->getUniqueId();

    std::optional<ItemStack> mainItem = player->getInventory().getItem(event.getNewSlot());
    std::optional<ItemStack> offItem = player->getInventory().getItemInOffHand();

    if (!mainhand_map.contains(uuid)) mainhand_map.insert({uuid, "minecraft:air"});
    if (!offhand_map.contains(uuid)) offhand_map.insert({uuid, "minecraft:air"});

    if (!mainItem.has_value()) mainhand_map.at(uuid) = "minecraft:air";
    else mainhand_map.at(uuid) = mainItem->getType().getId();
    if (!offItem.has_value()) offhand_map.at(uuid) = "minecraft:air";
    else offhand_map.at(uuid) = offItem->getType().getId();

    int mainhand_level = getLightValue(mainhand_map.at(uuid));
    int offhand_level = getLightValue(offhand_map.at(uuid));

    if (mainhand_level == 0 && offhand_level == 0) return sendLightOrAir(player, player->getLocation(), 0);

    if (mainhand_level >= offhand_level) sendLightOrAir(player, player->getLocation(), mainhand_level);
    else sendLightOrAir(player, player->getLocation(), offhand_level);

    light_map.at(uuid).push_back(player->getLocation());
}

int DynamicLights::getLightValue(std::string typeID) {
    if (typeID == "minecraft:air") return 0;
    for (auto it = config["dynamic_lights"].begin(); it != config["dynamic_lights"].end(); ++it) {
        if (typeID == it.key()) return it.value();
    }
    return 0;
}

void DynamicLights::sendLightOrAir(Player* player, Location location, int light_level) {
    std::string typeID = "minecraft:air";
    if (light_level > 0 && light_level <= 15) typeID =  fmt::format("minecraft:light_block_{}", light_level);

    std::unique_ptr<BlockData> data = getServer().createBlockData(typeID);

    std::unique_ptr<Block> bottom = player->getDimension().getBlockAt(location);

    if (bottom.get()->getType() == "minecraft:air") {
        BinStream bs = BinStream(18);
        bs.writeVarInt(location.getBlockX());
        bs.writeUVarInt(location.getBlockY());
        bs.writeVarInt(location.getBlockZ());
        bs.writeUVarInt(data.get()->getRuntimeId());
        bs.writeUVarInt(3); // Block Flags
        bs.writeUVarInt(0); // Layer
        player->sendPacket(21, std::string_view(reinterpret_cast<char *>(bs.getData()), bs.getPosition()));
    }

    bottom.release();

    location.setY(location.getBlockY() + 1);
    std::unique_ptr<Block> top = player->getDimension().getBlockAt(location);

    if (top.get()->getType() == "minecraft:air") {
        BinStream bs = BinStream(18);
        bs.writeVarInt(location.getBlockX());
        bs.writeUVarInt(location.getBlockY());
        bs.writeVarInt(location.getBlockZ());
        bs.writeUVarInt(data.get()->getRuntimeId());
        bs.writeUVarInt(3); // Block Flags
        bs.writeUVarInt(0); // Layer
        player->sendPacket(21, std::string_view(reinterpret_cast<char *>(bs.getData()), bs.getPosition()));
    }

    data.release();
}
