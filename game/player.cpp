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

    m_skinvertex.resize(6*4);
    m_skinvertex.setPrimitiveType(sf::Quads);
    move((WORLD_W*32)/2, 32.0f);
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

    while (m_world.getBlock(x/32, yy) == BLOCK_AIR)
    {
        y+=1.f;
        yy = y/32;
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
    float angle = sin((m_ticks/60.0f)) * maxangle;
    arm1.rotate(angle, x-4, y-48);
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

void Player::update(GameEngine *engine)
{
    if (blockCollide(x/32, (y+vspeed)/32)) // gravity.
    {
        moveToGround();
        gravity = 0.f;
        if (not sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            vspeed = 0.f;
    }
    else
        gravity = 0.25f;

    hspeed += x_acc;
    vspeed += gravity;

    if (hspeed != 0)
    {
        if (blockCollide((x+hspeed+(4*m_dir))/32, (y-32)/32) or blockCollide((x+hspeed+(4*m_dir))/32, (y-64)/32)) // horizontal collisions
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
        float armswing = (hspeed > 5) ? 5 : hspeed;
        m_ticks += (armswing*2.5f);
    }
}

void Player::process_input(GameEngine *engine)
{
    if (not m_isPlayer or not can_move) return;

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
