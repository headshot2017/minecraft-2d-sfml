#ifndef TNT_H_INCLUDED
#define TNT_H_INCLUDED

#include "falling_block.h"
#include <SFML/Graphics.hpp>

class Dynamite : public FallingBlock
{
public:
    Dynamite() {}
    Dynamite(World *world, GameEngine *engine, int xx, int yy, int startingticks=60*5);

    void update();
    void draw();

private:
    bool m_whiteFlash;
};

#endif // TNT_H_INCLUDED
