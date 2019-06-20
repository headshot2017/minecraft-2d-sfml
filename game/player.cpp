#define _USE_MATH_DEFINES

#include "player.h"
#include <math.h>
#include <fstream>

Player::Player()
{
    x = y = hspeed = vspeed = gravity = new_x = new_y = x_acc = m_angle = 0.0f;
}

Player::Player(World& world, GameEngine *engine)
{
    hspeed = vspeed = gravity = new_x = new_y = x_acc = m_angle = 0.0f;
    m_world = world;
    m_engine = engine;

    m_skinvertex.resize(6*4);
    m_skinvertex.setPrimitiveType(sf::Quads);
    move((WORLD_W*32)/1.25, 32.0f);
    moveToGround();
    setSkin("steve");
}

Player::~Player()
{

}

void Player::setSkin(const char *name)
{
    char aBuf[96];
    sprintf(aBuf, "data/skins/%s.png", name);
    std::fstream file(aBuf);

    if (not file.good())
    {
        file.close();
        setSkin("steve");
        return;
    }
    file.close();

    m_skin.loadFromFile(aBuf);
}

void Player::moveToGround()
{
    int yy = y/32;

    if (blockCollide(x/32, yy))
    {
        while (blockCollide(x/32, yy))
        {
            y-=1.f;
            yy = y/32;
        }
    }
    else
    {
        while (not blockCollide(x/32, yy))
        {
            y+=1.f;
            yy = y/32;
        }
    }

    //y -= 1.f;
}

bool Player::blockCollide(int x, int y)
{
    return m_world.getBlock(x, y) != BLOCK_AIR;
}

