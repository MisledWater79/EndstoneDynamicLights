# Endstone Dynamic Lights
![GitHub Downloads (specific asset, all releases)](https://img.shields.io/github/downloads/MisledWater79/EndstoneDynamicLights/total?label=Downloads)
![](https://img.shields.io/badge/language-c++-blue.svg?label=Language)
![GitHub License](https://img.shields.io/github/license/MisledWater79/EndstoneDynamicLights?label=License)

An endstone plugin that implements dynamic lights for items held in players hands.

### ✨ Credits & Thanks
#### [U-DynamicLight](https://github.com/U-Blocks/U_DynamicLight) - Inspiration & Data

---

## Features

* **Support for custom items**
* **Offhandable items**
* **Complete customization**

---

## Installation

**Supported Endstone versions:**

| Endstone Version | Full Support | Partial Support | No Support |
|:----------------:|:------------:|:---------------:|:----------:|
|   <= 0.10. 15    |      ❌       |        ❌        |     ✅      |
|      0.11.0      |      ✅       |        ❌        |     ❌      |
|      0.11.3      |      ✅       |        ❌        |     ❌      |

Make your way to the [Releases](https://github.com/MisledWater79/EndstoneDynamicLights/releases) page and download the respective version, and file format.
Use `.dll` for windows servers and `.so` for linux servers.

Once downloaded place the file into your `/plugins/` folder in your server.

Now start the server (or use `/reload`).

### Config

After installing the plugin, a `config.json` file will appear in the `/plugins/DynamicLights/` folder.
Here you can manage what items are dynamic lights, and whether or not they can be offhanded.

The format for this file is as such:
```json
{
  // For any Dynamic Light items
  "dynamic_lights": {
    "item:type_id": 15
  },
  // For any Offhandable items
  "offhand": [
    "item:type_id"
  ]
}
```

---

### Docs & Info

Here is the default config details:

|              Item Name              | Light Level | Offhandable |       Item Name       | Light Level | Offhandable |
|:-----------------------------------:|:-----------:|:-----------:|:---------------------:|:-----------:|:-----------:|
|                Torch                |     14      |      ✅      |   Orchre Froglight    |     15      |      ✅      |
|             Soul Torch              |     10      |      ✅      | Pearlescent Froglight |     15      |      ✅      |
|           Redstone Torch            |      7      |      ✅      |   Verdant Froglight   |     15      |      ✅      |
|            Copper Torch             |     14      |      ✅      |        Conduit        |     15      |      ✅      |
|               Lantern               |     15      |      ✅      |      Sea Lantern      |     15      |      ✅      |
|            Soul Lantern             |     10      |      ✅      |      Sea pickle       |      6      |      ✅      |
|      Copper Lantern (w/wo Wax)      |     15      |      ✅      |    Brown Mushroom     |      1      |      ✅      |
|  Exposed Copper Lantern (w/wo Wax)  |     12      |      ✅      |     Glow Berries      |     14      |      ✅      |
| Weathered Copper Lantern (w/wo Wax) |      7      |      ✅      |      Glow Lichen      |      7      |      ✅      |
| Oxidized Copper Lantern (w/wo Wax)  |      4      |      ✅      |  Small Amethyst Bud   |      1      |      ✅      |
|             Lava Bucket             |     15      |      ✅      |  Medium Amethyst Bud  |      2      |      ✅      |
|              Campfire               |     15      |      ✅      |  Large Amethyst Bud   |      4      |      ✅      |
|            Soul Campfire            |     10      |      ✅      |   Amethyst Cluster    |      5      |      ✅      |
|               Beacon                |     15      |      ✅      |         Magma         |      3      |      ✅      |
|          Enchanting Table           |      7      |      ✅      |      Shroomlight      |     15      |      ✅      |
|              Glowstone              |     15      |      ✅      |    Crying Obsidian    |      8      |      ✅      |
|             Lit Pumpkin             |     15      |      ✅      |        End Rod        |     14      |      ✅      |
|           Sculk Catalyst            |      6      |      ✅      |      Ender Chest      |      7      |      ✅      |
|                                     |             |             |      Dragon Egg       |      1      |      ❌      |

---

## Support & Contribution
I don't like asking for money; and all my projects will always be free, so for those who like my work and would like to support
* Buy me a [Coffee](https://ko-fi.com/misledwater79).

### Contribution
If you would like to help out with this or any of my projects
* Create an [Issue](https://github.com/MisledWater79/EndstoneDynamicLights/issues) for any bugs.
* Submit a [Pull Request](https://github.com/MisledWater79/EndstoneDynamicLights/pulls) for any contributions.
* Join the [Discord]() for miscellaneous stuff & chatting.