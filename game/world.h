#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDED

#define DEFAULT_WORLD_W 2048
#define DEFAULT_WORLD_H 192
#define CHUNK_W 16
#define CHUNK_H 16

#include <SFML/Graphics.hpp>
#include "../game_engine.h"
#include "chunk.h"
#include "entity.h"
#include "player.h"
#include <vector>

enum
{
    BIOME_PLAINS,
    BIOME_PLAINS_TREES,
    BIOME_DESERT,
    BIOME_MIXED
};

class Chunk;
class Entity;
class Player;

class World
{
public:
    World();
    World(GameEngine *engine);
    ~World();

    void setBlock(uint32_t x, uint32_t y, int block, sf::Uint8 layer=LAYER_BUILD);
    int getBlock(uint32_t x, uint32_t y);
    sf::Uint8 getBlockLayer(uint32_t x, uint32_t y);
    int getBlockFlags(uint32_t x, uint32_t y);

    void updateLighting(uint32_t x, uint32_t y);

    void addEntity(Entity* ent);
    void updateEntities();
    void drawEntities();
    std::vector<Entity*> getNearestEntities(float x, float y, int type, int radius);

    void generateWorld(unsigned int seed, const char *name, uint32_t width=DEFAULT_WORLD_W, uint32_t height=DEFAULT_WORLD_H, int biome=-1);
    void generateFlatWorld(const char *name, const std::vector<int>& blocks, uint32_t width=DEFAULT_WORLD_W, uint32_t height=DEFAULT_WORLD_H);
    void loadWorld(const char *worldName);
    void saveWorld();

    unsigned int Width() const {return m_width;}
    unsigned int Height() const {return m_height;}
    unsigned int getSeed() const {return m_seed;}
    Player* getPlayer() {return m_player;}
    sf::VertexArray& getBlocksFromPoint(uint32_t x, uint32_t y, bool front=false);
    std::vector<std::vector<Chunk>>* getBlocks() {return &m_blocks2;}

private:
    bool loaded;
    char fileName[96];
    unsigned int m_seed, m_width, m_height;
    GameEngine *m_engine;

    Player* m_player; // will later be changed to vectors for multiplayer
    std::vector<std::vector<Chunk>> m_blocks2;
    std::vector<Entity*> m_entities;
};

#endif // WORLD_H_INCLUDED
