#include "world.h"
#include "entities/falling_block.h"
#include "entities/tnt.h"
#include "entities/particles/explosion.h"
#include <fstream>
#include <stdlib.h>
#include <math.h>

World::World()
{
    loaded = false;
    m_blocks2.clear();
    m_entities.clear();
    for(int yy=0; yy < WORLD_H/CHUNK_H+1; yy++)
    {
        m_blocks2.push_back(std::vector<Chunk>());
        for (int xx=0; xx < WORLD_W/CHUNK_W+1; xx++)
        {
            m_blocks2.back().push_back(Chunk());
        }
    }
}

World::World(GameEngine *engine)
{
    loaded = false;
    m_engine = engine;
    m_blocks2.clear();
    m_entities.clear();
    for(int yy=0; yy < WORLD_H/CHUNK_H+1; yy++)
    {
        m_blocks2.push_back(std::vector<Chunk>());
        for (int xx=0; xx < WORLD_W/CHUNK_W+1; xx++)
        {
            m_blocks2.back().push_back(Chunk(engine));
        }
    }
}

World::~World()
{
    m_blocks2.clear();
    m_entities.clear();
    if (m_player) delete m_player;
}

void World::addEntity(Entity *ent)
{
    m_entities.push_back(ent);
}

std::vector<Entity*> World::getNearestEntities(float x, float y, int type, int radius)
{
    std::vector<Entity*> ents;
    for (unsigned int i=0; i<m_entities.size(); i++)
    {
        if (m_entities[i]->entityId() != type) continue;

        sf::Vector2f pos = m_entities[i]->getPos();
        float dist;
        dist = sqrt(pow(pos.x - x, 2) + pow(pos.y - y, 2));
        if (dist < radius)
            ents.push_back(m_entities[i]);
    }
    return ents;
}

void World::updateEntities()
{
    for (unsigned int i=0; i<m_entities.size(); i++)
    {
        if (i >= m_entities.size())
            break;

        if (m_entities[i])
        {
            m_entities[i]->update();
            if (m_entities[i]->entityId() == ENT_FALLINGBLOCK)
            {
                if (m_entities[i]->groundCollide())
                {
                    sf::Vector2f pos = m_entities[i]->getPos();
                    setBlock(pos.x/32, pos.y/32+1, m_entities[i]->getBlock());
                    delete m_entities[i];
                    m_entities.erase(m_entities.begin() + i);
                    i--;
                }
            }
            else if (m_entities[i]->entityId() == ENT_TNT)
            {
                if (m_entities[i]->getTicksLeft() == 0)
                {
                    sf::Vector2f pos = m_entities[i]->getPos();
                    int radius = 120;

                    std::vector<Entity*> nearTNT = getNearestEntities(pos.x, pos.y, ENT_TNT, radius);
                    std::vector<Entity*> nearGravBlocks = getNearestEntities(pos.x, pos.y, ENT_FALLINGBLOCK, radius);
                    for(unsigned int i=0; i<nearTNT.size(); i++)
                        nearTNT[i]->knockBack(pos.x, pos.y, radius);
                    for(unsigned int i=0; i<nearGravBlocks.size(); i++)
                        nearGravBlocks[i]->knockBack(pos.x, pos.y, radius);
                    m_player->knockBack(pos.x+16, pos.y+32, radius);

                    for (int yy=-3; yy<4; yy++)
                    {
                        for (int xx=-3; xx<4; xx++)
                        {
                            if ((yy == 3 and xx == 3) or (yy == -3 and xx == -3) or (yy == 3 and xx == -3) or (yy == -3 and xx == 3))
                                continue;

                            int block2 = getBlock(pos.x/32+xx, pos.y/32+yy);
                            if (block2 == BLOCK_TNT)
                            {
                                Entity *tnt = new Dynamite(this, m_engine, pos.x/32+xx, pos.y/32+yy, 25 + (rand() % 20));
                                tnt->knockBack(pos.x, pos.y, radius);
                                addEntity(tnt);
                            }

                            if (block2 != BLOCK_BEDROCK and block2 != BLOCK_OBSIDIAN)
                                setBlock(pos.x/32+xx, pos.y/32+yy, BLOCK_AIR);
                        }
                    }

                    for (int i=0; i < (8 + (rand() % 5)); i++)
                    {
                        float xx = (i) ? pos.x + (-96 + (rand() % 192)) : pos.x;
                        float yy = (i) ? pos.y + (-96 + (rand() % 192)) : pos.y;
                        ExplosionParticle *particle = new ExplosionParticle(this, m_engine, xx, yy);
                        addEntity(particle);
                    }
                    sf::Vector2f view = m_engine->m_window.getView().getCenter();
                    m_engine->Sound()->playSample(pos.x, pos.y, view.x, view.y, SAMPLE_EXPLODE);
                    delete m_entities[i];
                    m_entities.erase(m_entities.begin() + i);
                    i--;
                }
            }
            else if (m_entities[i]->entityId() == ENT_PARTICLE)
            {
                if (m_entities[i]->willDestroy())
                {
                    delete m_entities[i];
                    m_entities.erase(m_entities.begin() + i);
                    i--;
                }
            }
        }
    }
}

