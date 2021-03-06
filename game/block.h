#ifndef BLOCK_H_INCLUDED
#define BLOCK_H_INCLUDED

#include <SFML/Config.hpp>

enum
{
    BLOCK_AIR,
    BLOCK_GRASS,
    BLOCK_DIRT,
    BLOCK_OAK_WOOD,
    BLOCK_OAK_LEAVES,
    BLOCK_OAK_PLANKS,
    BLOCK_BRICK,
    BLOCK_COBBLESTONE,
    BLOCK_GRAVEL,
    BLOCK_SAND,
    BLOCK_SANDSTONE,
    BLOCK_TNT,
    BLOCK_CACTUS,
    BLOCK_GLASS,
    BLOCK_ICE,
    BLOCK_GLOWSTONE,
    BLOCK_BOOKSHELF,
    BLOCK_PUMPKIN,
    BLOCK_NOTEBLOCK,
    BLOCK_WOOL,
    BLOCK_CRAFTINGTABLE,
    BLOCK_FURNACE,
    BLOCK_COAL_ORE,
    BLOCK_IRON_ORE,
    BLOCK_GOLD_ORE,
    BLOCK_DIAMOND_ORE,
    BLOCK_WATER,
    BLOCK_STONE,
    BLOCK_OBSIDIAN,
    BLOCK_BEDROCK,
    BLOCK_TOTAL
};

enum
{
    LAYER_BUILD,
    LAYER_NONSOLID,
    LAYER_DECORATION
};

enum
{
    BLOCKFLAG_SLAB_TOP = 1<<0,
    BLOCKFLAG_SLAB_BOTTOM = 1<<1,
    BLOCKFLAG_STAIRS = 1<<2,
    BLOCKFLAG_DOOR_BOTTOM = 1<<3,
    BLOCKFLAG_DOOR_TOP = 1<<4,
    BLOCKFLAG_FRONT = 1<<5
};


class Block
{
public:
    Block() {m_dir = 1; m_layer = LAYER_BUILD; m_blockflags = 0;}
    void setDir(int dir) {m_dir = dir;}
    void setLayer(sf::Uint8 layer) {m_layer = layer;}
    int getDir() {return m_dir;}
    sf::Uint8 getLayer() {return m_layer;}
    int m_blockflags;

private:
    int m_dir;
    sf::Uint8 m_layer;
};

namespace Blocks
{
    const char * getBlockName(int num);
};


#endif // BLOCK_H_INCLUDED
