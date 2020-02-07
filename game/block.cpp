#include "block.h"

const char* Blocks::getBlockName(int num)
{
    switch(num)
    {
        case 0: return "Air";
        case 1: return "Grass Block";
        case 2: return "Dirt";
        case 3: return "Oak Wood";
        case 4: return "Oak Leaves";
        case 5: return "Oak Planks";
        case 6: return "Brick";
        case 7: return "Cobblestone";
        case 8: return "Gravel";
        case 9: return "Sand";
        case 10: return "Sandstone";
        case 11: return "TNT";
        case 12: return "Cactus";
        case 13: return "Glass";
        case 14: return "Ice";
        case 15: return "Glowstone";
        case 16: return "Bookshelf";
        case 17: return "Pumpkin";
        case 18: return "Noteblock";
        case 19: return "Wool";
        case 20: return "Crafting Table";
        case 21: return "Furnace";
        case 22: return "Coal Ore";
        case 23: return "Iron Ore";
        case 24: return "Gold Ore";
        case 25: return "Diamond Ore";
        case 26: return "Water";
        case 27: return "Stone";
        case 28: return "Obsidian";
        case 29: return "Bedrock";

        default: return "???";
    }
}
