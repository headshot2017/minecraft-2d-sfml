#include "world.h"
#include "entities/falling_block.h"
#include "entities/tnt.h"
#include "entities/particles/explosion.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <zlib.h>

World::World()
{
    loaded = false;
    m_blocks2.clear();
    m_entities.clear();
    m_seed = 0;
}

World::World(GameEngine *engine)
{
    loaded = false;
    m_engine = engine;
    m_blocks2.clear();
    m_entities.clear();
    m_seed = 0;
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

void World::setBlock(uint32_t x, uint32_t y, int block, sf::Uint8 layer)
{
    uint32_t x_ind = x/CHUNK_W;
    uint32_t y_ind = y/CHUNK_H;
    uint32_t x_block_chunk = x % CHUNK_W;
    uint32_t y_block_chunk = y % CHUNK_H;
    uint32_t ind = (y_block_chunk * CHUNK_W + x_block_chunk)*4;

    if (x >= m_width or y >= m_height) return;

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
        if (x < m_width-1)
            updateLighting(x+1, y);
        if (y > 0)
            updateLighting(x, y-1);
        if (y < m_height-1)
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
            if (y < m_height-1)
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

int World::getBlock(uint32_t x, uint32_t y)
{
    if (x >= m_width) x = m_width-1;
    if (y >= m_height) y = m_height-1;

    uint32_t x_ind = x/CHUNK_W;
    uint32_t y_ind = y/CHUNK_H;
    uint32_t x_block_chunk = x % CHUNK_W;
    uint32_t y_block_chunk = y % CHUNK_H;

    int block = m_blocks2[y_ind][x_ind].getBlock(x_block_chunk, y_block_chunk);
    return block;
}

int World::getBlockFlags(uint32_t x, uint32_t y)
{
    if (x >= m_width) x = m_width-1;
    if (y >= m_height) y = m_height-1;

    uint32_t x_ind = x/CHUNK_W;
    uint32_t y_ind = y/CHUNK_H;
    uint32_t x_block_chunk = x % CHUNK_W;
    uint32_t y_block_chunk = y % CHUNK_H;

    int block = m_blocks2[y_ind][x_ind].getBlockFlags(x_block_chunk, y_block_chunk);
    return block;
}

sf::Uint8 World::getBlockLayer(uint32_t x, uint32_t y)
{
    if (x >= m_width) x = m_width-1;
    if (y >= m_height) y = m_height-1;

    uint32_t x_ind = x/CHUNK_W;
    uint32_t y_ind = y/CHUNK_H;
    uint32_t x_block_chunk = x % CHUNK_W;
    uint32_t y_block_chunk = y % CHUNK_H;

    sf::Uint8 block = m_blocks2[y_ind][x_ind].getBlockLayer(x_block_chunk, y_block_chunk);
    return block;
}

void World::updateLighting(uint32_t x, uint32_t y)
{
    int block = getBlock(x, y);
    sf::Uint8 layer = getBlockLayer(x, y);
    uint32_t x_block_chunk = x % CHUNK_W;
    uint32_t y_block_chunk = y % CHUNK_H;
    uint32_t ind = (y_block_chunk * CHUNK_W + x_block_chunk)*4;
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

sf::VertexArray& World::getBlocksFromPoint(uint32_t x, uint32_t y, bool front)
{
    uint32_t x_ind = x/CHUNK_W;
    uint32_t y_ind = y/CHUNK_H;

    if (x_ind > m_width/CHUNK_W) x_ind = m_width/CHUNK_W;
    if (y_ind > m_height/CHUNK_H) y_ind = m_height/CHUNK_H;
    //if (x_ind < 0) x_ind = 0;
    //if (y_ind < 0) y_ind = 0;

    return (not front) ? m_blocks2[y_ind][x_ind].getVertex() : m_blocks2[y_ind][x_ind].getVertexFront();
}

void World::generateFlatWorld(const char *name, const std::vector<int>& blocks, uint32_t width, uint32_t height)
{
    sprintf(fileName, "worlds/%s.dat", name);
    loaded = false;
    m_seed = 0;
    m_width = width;
    m_height = height;

    printf("%u blocks for flatworld\n", blocks.size());

    for(uint32_t yy=0; yy < m_height/CHUNK_H+1; yy++)
    {
        m_blocks2.push_back(std::vector<Chunk>());
        for (uint32_t xx=0; xx < m_width/CHUNK_W+1; xx++)
        {
            m_blocks2.back().push_back(Chunk(m_engine));
        }
    }

    for(uint32_t xx=0; xx<width; xx++)
    {
        uint32_t yy = height-1;
        for (uint32_t i=0; i<blocks.size(); i++)
        {
            setBlock(xx, yy, blocks[i]);
            yy--;
        }
    }

    saveWorld();
    printf("lighting up the world...\n");
    for(uint32_t i=0; i<width*height*4; i++)
    {
        uint32_t x = i % width;
        uint32_t y = i / width;
        updateLighting(x, y);
    }
    printf("completed!\n");
    m_player = new Player(this, m_engine);
    m_player->move((width*32)/2.0f, 32);
    m_player->moveToGround();
    loaded = true;
}

void World::generateWorld(unsigned int seed, const char *name, int biome, uint32_t width, uint32_t height)
{
    srand(seed);
    m_seed = seed;
    m_width = width;
    m_height = height;
    sprintf(fileName, "worlds/%s.dat", name);
    loaded = false;

    double pi = 3.141592653589793;
    uint32_t heights[] = {58,59,60,61,62,63,64,65,66,67,68,69,70};
    bool aHill[3] = {true, false, false};
    uint32_t randomDirtLevel[] = {2,3,3,3};
    uint32_t startingHeight = heights[rand() % 13]*32;
    uint32_t alternateHeight = startingHeight;
    uint32_t dirtLevel, stoneLevel, altDirtLvl;
    int tree = rand() % 2;
    bool hill = aHill[rand() % 3];

    bool mixed_biome = biome == 3;
    biome = (biome == -1 or mixed_biome) ? rand()%3 : biome;

    printf("biome selected: %d\n", biome);

    for(uint32_t yy=0; yy < m_height/CHUNK_H+1; yy++)
    {
        m_blocks2.push_back(std::vector<Chunk>());
        for (uint32_t xx=0; xx < m_width/CHUNK_W+1; xx++)
        {
            m_blocks2.back().push_back(Chunk(m_engine));
        }
    }

    for(uint32_t xx=0; xx<width*32; xx+=32)
    {
        if (biome == 0) // PLAINS //
        {
            dirtLevel = alternateHeight + 32*randomDirtLevel[rand() % 4];
            stoneLevel = height*32;

            setBlock(xx/32, alternateHeight/32, BLOCK_GRASS);

            // dirt level
            for(uint32_t yy=alternateHeight; yy<dirtLevel; yy+=32)
            {
                setBlock(xx/32, yy/32+1, BLOCK_DIRT);
                altDirtLvl = yy;
            }

            for(uint32_t yy=altDirtLvl; yy<altDirtLvl+64; yy+=32)
            {
                int u = rand() % 4;
                if (u == 1)
                    setBlock(xx/32, yy/32+2, BLOCK_STONE);
                else
                    setBlock(xx/32, yy/32+2, BLOCK_DIRT);
            }

            //stoneLevel
            for(uint32_t yy=altDirtLvl+64; yy<stoneLevel; yy+=32)
            {
                if (yy/32+2 < m_height-2)
                    setBlock(xx/32, yy/32+2, BLOCK_STONE);
                else
                    setBlock(xx/32, yy/32+2, BLOCK_BEDROCK);
            }

            int amp = rand() % 13;
            alternateHeight += uint32_t((amp*sin((2*pi)/1728*(xx-864))));
        }

        else if (biome == 1) // PLAINS WITH TREES //
        {
            setBlock(xx/32, alternateHeight/32, BLOCK_GRASS);

            dirtLevel = alternateHeight + 32*randomDirtLevel[rand() % 4];
            stoneLevel = height*32;
            uint32_t waterLevel = (height-32)*32;
            uint32_t cc[3] = {1,1,2};
            if (alternateHeight <= waterLevel)
                tree++;

            // trees
            if (tree >= 9 and alternateHeight <= waterLevel)
            {
                uint32_t c = cc[rand() % 3];
                if (c == 1) // put the tree
                {
                    uint32_t tree_y = alternateHeight-32;
                    uint32_t tree_height = (rand() % 4) + 2;
                    for (uint32_t aY=tree_y; aY >= tree_y-(tree_height*32); aY-=32)
                        setBlock(xx/32, aY/32, BLOCK_OAK_WOOD, LAYER_NONSOLID);
                    for (uint32_t aY = tree_y-(tree_height*32)-32; aY <= tree_y-(tree_height*32)+32; aY+=32)
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
                uint32_t aTree[7] = {1,2,2,3,3,3,4};
                tree = aTree[rand() % 7];
            }

            // dirt level
            for(uint32_t yy=alternateHeight; yy<dirtLevel; yy+=32)
            {
                setBlock(xx/32, yy/32+1, BLOCK_DIRT);
                altDirtLvl = yy;
            }

            for(uint32_t yy=altDirtLvl; yy<altDirtLvl+64; yy+=32)
            {
                int u = rand() % 4;
                if (u == 1)
                    setBlock(xx/32, yy/32+2, BLOCK_STONE);
                else
                    setBlock(xx/32, yy/32+2, BLOCK_DIRT);
            }

            //stoneLevel
            for(uint32_t yy=altDirtLvl+64; yy<stoneLevel; yy+=32)
            {
                if (yy/32+2 < height-2)
                    setBlock(xx/32, yy/32+2, BLOCK_STONE);
                else
                    setBlock(xx/32, yy/32+2, BLOCK_BEDROCK);
            }

            uint32_t amp;
            if (hill)
                amp = 16;
            else
                amp = 10 + (rand() % 4);

            uint32_t aChoose[3] = {54,108,240};
            alternateHeight += amp*sin((2*pi)/1728*(xx-864))+((8+(rand() % 8))*sin((2*pi)/(aChoose[rand()%3])*xx))+((15+(rand()%2))*sin((2*pi/240)*xx)); // having a stroke yet?
        }

        else if (biome == 2) // DESERT //
        {
            setBlock(xx/32, alternateHeight/32, BLOCK_SAND);

            uint32_t cc[3] = {1,1,2};
            tree += cc[rand() % 3];
            if (tree >= 14 and tree <= 24)
            {
                uint32_t p = cc[rand() % 3];
                if (p == 1)
                {
                    uint32_t cacti_y = alternateHeight-32;
                    uint32_t cacti_height = (rand() % 3) + 1;
                    for (uint32_t aY = cacti_y; aY >= cacti_y-(cacti_height*32); aY-=32)
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
            stoneLevel = height*32;

            // sand level
            for(uint32_t yy=alternateHeight; yy<dirtLevel; yy+=32)
            {
                setBlock(xx/32, yy/32+1, BLOCK_SAND);
                altDirtLvl = yy;
            }

            uint32_t altLevels[5] = {1,2,2,2,3};
            uint32_t sandStoneLvl = altDirtLvl + 32*altLevels[rand() % 5];

            for(uint32_t yy=altDirtLvl; yy<sandStoneLvl; yy+=32)
                setBlock(xx/32, yy/32+2, BLOCK_SANDSTONE);

            //stoneLevel
            for(uint32_t yy=sandStoneLvl; yy<stoneLevel; yy+=32)
            {
                if (yy/32+2 < height-2)
                    setBlock(xx/32, yy/32+2, BLOCK_STONE);
                else
                    setBlock(xx/32, yy/32+2, BLOCK_BEDROCK);
            }

            uint32_t amp;
            if (hill)
                amp = 16;
            else
                amp = 10 + (rand() % 4);

            uint32_t aChoose[3] = {54,108,240};
            alternateHeight += amp*sin((2*pi)/1728*(xx-864))+((8+(rand() % 8))*sin((2*pi)/(aChoose[rand()%3])*xx))+((15+(rand()%2))*sin((2*pi/240)*xx));
        }

        if (mixed_biome and rand() % 1000 >= 980)
            biome = rand()%3;
    }

    saveWorld();
    printf("lighting up the world...\n");
    for(uint32_t i=0; i<width*height*4; i++)
    {
        uint32_t x = i % width;
        uint32_t y = i / width;
        updateLighting(x, y);
    }
    printf("completed!\n");
    m_player = new Player(this, m_engine);
    m_player->move((width*32)/2.0f, 32);
    m_player->moveToGround();
    loaded = true;
}

void World::saveWorld()
{
    unsigned char *buf = new unsigned char[m_width*m_height * ((sizeof(uint32_t)*2) + sizeof(int) + sizeof(sf::Uint8))];

    unsigned long bufpos = 0;
    printf("saving blocks to buf\n");
    for(uint32_t i=0; i<m_width*m_height; i++)
    {
        uint32_t x = i % m_width;
        uint32_t y = i / m_width;
        int block = getBlock(x,y);
        sf::Uint8 layer = getBlockLayer(x,y);
        if (x >= m_width || y >= m_height || block == BLOCK_AIR) continue;

        memcpy(&buf[bufpos], &x, sizeof(x));            bufpos += sizeof(x);
        memcpy(&buf[bufpos], &y, sizeof(y));            bufpos += sizeof(y);
        memcpy(&buf[bufpos], &block, sizeof(block));    bufpos += sizeof(block);
        memcpy(&buf[bufpos], &layer, sizeof(layer));    bufpos += sizeof(layer);
    }

    unsigned long compressedSize = compressBound(bufpos);
    unsigned char *compressed = new unsigned char[compressedSize];

    compress(compressed, &compressedSize, buf, bufpos);
    printf("compressed world size: %lu\n", compressedSize);

    printf("opening file %s\n", fileName);
    FILE* file = fopen(fileName, "wb");
    fwrite((char*)&m_width, 1, sizeof(m_width), file);
    fwrite((char*)&m_height, 1, sizeof(m_height), file);
    fwrite((char*)&m_seed, 1, sizeof(m_seed), file);
    fwrite(compressed, 1, compressedSize, file);
    fclose(file);

    delete[] buf;
    delete[] compressed;

    printf("world saved to %s\n", fileName);
}

void World::loadWorld(const char *worldName)
{
    sprintf(fileName, "worlds/%s.dat", worldName);
    FILE* file = fopen(fileName, "rb");
    loaded = false;

    m_blocks2.clear();

    printf("loading world %s...\n", fileName);

    fread(&m_width, 1, sizeof(m_width), file);
    fread(&m_height, 1, sizeof(m_height), file);
    fread(&m_seed, 1, sizeof(m_seed), file);
    long continuepos = ftell(file);
    printf("world size: %d,%d\nseed: %d\n", m_width, m_height, m_seed);

    for(uint32_t yy=0; yy < m_height/CHUNK_H+1; yy++)
    {
        m_blocks2.push_back(std::vector<Chunk>());
        for (uint32_t xx=0; xx < m_width/CHUNK_W+1; xx++)
        {
            m_blocks2.back().push_back(Chunk(m_engine));
        }
    }

    fseek(file, 0, SEEK_END); // get compressed size below
    unsigned long compressedSize = ftell(file) - (sizeof(unsigned int)*3); // discount the header size (width,height,seed)
    unsigned char *compressedBuf = new unsigned char[compressedSize];
    fseek(file, continuepos, SEEK_SET); // go back to pos after the header (12, but i like playing it safe)
    fread(compressedBuf, 1, compressedSize, file);
    printf("compressed world size: %lu\n", compressedSize);

    // the uncompressed world data
    unsigned char *buf = new unsigned char[m_width*m_height * ((sizeof(uint32_t)*2) + sizeof(int) + sizeof(unsigned char))];
    unsigned long bufSize = m_width*m_height * ((sizeof(uint32_t)*2) + sizeof(int) + sizeof(unsigned char));
    uncompress(buf, &bufSize, compressedBuf, compressedSize); // bufSize is updated with the real size
    printf("uncompressed world size: %lu\n", bufSize);
    unsigned long bufpos = 0;

    // block data
    uint32_t block_x, block_y;
    int block;
    sf::Uint8 layer;

    for(uint32_t i=0; i<m_width*m_height; i++)
    {
        memcpy(&block_x, &buf[bufpos], sizeof(block_x));    bufpos += sizeof(block_x);
        memcpy(&block_y, &buf[bufpos], sizeof(block_y));    bufpos += sizeof(block_y);
        memcpy(&block, &buf[bufpos], sizeof(block));        bufpos += sizeof(block);
        memcpy(&layer, &buf[bufpos], sizeof(layer));        bufpos += sizeof(layer);

        setBlock(block_x, block_y, block, layer);
    }

    printf("lighting up the world...\n");
    for(uint32_t i=0; i<m_width*m_height; i++)
    {
        uint32_t x = i % m_width;
        uint32_t y = i / m_width;
        updateLighting(x, y);
    }

    fclose(file);
    delete[] compressedBuf;
    delete[] buf;

    m_player = new Player(this, m_engine);
    m_player->move((m_width*32)/2.0f, 32);
    m_player->moveToGround();
    loaded = true;
}