void Player::adjustSkinDir()
{
    sf::Transform head;
    if (m_isPlayer)
        m_angle = atan2(mousepos.y - y, mousepos.x - x) * 180 / M_PI;
    head.rotate((m_dir > 0) ? m_angle : m_angle+180, x, y-48);

    sf:: Transform arm1, arm2, leg1, leg2;
    float armswing = (hspeed > 5) ? 5 : hspeed;
    float maxangle = armswing*9.0f;
    float angle = sin((m_footstepticks/60.0f)) * maxangle;
    arm1.rotate(angle + (((m_armtick/10.0f)*(6.0f))*-m_dir), x-4, y-48);
    arm2.rotate(-angle, x+4, y-48);
    leg1.rotate(angle, x, y-24);
    leg2.rotate(-angle, x, y-24);

    if (m_dir > 0)
    {
        // texture coordinates
        m_skinvertex[PLAYER_HEAD+0].texCoords = sf::Vector2f(0, 8);
        m_skinvertex[PLAYER_HEAD+1].texCoords = sf::Vector2f(8, 8);
        m_skinvertex[PLAYER_HEAD+2].texCoords = sf::Vector2f(8, 16);
        m_skinvertex[PLAYER_HEAD+3].texCoords = sf::Vector2f(0, 16);

        m_skinvertex[PLAYER_ARMBACK+0].texCoords = sf::Vector2f(40, 20);
        m_skinvertex[PLAYER_ARMBACK+1].texCoords = sf::Vector2f(44, 20);
        m_skinvertex[PLAYER_ARMBACK+2].texCoords = sf::Vector2f(44, 32);
        m_skinvertex[PLAYER_ARMBACK+3].texCoords = sf::Vector2f(40, 32);

        m_skinvertex[PLAYER_BODY+0].texCoords = sf::Vector2f(16, 20);
        m_skinvertex[PLAYER_BODY+1].texCoords = sf::Vector2f(20, 20);
        m_skinvertex[PLAYER_BODY+2].texCoords = sf::Vector2f(20, 32);
        m_skinvertex[PLAYER_BODY+3].texCoords = sf::Vector2f(16, 32);

        m_skinvertex[PLAYER_ARMFRONT+0].texCoords = sf::Vector2f(40, 20);
        m_skinvertex[PLAYER_ARMFRONT+1].texCoords = sf::Vector2f(44, 20);
        m_skinvertex[PLAYER_ARMFRONT+2].texCoords = sf::Vector2f(44, 32);
        m_skinvertex[PLAYER_ARMFRONT+3].texCoords = sf::Vector2f(40, 32);

        m_skinvertex[PLAYER_LEGBACK+0].texCoords = sf::Vector2f(0, 20);
        m_skinvertex[PLAYER_LEGBACK+1].texCoords = sf::Vector2f(4, 20);
        m_skinvertex[PLAYER_LEGBACK+2].texCoords = sf::Vector2f(4, 32);
        m_skinvertex[PLAYER_LEGBACK+3].texCoords = sf::Vector2f(0, 32);

        m_skinvertex[PLAYER_LEGFRONT+0].texCoords = sf::Vector2f(0, 20);
        m_skinvertex[PLAYER_LEGFRONT+1].texCoords = sf::Vector2f(4, 20);
        m_skinvertex[PLAYER_LEGFRONT+2].texCoords = sf::Vector2f(4, 32);
        m_skinvertex[PLAYER_LEGFRONT+3].texCoords = sf::Vector2f(0, 32);


        // positions
        m_skinvertex[PLAYER_LEGFRONT+0].position = leg1.transformPoint(sf::Vector2f(x-4, y-24));
        m_skinvertex[PLAYER_LEGFRONT+1].position = leg1.transformPoint(sf::Vector2f(x+4, y-24));
        m_skinvertex[PLAYER_LEGFRONT+2].position = leg1.transformPoint(sf::Vector2f(x+4, y));
        m_skinvertex[PLAYER_LEGFRONT+3].position = leg1.transformPoint(sf::Vector2f(x-4, y));

        m_skinvertex[PLAYER_LEGBACK+0].position = leg2.transformPoint(sf::Vector2f(x-4, y-24));
        m_skinvertex[PLAYER_LEGBACK+1].position = leg2.transformPoint(sf::Vector2f(x+4, y-24));
        m_skinvertex[PLAYER_LEGBACK+2].position = leg2.transformPoint(sf::Vector2f(x+4, y));
        m_skinvertex[PLAYER_LEGBACK+3].position = leg2.transformPoint(sf::Vector2f(x-4, y));

        m_skinvertex[PLAYER_ARMFRONT+0].position = arm1.transformPoint(sf::Vector2f(x-6, y-48));
        m_skinvertex[PLAYER_ARMFRONT+1].position = arm1.transformPoint(sf::Vector2f(x+2, y-48));
        m_skinvertex[PLAYER_ARMFRONT+2].position = arm1.transformPoint(sf::Vector2f(x+2, y-24));
        m_skinvertex[PLAYER_ARMFRONT+3].position = arm1.transformPoint(sf::Vector2f(x-6, y-24));

        m_skinvertex[PLAYER_ARMBACK+0].position = arm2.transformPoint(sf::Vector2f(x-2, y-48));
        m_skinvertex[PLAYER_ARMBACK+1].position = arm2.transformPoint(sf::Vector2f(x+6, y-48));
        m_skinvertex[PLAYER_ARMBACK+2].position = arm2.transformPoint(sf::Vector2f(x+6, y-24));
        m_skinvertex[PLAYER_ARMBACK+3].position = arm2.transformPoint(sf::Vector2f(x-2, y-24));

        m_skinvertex[PLAYER_BODY+0].position = sf::Vector2f(x-4, y-48);
        m_skinvertex[PLAYER_BODY+1].position = sf::Vector2f(x+4, y-48);
        m_skinvertex[PLAYER_BODY+2].position = sf::Vector2f(x+4, y-24);
        m_skinvertex[PLAYER_BODY+3].position = sf::Vector2f(x-4, y-24);

        m_skinvertex[PLAYER_HEAD+0].position = head.transformPoint(sf::Vector2f(x-8, y-64));
        m_skinvertex[PLAYER_HEAD+1].position = head.transformPoint(sf::Vector2f(x+8, y-64));
        m_skinvertex[PLAYER_HEAD+2].position = head.transformPoint(sf::Vector2f(x+8, y-48));
        m_skinvertex[PLAYER_HEAD+3].position = head.transformPoint(sf::Vector2f(x-8, y-48));
    }

    else if (m_dir < 0)
    {
        // texture coordinates
        m_skinvertex[PLAYER_HEAD+0].texCoords = sf::Vector2f(16, 8);
        m_skinvertex[PLAYER_HEAD+1].texCoords = sf::Vector2f(24, 8);
        m_skinvertex[PLAYER_HEAD+2].texCoords = sf::Vector2f(24, 16);
        m_skinvertex[PLAYER_HEAD+3].texCoords = sf::Vector2f(16, 16);

        m_skinvertex[PLAYER_ARMBACK+0].texCoords = sf::Vector2f(48, 20);
        m_skinvertex[PLAYER_ARMBACK+1].texCoords = sf::Vector2f(52, 20);
        m_skinvertex[PLAYER_ARMBACK+2].texCoords = sf::Vector2f(52, 32);
        m_skinvertex[PLAYER_ARMBACK+3].texCoords = sf::Vector2f(48, 32);

        m_skinvertex[PLAYER_BODY+0].texCoords = sf::Vector2f(28, 20);
        m_skinvertex[PLAYER_BODY+1].texCoords = sf::Vector2f(32, 20);
        m_skinvertex[PLAYER_BODY+2].texCoords = sf::Vector2f(32, 32);
        m_skinvertex[PLAYER_BODY+3].texCoords = sf::Vector2f(28, 32);

        m_skinvertex[PLAYER_ARMFRONT+0].texCoords = sf::Vector2f(48, 20);
        m_skinvertex[PLAYER_ARMFRONT+1].texCoords = sf::Vector2f(52, 20);
        m_skinvertex[PLAYER_ARMFRONT+2].texCoords = sf::Vector2f(52, 32);
        m_skinvertex[PLAYER_ARMFRONT+3].texCoords = sf::Vector2f(48, 32);

        m_skinvertex[PLAYER_LEGBACK+0].texCoords = sf::Vector2f(8, 20);
        m_skinvertex[PLAYER_LEGBACK+1].texCoords = sf::Vector2f(12, 20);
        m_skinvertex[PLAYER_LEGBACK+2].texCoords = sf::Vector2f(12, 32);
        m_skinvertex[PLAYER_LEGBACK+3].texCoords = sf::Vector2f(8, 32);

        m_skinvertex[PLAYER_LEGFRONT+0].texCoords = sf::Vector2f(8, 20);
        m_skinvertex[PLAYER_LEGFRONT+1].texCoords = sf::Vector2f(12, 20);
        m_skinvertex[PLAYER_LEGFRONT+2].texCoords = sf::Vector2f(12, 32);
        m_skinvertex[PLAYER_LEGFRONT+3].texCoords = sf::Vector2f(8, 32);


        // positions
        m_skinvertex[PLAYER_LEGFRONT+0].position = leg1.transformPoint(sf::Vector2f(x-4, y-24));
        m_skinvertex[PLAYER_LEGFRONT+1].position = leg1.transformPoint(sf::Vector2f(x+4, y-24));
        m_skinvertex[PLAYER_LEGFRONT+2].position = leg1.transformPoint(sf::Vector2f(x+4, y));
        m_skinvertex[PLAYER_LEGFRONT+3].position = leg1.transformPoint(sf::Vector2f(x-4, y));

        m_skinvertex[PLAYER_LEGBACK+0].position = leg2.transformPoint(sf::Vector2f(x-4, y-24));
        m_skinvertex[PLAYER_LEGBACK+1].position = leg2.transformPoint(sf::Vector2f(x+4, y-24));
        m_skinvertex[PLAYER_LEGBACK+2].position = leg2.transformPoint(sf::Vector2f(x+4, y));
        m_skinvertex[PLAYER_LEGBACK+3].position = leg2.transformPoint(sf::Vector2f(x-4, y));

        m_skinvertex[PLAYER_ARMFRONT+0].position = arm2.transformPoint(sf::Vector2f(x-2, y-48));
        m_skinvertex[PLAYER_ARMFRONT+1].position = arm2.transformPoint(sf::Vector2f(x+6, y-48));
        m_skinvertex[PLAYER_ARMFRONT+2].position = arm2.transformPoint(sf::Vector2f(x+6, y-24));
        m_skinvertex[PLAYER_ARMFRONT+3].position = arm2.transformPoint(sf::Vector2f(x-2, y-24));

        m_skinvertex[PLAYER_ARMBACK+0].position = arm1.transformPoint(sf::Vector2f(x-6, y-48));
        m_skinvertex[PLAYER_ARMBACK+1].position = arm1.transformPoint(sf::Vector2f(x+2, y-48));
        m_skinvertex[PLAYER_ARMBACK+2].position = arm1.transformPoint(sf::Vector2f(x+2, y-24));
        m_skinvertex[PLAYER_ARMBACK+3].position = arm1.transformPoint(sf::Vector2f(x-6, y-24));

        m_skinvertex[PLAYER_BODY+0].position = sf::Vector2f(x-4, y-48);
        m_skinvertex[PLAYER_BODY+1].position = sf::Vector2f(x+4, y-48);
        m_skinvertex[PLAYER_BODY+2].position = sf::Vector2f(x+4, y-24);
        m_skinvertex[PLAYER_BODY+3].position = sf::Vector2f(x-4, y-24);

        m_skinvertex[PLAYER_HEAD+0].position = head.transformPoint(sf::Vector2f(x-8, y-64));
        m_skinvertex[PLAYER_HEAD+1].position = head.transformPoint(sf::Vector2f(x+8, y-64));
        m_skinvertex[PLAYER_HEAD+2].position = head.transformPoint(sf::Vector2f(x+8, y-48));
        m_skinvertex[PLAYER_HEAD+3].position = head.transformPoint(sf::Vector2f(x-8, y-48));
    }
}