void World::drawEntities()
{
    sf::Vector2i windowsize = sf::Vector2i(m_engine->m_window.getSize().x, m_engine->m_window.getSize().y);
    for (unsigned int i=0; i<m_entities.size(); i++)
    {
        if (m_entities[i])
        {
            sf::Vector2f pos = m_entities[i]->getPos();
            sf::Vector2i screenpos = m_engine->m_window.mapCoordsToPixel(pos, m_engine->m_window.getView());
            sf::Vector2i Size = sf::Vector2i(m_entities[i]->getSize().x, m_entities[i]->getSize().y);
            if (screenpos.x >= 0-Size.x and screenpos.x < windowsize.x and
                screenpos.y >= 0-Size.y and screenpos.y < windowsize.y)
                m_entities[i]->draw();
        }
    }
}

void World::setBlock(int x, int y, int block, int layer)
{
    int x_ind = x/CHUNK_W;
    int y_ind = y/CHUNK_H;
    int x_block_chunk = x % CHUNK_W;
    int y_block_chunk = y % CHUNK_H;
    int ind = (y_block_chunk * CHUNK_W + x_block_chunk)*4;

    if (x < 0 or x >= WORLD_W or y < 0 or y >= WORLD_H) return;

    m_blocks2[y_ind][x_ind].setBlock(x, y, block, layer);
    sf::VertexArray& m_blocks = getBlocksFromPoint(x, y);

    if (layer == LAYER_DECORATION)
    {
        m_blocks[ind+0].color = sf::Color(192, 192, 192);
        m_blocks[ind+1].color = sf::Color(192, 192, 192);
        m_blocks[ind+2].color = sf::Color(192, 192, 192);
        m_blocks[ind+3].color = sf::Color(192, 192, 192);
    }
    else if (layer == LAYER_BUILD)
    {
        updateLighting(x, y);
        if (x > 0)
            updateLighting(x-1, y);
        if (x < WORLD_W-1)
            updateLighting(x+1, y);
        if (y > 0)
            updateLighting(x, y-1);
        if (y < WORLD_H-1)
            updateLighting(x, y+1);
    }

    if (loaded) // only spawn block entities if the world has loaded
    {
        if (block == BLOCK_AIR)
        {
            if (y > 0)
            {
                if (getBlock(x, y-1) == BLOCK_SAND or getBlock(x, y-1) == BLOCK_GRAVEL)
                {
                    m_entities.push_back(new FallingBlock(this, m_engine, x, y-1, getBlock(x, y-1)));
                    setBlock(x, y-1, BLOCK_AIR);
                }
            }
        }
        else if (block == BLOCK_SAND or block == BLOCK_GRAVEL)
        {
            if (y < WORLD_H-1)
            {
                if (getBlock(x, y+1) == BLOCK_AIR)
                {
                    setBlock(x, y, BLOCK_AIR);
                    m_entities.push_back(new FallingBlock(this, m_engine, x, y, block));
                }
            }
        }
    }
}

int World::getBlock(int x, int y)
{
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x >= WORLD_W) x = WORLD_W-1;
    if (y >= WORLD_H) y = WORLD_H-1;

    int x_ind = x/CHUNK_W;
    int y_ind = y/CHUNK_H;
    int x_block_chunk = x % CHUNK_W;
    int y_block_chunk = y % CHUNK_H;

    int block = m_blocks2[y_ind][x_ind].getBlock(x_block_chunk, y_block_chunk);
    return block;
}

int World::getBlockFlags(int x, int y)
{
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x >= WORLD_W) x = WORLD_W-1;
    if (y >= WORLD_H) y = WORLD_H-1;

    int x_ind = x/CHUNK_W;
    int y_ind = y/CHUNK_H;
    int x_block_chunk = x % CHUNK_W;
    int y_block_chunk = y % CHUNK_H;

    int block = m_blocks2[y_ind][x_ind].getBlockFlags(x_block_chunk, y_block_chunk);
    return block;
}

