#include "block.h"

const char* Blocks::getBlockName(int num)
{
    return (num == 0) ? "Air" :
           (num == 1) ? "Grass Block" :
           (num == 2) ? "Dirt" :
           (num == 3) ? "Oak Wood" :
           (num == 4) ? "Oak Leaves" :
           (num == 5) ? "Oak Planks" :
           (num == 6) ? "Brick" :
           (num == 7) ? "Cobblestone" :
           (num == 8) ? "Gravel" :
           (num == 9) ? "Sand" :
           (num == 10) ? "Sandstone" :
           (num == 11) ? "TNT" :
           (num == 12) ? "Cactus" :
           (num == 13) ? "Glass" :
           (num == 14) ? "Ice" :
           (num == 15) ? "Glowstone" :
           (num == 16) ? "Bookshelf" :
           (num == 17) ? "Pumpkin" :
           (num == 18) ? "Noteblock" :
           (num == 19) ? "Wool" :
           (num == 20) ? "Crafting Table" :
           (num == 21) ? "Furnace" :
           (num == 22) ? "Coal Ore" :
           (num == 23) ? "Iron Ore" :
           (num == 24) ? "Gold Ore" :
           (num == 25) ? "Diamond Ore" :
           (num == 26) ? "Water" :
           (num == 27) ? "Stone" :
           (num == 28) ? "Obsidian" :
           (num == 29) ? "Bedrock" :
           "???";
}
