#include "DynamicLights.hpp"
#include "BinStream.hpp"

#include <fstream>
#include <filesystem>

using json = nlohmann::json;

using namespace endstone;

void DynamicLights::onEnable() {
    getLogger().info("Misled Dynamic Lights Loading");

    loadConfig();
    for (auto it = config["dynamic_lights"].begin(); it != config["dynamic_lights"].end(); ++it) {
        itemLightMap.insert({it.key(), it.value()});
    }

    registerEvent(&DynamicLights::onJoin, *this);
    registerEvent(&DynamicLights::onQuit, *this);
    registerEvent(&DynamicLights::onMove, *this);
    registerEvent(&DynamicLights::onItemHeld, *this);

    getServer().getScheduler().runTaskTimer(*this, [this]() -> void {
        for (const auto player : getServer().getOnlinePlayers()) {
            updateData(player);
            updateLights(player);
        }
    }, 0, 20);

    getLogger().info("Misled Dynamic Lights Enabled");
}

void DynamicLights::onDisable() {
    getLogger().info("Misled Dynamic Lights Disabled");
}

void DynamicLights::loadConfig() {
    if (std::filesystem::exists("plugins/DynamicLights/config.json")) {
        if (isValidConfig()) return getLogger().info("Using loaded config");
    }
    getLogger().info("Using default config");
    config = json::parse(R"({
        "dynamic_lights": {
            "minecraft:torch": 14,
            "minecraft:soul_torch": 10,
            "minecraft:redstone_torch": 7,
            "minecraft:copper_torch": 14,

            "minecraft:lantern": 15,
            "minecraft:soul_lantern": 10,
            "minecraft:copper_lantern": 15,
            "minecraft:exposed_copper_lantern": 12,
            "minecraft:weathered_copper_lantern": 7,
            "minecraft:oxidized_copper_lantern": 4,
            "minecraft:waxed_copper_lantern": 15,
            "minecraft:waxed_exposed_copper_lantern": 12,
            "minecraft:waxed_weathered_copper_lantern": 7,
            "minecraft:waxed_oxidized_copper_lantern": 4,

            "minecraft:lava_bucket": 15,

            "minecraft:campfire": 15,
            "minecraft:soul_campfire": 10,

            "minecraft:beacon": 15,
            "minecraft:enchanting_table": 7,
            "minecraft:glowstone": 15,
            "minecraft:lit_pumpkin": 15,
            "minecraft:sculk_catalyst": 6,

            "minecraft:ochre_froglight": 15,
            "minecraft:pearlescent_froglight": 15,
            "minecraft:verdant_froglight": 15,

            "minecraft:conduit": 15,
            "minecraft:sea_lantern": 15,
            "minecraft:sea_pickle": 6,

            "minecraft:brown_mushroom": 1,
            "minecraft:glow_berries": 14,
            "minecraft:glow_lichen": 7,

            "minecraft:small_amethyst_bud": 1,
            "minecraft:medium_amethyst_bud": 2,
            "minecraft:large_amethyst_bud": 4,
            "minecraft:amethyst_cluster": 5,

            "minecraft:magma": 3,
            "minecraft:shroomlight": 15,
            "minecraft:crying_obsidian": 8,

            "minecraft:end_rod": 14,
            "minecraft:ender_chest": 7,
            "minecraft:dragon_egg": 1
        },
        "offhand": [
            "minecraft:torch",
            "minecraft:soul_torch",
            "minecraft:redstone_torch",
            "minecraft:copper_torch",

            "minecraft:lantern",
            "minecraft:soul_lantern",
            "minecraft:copper_lantern",
            "minecraft:exposed_copper_lantern",
            "minecraft:weathered_copper_lantern",
            "minecraft:oxidized_copper_lantern",
            "minecraft:waxed_copper_lantern",
            "minecraft:waxed_exposed_copper_lantern",
            "minecraft:waxed_weathered_copper_lantern",
            "minecraft:waxed_oxidized_copper_lantern",

            "minecraft:lava_bucket",

            "minecraft:campfire",
            "minecraft:soul_campfire",

            "minecraft:beacon",
            "minecraft:enchanting_table",
            "minecraft:glowstone",
            "minecraft:lit_pumpkin",
            "minecraft:sculk_catalyst",

            "minecraft:ochre_froglight",
            "minecraft:pearlescent_froglight",
            "minecraft:verdant_froglight",

            "minecraft:conduit",
            "minecraft:sea_lantern",
            "minecraft:sea_pickle",

            "minecraft:brown_mushroom",
            "minecraft:glow_berries",
            "minecraft:glow_lichen",

            "minecraft:small_amethyst_bud",
            "minecraft:medium_amethyst_bud",
            "minecraft:large_amethyst_bud",
            "minecraft:amethyst_cluster",

            "minecraft:magma",
            "minecraft:shroomlight",
            "minecraft:crying_obsidian",

            "minecraft:end_rod",
            "minecraft:ender_chest"
        ]
    })");

    if (!std::filesystem::exists("plugins/DynamicLights/config.json")) {
        std::filesystem::create_directories("plugins/DynamicLights");
        std::ofstream out("plugins/DynamicLights/config.json");
        out << config.dump(4);
        out.close();
    }
}