int World::getBlockLayer(int x, int y)
{
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x >= WORLD_W) x = WORLD_W-1;
    if (y >= WORLD_H) y = WORLD_H-1;

    int x_ind = x/CHUNK_W;
    int y_ind = y/CHUNK_H;
    int x_block_chunk = x % CHUNK_W;
    int y_block_chunk = y % CHUNK_H;

    int block = m_blocks2[y_ind][x_ind].getBlockLayer(x_block_chunk, y_block_chunk);
    return block;
}

void World::updateLighting(int x, int y)
{
    int block = getBlock(x, y);
    int layer = getBlockLayer(x, y);
    int x_block_chunk = x % CHUNK_W;
    int y_block_chunk = y % CHUNK_H;
    int ind = (y_block_chunk * CHUNK_W + x_block_chunk)*4;
    sf::VertexArray& m_blocks = getBlocksFromPoint(x, y);

    if (layer == LAYER_DECORATION)
    {
        m_blocks[ind+0].color = sf::Color(192, 192, 192);
        m_blocks[ind+1].color = sf::Color(192, 192, 192);
        m_blocks[ind+2].color = sf::Color(192, 192, 192);
        m_blocks[ind+3].color = sf::Color(192, 192, 192);
    }
    else if (layer == LAYER_BUILD and block != BLOCK_AIR)
    {
        int alpha = (((!getBlock(x-1, y) or getBlockLayer(x-1, y) != LAYER_BUILD) or
                      (!getBlock(x+1, y) or getBlockLayer(x+1, y) != LAYER_BUILD) or
                      (!getBlock(x, y-1) or getBlockLayer(x, y-1) != LAYER_BUILD) or
                      (!getBlock(x, y+1) or getBlockLayer(x, y+1) != LAYER_BUILD))) ? 255 : 128;
        m_blocks[ind+0].color = sf::Color(alpha, alpha, alpha);
        m_blocks[ind+1].color = sf::Color(alpha, alpha, alpha);
        m_blocks[ind+2].color = sf::Color(alpha, alpha, alpha);
        m_blocks[ind+3].color = sf::Color(alpha, alpha, alpha);
    }
}

sf::VertexArray& World::getBlocksFromPoint(int x, int y, bool front)
{
    int x_ind = x/CHUNK_W;
    int y_ind = y/CHUNK_H;

    if (x_ind > WORLD_W/CHUNK_W) x_ind = WORLD_W/CHUNK_W;
    if (y_ind > WORLD_H/CHUNK_H) y_ind = WORLD_H/CHUNK_H;
    if (x_ind < 0) x_ind = 0;
    if (y_ind < 0) y_ind = 0;

    return (not front) ? m_blocks2[y_ind][x_ind].getVertex() : m_blocks2[y_ind][x_ind].getVertexFront();
}

void World::generateFlatWorld(const char *name, const std::vector<int>& blocks)
{
    sprintf(fileName, "worlds/%s.dat", name);
    loaded = false;

    printf("%d\n", blocks.size());
    for(int xx=0; xx<WORLD_W*32; xx+=32)
    {
        int yy = WORLD_H*32-32;
        for (unsigned i=0; i<blocks.size(); i++)
        {
            //printf("i %d\n", i);
            setBlock(xx/32, yy/32, blocks[i]);
            yy -= 32;
        }
    }

    saveWorld();
    printf("lighting up the world...\n");
    for(int i=0; i<WORLD_W*WORLD_H*4; i++)
    {
        int x = i % WORLD_W;
        int y = i / WORLD_W;
        updateLighting(x, y);
    }
    printf("completed!\n");
    m_player = new Player(this, m_engine);
    m_player->move((WORLD_W*32)/2.0f, 32);
    m_player->moveToGround();
    loaded = true;
}