void Player::placeBlock(int x, int y, int block)
{
    m_armtick = 150;
}

void Player::update(GameEngine *engine)
{
    if (blockCollide(x/32, (y+vspeed)/32) or blockCollide(x/32, (y+vspeed-64)/32)) // gravity.
    {
        if (vspeed > 0)
        {
            moveToGround();
            gravity = 0.f;
            vspeed = 0.f;
        }
        else if (vspeed < 0)
            if (not sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                vspeed = 0.f;
    }
    else
        gravity = 0.25f;

    hspeed += x_acc;
    vspeed += gravity;

    if (hspeed != 0)
    {
        if (blockCollide((x+hspeed+(4*m_dir))/32, (y-32)/32) or
            blockCollide((x+hspeed+(-4*m_dir))/32, (y-32)/32) or
            blockCollide((x+hspeed+(4*m_dir))/32, (y-64)/32) or
            blockCollide((x+hspeed+(-4*m_dir))/32, (y-64)/32)) // horizontal collisions
            hspeed = x_acc = 0;
    }

    x += hspeed;
    y += vspeed;

    if (m_isPlayer)
    {
        mousepos = engine->app.mapPixelToCoords(sf::Mouse::getPosition(engine->app), engine->app.getView());
        mousepos.y += 56.0f;
        if (mousepos.x >= x)
            m_dir = 1;
        else
            m_dir = -1;
    }

    if (hspeed != 0)
    {
        m_ticks++;
        float armswing = abs(hspeed);
        if (hspeed > 5)
            armswing = 5;
        m_footstepticks += (armswing*2.5f);

        float maxangle = armswing*9.0f;
        float angle = sin((m_footstepticks/60.0f)) * maxangle;
        if (angle < 6 and angle > -6 and not m_footstepwait and blockCollide(x/32, y/32))
        {
            int block = m_world.getBlock(x/32, y/32);
            sf::Vector2f view = engine->app.getView().getCenter();
            engine->Sound()->playFootstepSound(x, y, view.x, view.y, block);
            m_footstepwait = 25 - (armswing*2);
        }

        if (m_footstepwait) m_footstepwait--;
    }

    if (m_armtick)
        m_armtick-=10;
}

void Player::process_input(GameEngine *engine)
{
    if (not m_isPlayer or not can_move) return;

    // mouse
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right) and not rmb)
    {
        placeBlock(0, 0, 0);
        rmb = true;
    }
    else if (not sf::Mouse::isButtonPressed(sf::Mouse::Right) and rmb)
        rmb = false;

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        if (not lmb_tick)
        {
            placeBlock(0, 0, 0);
            lmb_tick = 60 * 0.22;
        }
        else
            lmb_tick--;
        lmb = true;
    }
    else
    {
        lmb = false;
        lmb_tick = 0;
    }

    // keyboard
    int spd = (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) ? 4 : 3;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        x_acc = (hspeed > 0) ? -0.5f : -0.25f;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        x_acc = (hspeed < 0) ? 0.5f : 0.25f;
    else
    {
        if (hspeed > 0)
            x_acc = -0.25f;
        else if (hspeed < 0)
            x_acc = +0.25f;
        else
            x_acc = 0.f;
    }
    if (hspeed > spd)
        hspeed = spd;
    if (hspeed < -spd)
        hspeed = -spd;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) and blockCollide(x/32, y/32))
        vspeed = -4.5f;
}

void Player::draw(GameEngine *engine)
{
    adjustSkinDir();

    sf::RenderStates state;
    state.texture = &m_skin;

    engine->app.draw(m_skinvertex, state);
}