bool DynamicLights::isValidConfig() {
    std::ifstream f("plugins/DynamicLights/config.json");
    if (!f.is_open()) return false;
    try {
        nlohmann::json j;
        f >> j;

        if (!j.contains("dynamic_lights") || !j["dynamic_lights"].is_object()) {
            getLogger().error("Invalid JSON Config. No \"dynamic_light\" object");
            return false;
        }
        if (!j.contains("offhand") || !j["offhand"].is_array()) {
            getLogger().error("Invalid JSON Config. No \"offhand\" array");
            return false;
        }

        for (auto it = j["dynamic_lights"].begin(); it != j["dynamic_lights"].end(); ++it) {
            if (getServer().getRegistry<ItemType>().get(it.key()) == nullptr) {
                getLogger().error("Invalid JSON Config. Could not find item type ID \"" + it.key() + "\"");
                return false;
            }
            if (!it.value().is_number_integer()) {
                getLogger().error("Invalid JSON Config. Non-integer in dynamic lights at item type ID \"" + it.key() + "\"");
                return false;
            }
        }

        for (const auto& id : j["offhand"]) {
            if (!id.is_string()) {
                getLogger().error("Invalid JSON Config. Non-string in offhand");
                return false;
            }
            if (getServer().getRegistry<ItemType>().get(id.get<std::string>()) == nullptr) {
                getLogger().error("Invalid JSON Config. Could not find item type ID \"" + id.get<std::string>() + "\"");
                return false;
            }
        }

        config = j;

        return true;
    } catch (const nlohmann::json::parse_error&) {
        return false;
    }
}

void DynamicLights::onJoin(PlayerJoinEvent &event) {
    UUID uuid = event.getPlayer().getUniqueId();
    light_map.insert({uuid, {}});
    mainhand_map.insert({uuid, "minecraft:air"});
    offhand_map.insert({uuid, "minecraft:air"});
}

void DynamicLights::onQuit(PlayerQuitEvent &event) {
    UUID uuid = event.getPlayer().getUniqueId();
    if (light_map.contains(uuid)) light_map.erase(uuid);
    if (mainhand_map.contains(uuid)) mainhand_map.erase(uuid);
    if (offhand_map.contains(uuid)) offhand_map.erase(uuid);
}

void DynamicLights::onMove(PlayerMoveEvent &event) {
    const Location from = event.getFrom();
    const Location to = event.getTo();

    if (from.getBlockX() == to.getBlockX()
        && from.getBlockY() == to.getBlockY()
        && from.getBlockZ() == to.getBlockZ()) return;

    Player* player = &event.getPlayer();

    getServer().getScheduler().runTaskLater(*this, [this, player]() -> void {
        updateLights(player);
    }, 1);
}