void World::generateWorld(unsigned int seed, const char *name, int biome)
{
    srand(seed);
    sprintf(fileName, "worlds/%s.dat", name);
    loaded = false;

    double pi = 3.141592653589793;
    int heights[] = {58,59,60,61,62,63,64,65,66,67,68,69,70};
    bool aHill[3] = {true, false, false};
    int randomDirtLevel[] = {2,3,3,3};
    int startingHeight = heights[rand() % 13]*32;
    int alternateHeight = startingHeight;
    int dirtLevel, stoneLevel, altDirtLvl;
    int tree = rand() % 2;
    bool hill = aHill[rand() % 3];

    bool mixed_biome = biome == 3;
    biome = (biome == -1 or mixed_biome) ? rand()%3 : biome;

    printf("biome selected: %d\n", biome);

    for(int xx=0; xx<WORLD_W*32; xx+=32)
    {
        if (biome == 0) // PLAINS //
        {
            dirtLevel = alternateHeight + 32*randomDirtLevel[rand() % 4];
            stoneLevel = WORLD_H*32;

            setBlock(xx/32, alternateHeight/32, BLOCK_GRASS);

            // dirt level
            for(int yy=alternateHeight; yy<dirtLevel; yy+=32)
            {
                setBlock(xx/32, yy/32+1, BLOCK_DIRT);
                altDirtLvl = yy;
            }

            for(int yy=altDirtLvl; yy<altDirtLvl+64; yy+=32)
            {
                int u = rand() % 4;
                if (u == 1)
                    setBlock(xx/32, yy/32+2, BLOCK_STONE);
                else
                    setBlock(xx/32, yy/32+2, BLOCK_DIRT);
            }

            //stoneLevel
            for(int yy=altDirtLvl+64; yy<stoneLevel; yy+=32)
            {
                if (yy/32+2 < WORLD_H-2)
                    setBlock(xx/32, yy/32+2, BLOCK_STONE);
                else
                    setBlock(xx/32, yy/32+2, BLOCK_BEDROCK);
            }

            int amp = rand() % 13;
            alternateHeight += int((amp*sin((2*pi)/1728*(xx-864))));
        }

        else if (biome == 1) // PLAINS WITH TREES //
        {
            setBlock(xx/32, alternateHeight/32, BLOCK_GRASS);

            dirtLevel = alternateHeight + 32*randomDirtLevel[rand() % 4];
            stoneLevel = WORLD_H*32;
            int waterLevel = (WORLD_H-32)*32;
            int cc[3] = {1,1,2};
            if (alternateHeight <= waterLevel)
                tree++;

            // trees
            if (tree >= 9 and alternateHeight <= waterLevel)
            {
                int c = cc[rand() % 3];
                if (c == 1) // put the tree
                {
                    int tree_y = alternateHeight-32;
                    int tree_height = (rand() % 4) + 2;
                    for (int aY=tree_y; aY >= tree_y-(tree_height*32); aY-=32)
                        setBlock(xx/32, aY/32, BLOCK_OAK_WOOD, LAYER_NONSOLID);
                    for (int aY = tree_y-(tree_height*32)-32; aY <= tree_y-(tree_height*32)+32; aY+=32)
                    {
                        for (int aX=-2; aX<=2; aX++)
                        {
                            if (aY == tree_y-(tree_height*32)-32 and (aX == -2 or aX == 2))
                                continue;
                            if (getBlock(xx/32+aX, aY/32) != BLOCK_OAK_WOOD)
                                setBlock(xx/32+aX, aY/32, BLOCK_OAK_LEAVES, LAYER_NONSOLID);
                        }
                    }
                }
                int aTree[7] = {1,2,2,3,3,3,4};
                tree = aTree[rand() % 7];
            }

            // dirt level
            for(int yy=alternateHeight; yy<dirtLevel; yy+=32)
            {
                setBlock(xx/32, yy/32+1, BLOCK_DIRT);
                altDirtLvl = yy;
            }

            for(int yy=altDirtLvl; yy<altDirtLvl+64; yy+=32)
            {
                int u = rand() % 4;
                if (u == 1)
                    setBlock(xx/32, yy/32+2, BLOCK_STONE);
                else
                    setBlock(xx/32, yy/32+2, BLOCK_DIRT);
            }

            //stoneLevel
            for(int yy=altDirtLvl+64; yy<stoneLevel; yy+=32)
            {
                if (yy/32+2 < WORLD_H-2)
                    setBlock(xx/32, yy/32+2, BLOCK_STONE);
                else
                    setBlock(xx/32, yy/32+2, BLOCK_BEDROCK);
            }

            int amp;
            if (hill)
                amp = 16;
            else
                amp = 10 + (rand() % 4);

            int aChoose[3] = {54,108,240};
            alternateHeight += amp*sin((2*pi)/1728*(xx-864))+((8+(rand() % 8))*sin((2*pi)/(aChoose[rand()%3])*xx))+((15+(rand()%2))*sin((2*pi/240)*xx));
        }

        else if (biome == 2) // DESERT //
        {
            setBlock(xx/32, alternateHeight/32, BLOCK_SAND);

            int cc[3] = {1,1,2};
            tree += cc[rand() % 3];
            if (tree >= 14 and tree <= 24)
            {
                int p = cc[rand() % 3];
                if (p == 1)
                {
                    int cacti_y = alternateHeight-32;
                    int cacti_height = (rand() % 3) + 1;
                    for (int aY = cacti_y; aY >= cacti_y-(cacti_height*32); aY-=32)
                        setBlock(xx/32, aY/32, BLOCK_CACTUS, LAYER_NONSOLID);
                }
                tree = rand() % 6;
            }
            int d = rand() % 10;
            if (d == 1)
            {
                // xx/32, alternateHeight/32-1, deadbush
            }

            dirtLevel = alternateHeight + 32*randomDirtLevel[rand() % 4];
            stoneLevel = WORLD_H*32;

            // sand level
            for(int yy=alternateHeight; yy<dirtLevel; yy+=32)
            {
                setBlock(xx/32, yy/32+1, BLOCK_SAND);
                altDirtLvl = yy;
            }

            int altLevels[5] = {1,2,2,2,3};
            int sandStoneLvl = altDirtLvl + 32*altLevels[rand() % 5];

            for(int yy=altDirtLvl; yy<sandStoneLvl; yy+=32)
                setBlock(xx/32, yy/32+2, BLOCK_SANDSTONE);

            //stoneLevel
            for(int yy=sandStoneLvl; yy<stoneLevel; yy+=32)
            {
                if (yy/32+2 < WORLD_H-2)
                    setBlock(xx/32, yy/32+2, BLOCK_STONE);
                else
                    setBlock(xx/32, yy/32+2, BLOCK_BEDROCK);
            }

            int amp;
            if (hill)
                amp = 16;
            else
                amp = 10 + (rand() % 4);

            int aChoose[3] = {54,108,240};
            alternateHeight += amp*sin((2*pi)/1728*(xx-864))+((8+(rand() % 8))*sin((2*pi)/(aChoose[rand()%3])*xx))+((15+(rand()%2))*sin((2*pi/240)*xx));
        }

        if (mixed_biome and rand() % 1000 >= 980)
            biome = rand()%3;
    }

    saveWorld();
    printf("lighting up the world...\n");
    for(int i=0; i<WORLD_W*WORLD_H*4; i++)
    {
        int x = i % WORLD_W;
        int y = i / WORLD_W;
        updateLighting(x, y);
    }
    printf("completed!\n");
    m_player = new Player(this, m_engine);
    m_player->move((WORLD_W*32)/2.0f, 32);
    m_player->moveToGround();
    loaded = true;
}