void DynamicLights::onItemHeld(PlayerItemHeldEvent &event) {
    Player* player = &event.getPlayer();

    getServer().getScheduler().runTaskLater(*this, [this, player]() -> void {
        updateData(player);
        updateLights(player);
    }, 1);
}



bool DynamicLights::hasLightInHand(Player* player) {
    std::optional<ItemStack> mainItem = player->getInventory().getItemInMainHand();
    std::optional<ItemStack> offItem = player->getInventory().getItemInOffHand();

    if (mainItem.has_value()) {
        if (itemLightMap.contains(mainItem.value().getType().getId())) return true;
    }
    if (offItem.has_value()) {
        if (itemLightMap.contains(offItem.value().getType().getId())) return true;
    }

    return false;
}

void DynamicLights::updateData(Player* player) {
    UUID uuid = player->getUniqueId();

    std::optional<ItemStack> mainItem = player->getInventory().getItemInMainHand();
    std::optional<ItemStack> offItem = player->getInventory().getItemInOffHand();

    if (!mainhand_map.contains(uuid)) mainhand_map.insert({uuid, "minecraft:air"});
    if (!offhand_map.contains(uuid)) offhand_map.insert({uuid, "minecraft:air"});

    if (!mainItem.has_value()) mainhand_map.at(uuid) = "minecraft:air";
    else mainhand_map.at(uuid) = mainItem->getType().getId();
    if (!offItem.has_value()) offhand_map.at(uuid) = "minecraft:air";
    else offhand_map.at(uuid) = offItem->getType().getId();
}

void DynamicLights::updateLights(Player* player) {
    UUID uuid = player->getUniqueId();

    for (Location loc : light_map.at(uuid)) {
        sendLightOrAir(player, loc, 0);
    }

    light_map.at(uuid).clear();

    int mainhand_level = getLightValue(mainhand_map.at(uuid));
    int offhand_level = getLightValue(offhand_map.at(uuid));

    if (mainhand_level == 0 && offhand_level == 0) return;

    if (mainhand_level >= offhand_level) sendLightOrAir(player, player->getLocation(), mainhand_level);
    else sendLightOrAir(player, player->getLocation(), offhand_level);

    light_map.at(uuid).push_back(player->getLocation());
}

int DynamicLights::getLightValue(std::string typeID) {
    if (typeID == "minecraft:air") return 0;
    if (itemLightMap.contains(typeID)) return itemLightMap.at(typeID);
    return 0;
}

bool DynamicLights::isOffhandable(std::string typeID) {
    if (typeID == "minecraft:air") return false;
    return config["offhand"].contains(typeID);
}

void DynamicLights::sendLightOrAir(Player* player, Location location, int light_level) {
    std::string typeID = "minecraft:air";
    if (light_level > 0 && light_level <= 15) {
        typeID = "minecraft:light_block_";
        typeID += std::to_string(light_level);
    }

    std::unique_ptr<BlockData> data = getServer().createBlockData(typeID);

    std::unique_ptr<Block> bottom = player->getDimension().getBlockAt(location);

    if (bottom.get()->getType() == "minecraft:air") {
        BinStream bs = BinStream(18);
        bs.writeVarInt(location.getBlockX());
        bs.writeVarInt(location.getBlockY());
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
        bs.writeVarInt(location.getBlockY());
        bs.writeVarInt(location.getBlockZ());
        bs.writeUVarInt(data.get()->getRuntimeId());
        bs.writeUVarInt(3); // Block Flags
        bs.writeUVarInt(0); // Layer
        player->sendPacket(21, std::string_view(reinterpret_cast<char *>(bs.getData()), bs.getPosition()));
    }

    data.release();
}