void World::saveWorld()
{
    printf("opening file\n");
    std::fstream file(fileName, std::ios::out | std::ios::binary);

    int width = WORLD_W, height = WORLD_H;
    file.write((char*)&width, sizeof(width));
    file.write((char*)&height, sizeof(height));
    for(int i=0; i<width*height*4; i++)
    {
        int x = i % width;
        int y = i / width;
        if (x < 0 or x >= width or y < 0 or y >= height) continue;
        int block = getBlock(x,y);
        int layer = getBlockLayer(x,y);

        file.write((char*)&x, sizeof(x));
        file.write((char*)&y, sizeof(x));
        file.write((char*)&block, sizeof(block));
        file.write((char*)&layer, sizeof(layer));
    }
    file.close();
    printf("world saved to %s\n", fileName);
}

void World::loadWorld(const char *worldName)
{
    sprintf(fileName, "worlds/%s.dat", worldName);
    std::fstream file(fileName, std::ios::in | std::ios::binary);
    loaded = false;

    m_blocks2.clear();

    printf("loading world %s...\n", fileName);

    int width, height, pos_x, pos_y, block_type, block_layer;

    file.read((char*)&width, sizeof(width));
    file.read((char*)&height, sizeof(height));
    printf("world size: %d,%d\n", width, height);

    for(int yy=0; yy < height/CHUNK_H+1; yy++)
    {
        m_blocks2.push_back(std::vector<Chunk>());
        for (int xx=0; xx < width/CHUNK_W+1; xx++)
        {
            m_blocks2.back().push_back(Chunk(m_engine));
        }
    }

    for(int i=0; i<width*height*4; i++)
    {
        //printf("read %d\n", i);
        file.read((char*)&pos_x, sizeof(pos_x));
        file.read((char*)&pos_y, sizeof(pos_y));
        file.read((char*)&block_type, sizeof(block_type));
        file.read((char*)&block_layer, sizeof(block_layer));

        setBlock(pos_x, pos_y, block_type, block_layer);
    }

    printf("lighting up the world...\n");
    for(int i=0; i<width*height*4; i++)
    {
        int x = i % width;
        int y = i / width;
        updateLighting(x, y);
    }

    printf("world loaded!\n");
    m_player = new Player(this, m_engine);
    m_player->move((WORLD_W*32)/2.0f, 32);
    m_player->moveToGround();
    loaded = true;
}
