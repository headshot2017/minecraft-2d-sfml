#define _USE_MATH_DEFINES

#include "player.h"
#include "entities/tnt.h"
#include <math.h>
#include <fstream>

Player::Player()
{
    x = y = hspeed = vspeed = gravity = new_x = new_y = x_acc = m_angle = 0.0f;
}

Player::Player(World* world, GameEngine *engine)
{
    hspeed = vspeed = gravity = new_x = new_y = x_acc = m_angle = 0.0f;
    m_world = world;
    m_engine = engine;
    m_layer1_collide = false;

    m_skinvertex.resize(7*4);
    m_skinvertex.setPrimitiveType(sf::Quads);
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

    if (blockCollide(x/32, yy) or blockCollide((x-4)/32, yy) or blockCollide((x+4)/32, yy))
    {
        while (blockCollide(x/32, yy) or blockCollide((x-4)/32, yy) or blockCollide((x+4)/32, yy))
        {
            y-=1.f;
            yy = y/32;
        }
    }
    else
    {
        while (not blockCollide(x/32, yy) and not blockCollide((x-4)/32, yy) and not blockCollide((x+4)/32, yy))
        {
            y+=1.f;
            yy = y/32;
        }
    }
}

void Player::moveToRoof()
{
    int yy = (y-56)/32;

    if (blockCollide(x/32, yy) or blockCollide((x-4)/32, yy) or blockCollide((x+4)/32, yy))
    {
        while (blockCollide(x/32, yy) or blockCollide((x-4)/32, yy) or blockCollide((x+4)/32, yy))
        {
            y+=1.f;
            yy = (y-56)/32;
        }
    }
}

bool Player::blockCollide(int x, int y)
{
    return m_world->getBlock(x, y) != BLOCK_AIR and (m_world->getBlockLayer(x, y) == LAYER_BUILD or (m_world->getBlockLayer(x, y) == LAYER_NONSOLID and m_layer1_collide));
}

bool Player::groundCollide()
{
    return (blockCollide(x/32, (y+vspeed)/32) or blockCollide(x/32, (y+vspeed-32)/32) or blockCollide(x/32, (y+vspeed-56)/32) or
            blockCollide((x-4)/32, (y+vspeed)/32) or blockCollide((x-4)/32, (y+vspeed-32)/32) or blockCollide((x-4)/32, (y+vspeed-56)/32) or
            blockCollide((x+4)/32, (y+vspeed)/32) or blockCollide((x+4)/32, (y+vspeed-32)/32) or blockCollide((x+4)/32, (y+vspeed-56)/32));
}

bool Player::canBuild(int xx, int yy)
{
    if ((m_world->getBlock(xx+0, yy+0) or
        m_world->getBlock(xx+1, yy+0) or
        m_world->getBlock(xx-1, yy+0) or
        m_world->getBlock(xx+0, yy+1) or
        m_world->getBlock(xx+0, yy-1))
        and ((xx != floor(x/32) and xx != floor((x+4)/32) and xx != floor((x-4)/32))
             or (yy != floor((y-32)/32) and yy != floor((y-64)/32))))
        return true;
    return false;
}

void Player::adjustSkinDir()
{
    if (m_isPlayer)
        m_angle = atan2(mousepos.y - y, mousepos.x - x) * 180 / M_PI;
}

void Player::placeBlock(int xx, int yy, int block, int layer)
{
    int block2 = m_world->getBlock(xx, yy);
    if (block2 != BLOCK_AIR) return;

    sf::Vector2f view = m_engine->m_window.getView().getCenter();

    m_armtick = 150;
    m_world->setBlock(xx, yy, block, layer);
    m_engine->Sound()->playDigSound(xx*32, yy*32, view.x, view.y, block);
}

void Player::destroyBlock(int xx, int yy)
{
    int block2 = m_world->getBlock(xx, yy);

    m_armtick = 150;
    if (block2 == BLOCK_AIR) return;

    sf::Vector2f view = m_engine->m_window.getView().getCenter();
    if (block2 != BLOCK_GLASS and block2 != BLOCK_GLOWSTONE and block2 != BLOCK_ICE)
        m_engine->Sound()->playDigSound(xx*32, yy*32, view.x, view.y, block2);
    else
        m_engine->Sound()->playGlassBreakSound(xx*32, yy*32, view.x, view.y);
    m_world->setBlock(xx, yy, BLOCK_AIR);
}

void Player::interactBlock(int xx, int yy)
{
    int block2 = m_world->getBlock(xx, yy);

    m_armtick = 150;
    if (block2 == BLOCK_AIR or m_sneak) return;

    if (block2 == BLOCK_TNT)
    {
        m_world->setBlock(xx, yy, BLOCK_AIR);

        sf::Vector2f view = m_engine->m_window.getView().getCenter();
        m_engine->Sound()->playSample(xx*32, yy*32, view.x, view.y, SAMPLE_TNT_FUSE, 0);

        Entity *tnt = new Dynamite(m_world, m_engine, xx, yy);
        m_world->addEntity(tnt);
    }
}

void Player::knockBack(float xx, float yy, int maxdist)
{
    float dist;
    dist = sqrt(pow(xx - x, 2) + pow(yy - y, 2));
    if (dist < maxdist)
    {
        int force = abs(maxdist-dist)/4;
        float angle = atan2(y - yy, x - xx);
        hspeed += round(force * cos(angle));
        vspeed += round(force * sin(angle));
    }
}

void Player::update(GameEngine *engine)
{
    if (groundCollide()) // gravity.
    {
        if (vspeed > 0)
        {
            moveToGround();
            gravity = 0.f;
            vspeed = 0.f;
        }
        else if (vspeed < 0)
        {
            moveToRoof(); // move up
            vspeed = 0.f;
        }
    }
    else
        gravity = 0.25f;

    hspeed += x_acc;
    vspeed += gravity;

    if (hspeed != 0)
    {
        if (blockCollide((x+hspeed+(4*m_dir))/32, (y-1)/32) or
            blockCollide((x+hspeed+(-4*m_dir))/32, (y-1)/32) or
            blockCollide((x+hspeed+(4*m_dir))/32, (y-32)/32) or
            blockCollide((x+hspeed+(-4*m_dir))/32, (y-32)/32) or
            blockCollide((x+hspeed+(4*m_dir))/32, (y-56)/32) or
            blockCollide((x+hspeed+(-4*m_dir))/32, (y-56)/32)) // horizontal collisions
        {
            hspeed = x_acc = 0;
        }
    }

    if (m_sneak and hspeed != 0 and groundCollide() and
        (not blockCollide((x+hspeed+(4*m_dir))/32, y/32) or
        not blockCollide((x+hspeed+(-4*m_dir))/32, y/32))) // prevent from falling off edges while sneaking
        hspeed = 0;

    x += hspeed;
    y += vspeed;

    if (m_isPlayer)
    {
        mousepos = engine->m_window.mapPixelToCoords(sf::Mouse::getPosition(engine->app), engine->m_window.getView());
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
        if (angle < 6 and angle > -6 and not m_footstepwait and not m_sneak and blockCollide(x/32, y/32))
        {
            int block = m_world->getBlock(x/32, y/32);
            sf::Vector2f view = engine->m_window.getView().getCenter();
            engine->Sound()->playFootstepSound(x, y, view.x, view.y, block);
            m_footstepwait = 25 - (armswing*2);
        }

        if (m_footstepwait) m_footstepwait--;
    }

    if (m_armtick)
        m_armtick-=10;
}

void Player::event_input(GameEngine *engine, sf::Event &event)
{
    if (not m_isPlayer or not can_move) return;

    if (engine->Settings()->controls()->PressedEvent("layerswap", event))
    {
        m_layer++;
        if (m_layer > LAYER_DECORATION) m_layer = 0;
    }
    else if (engine->Settings()->controls()->PressedEvent("layer1_collide", event))
    {
        m_layer1_collide = not m_layer1_collide;
        if (m_layer1_collide and
           (blockCollide((x+4)/32, (y-1)/32) or
            blockCollide((x)/32, (y-1)/32) or
            blockCollide((x-4)/32, (y-1)/32) or
            blockCollide((x+4)/32, (y-32)/32) or
            blockCollide((x)/32, (y-32)/32) or
            blockCollide((x-4)/32, (y-32)/32) or
            blockCollide((x+4)/32, (y-56)/32) or
            blockCollide((x)/32, (y-56)/32) or
            blockCollide((x-4)/32, (y-56)/32)))
            m_layer1_collide = false;
    }
}

void Player::process_input(GameEngine *engine)
{
    if (not m_isPlayer) return;

    if (can_move)
    {
        // keyboard
        m_sneak = engine->Settings()->controls()->Pressed("sneak");
        float spd;
        if (not m_sneak)
            spd = (engine->Settings()->controls()->Pressed("run")) ? 4 : 3;
        else
            spd = 1.5f;

        if (engine->Settings()->controls()->Pressed("left"))
            x_acc = (hspeed > 0) ? -0.5f : -0.25f;
        else if (engine->Settings()->controls()->Pressed("right"))
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

        if (engine->Settings()->controls()->Pressed("jump") and
            (blockCollide(x/32, y/32) or
            blockCollide((x-4)/32, y/32) or
            blockCollide((x+4)/32, y/32)))
            vspeed = -4.5f;
    }
    else
    {
        if (hspeed > 0)
            x_acc = -0.25f;
        else if (hspeed < 0)
            x_acc = +0.25f;
        else
            x_acc = 0.f;
    }
}

void Player::draw(GameEngine *engine)
{
    adjustSkinDir();

    sf::RenderStates state;
    state.texture = &m_skin;

    int sneakangle = (m_sneak) ? 20*m_dir : 0;
    int x_sneak = (m_sneak) ? 8*(-m_dir) : 0;
    int y_sneak = (m_sneak) ? 6 : 0;
    int block_hand_angle = (m_currblock) ? -20*m_dir : 0;
    float armswing = (hspeed > 5) ? 5 : hspeed;
    float maxangle = armswing*9.0f;
    float angle = sin(m_footstepticks/60.0f) * maxangle;

    sf::Transform arm1, arm2, leg1, leg2, body, head, block_hand;
    sf::VertexArray v_block_hand(sf::Quads, 4), v_arm1(sf::Quads, 4);
    v_block_hand[0].texCoords = sf::Vector2f(m_currblock*32, 0);
    v_block_hand[1].texCoords = sf::Vector2f(m_currblock*32+32, 0);
    v_block_hand[2].texCoords = sf::Vector2f(m_currblock*32+32, 32);
    v_block_hand[3].texCoords = sf::Vector2f(m_currblock*32, 32);

    head.rotate((m_dir > 0) ? m_angle : m_angle+180, x, y-48+y_sneak);
    body.rotate(sneakangle, x, y-48+y_sneak);
    arm1.rotate((angle + (((m_armtick/10.0f)*(6.0f))*-m_dir)) + sneakangle + block_hand_angle, x-4+(m_dir*2), y-48+y_sneak);
    block_hand.rotate((angle + (((m_armtick/10.0f)*(6.0f))*-m_dir)) + sneakangle + block_hand_angle, x-4+(m_dir*2), y-48+y_sneak);
    arm2.rotate(-angle+sneakangle, x+4, y-48+y_sneak);
    leg1.rotate(angle, x+x_sneak, y-24);
    leg2.rotate(-angle, x+x_sneak, y-24);

    if (m_dir > 0) // right
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

        //m_skinvertex[PLAYER_ARMFRONT+0].texCoords = sf::Vector2f(40, 20);
        //m_skinvertex[PLAYER_ARMFRONT+1].texCoords = sf::Vector2f(44, 20);
        //m_skinvertex[PLAYER_ARMFRONT+2].texCoords = sf::Vector2f(44, 32);
        //m_skinvertex[PLAYER_ARMFRONT+3].texCoords = sf::Vector2f(40, 32);
        v_arm1[0].texCoords = sf::Vector2f(40, 20);
        v_arm1[1].texCoords = sf::Vector2f(44, 20);
        v_arm1[2].texCoords = sf::Vector2f(44, 32);
        v_arm1[3].texCoords = sf::Vector2f(40, 32);

        m_skinvertex[PLAYER_LEGBACK+0].texCoords = sf::Vector2f(0, 20);
        m_skinvertex[PLAYER_LEGBACK+1].texCoords = sf::Vector2f(4, 20);
        m_skinvertex[PLAYER_LEGBACK+2].texCoords = sf::Vector2f(4, 32);
        m_skinvertex[PLAYER_LEGBACK+3].texCoords = sf::Vector2f(0, 32);

        m_skinvertex[PLAYER_LEGFRONT+0].texCoords = sf::Vector2f(0, 20);
        m_skinvertex[PLAYER_LEGFRONT+1].texCoords = sf::Vector2f(4, 20);
        m_skinvertex[PLAYER_LEGFRONT+2].texCoords = sf::Vector2f(4, 32);
        m_skinvertex[PLAYER_LEGFRONT+3].texCoords = sf::Vector2f(0, 32);

        m_skinvertex[PLAYER_HOODIE+0].texCoords = sf::Vector2f(32, 8);
        m_skinvertex[PLAYER_HOODIE+1].texCoords = sf::Vector2f(40, 8);
        m_skinvertex[PLAYER_HOODIE+2].texCoords = sf::Vector2f(40, 16);
        m_skinvertex[PLAYER_HOODIE+3].texCoords = sf::Vector2f(32, 16);


        // positions
        m_skinvertex[PLAYER_LEGFRONT+0].position = leg1.transformPoint(sf::Vector2f(x-4+x_sneak, y-24));
        m_skinvertex[PLAYER_LEGFRONT+1].position = leg1.transformPoint(sf::Vector2f(x+4+x_sneak, y-24));
        m_skinvertex[PLAYER_LEGFRONT+2].position = leg1.transformPoint(sf::Vector2f(x+4+x_sneak, y));
        m_skinvertex[PLAYER_LEGFRONT+3].position = leg1.transformPoint(sf::Vector2f(x-4+x_sneak, y));

        m_skinvertex[PLAYER_LEGBACK+0].position = leg2.transformPoint(sf::Vector2f(x-4+x_sneak, y-24));
        m_skinvertex[PLAYER_LEGBACK+1].position = leg2.transformPoint(sf::Vector2f(x+4+x_sneak, y-24));
        m_skinvertex[PLAYER_LEGBACK+2].position = leg2.transformPoint(sf::Vector2f(x+4+x_sneak, y));
        m_skinvertex[PLAYER_LEGBACK+3].position = leg2.transformPoint(sf::Vector2f(x-4+x_sneak, y));

        m_skinvertex[PLAYER_ARMFRONT+0].position = sf::Vector2f(0, 0);
        m_skinvertex[PLAYER_ARMFRONT+1].position = sf::Vector2f(0, 0);
        m_skinvertex[PLAYER_ARMFRONT+2].position = sf::Vector2f(0, 0);
        m_skinvertex[PLAYER_ARMFRONT+3].position = sf::Vector2f(0, 0);
        v_arm1[0].position = arm1.transformPoint(sf::Vector2f(x-6, y-48+y_sneak));
        v_arm1[1].position = arm1.transformPoint(sf::Vector2f(x+2, y-48+y_sneak));
        v_arm1[2].position = arm1.transformPoint(sf::Vector2f(x+2, y-24+y_sneak));
        v_arm1[3].position = arm1.transformPoint(sf::Vector2f(x-6, y-24+y_sneak));

        v_block_hand[0].position = block_hand.transformPoint(sf::Vector2f(x-4, y-32+y_sneak));
        v_block_hand[1].position = block_hand.transformPoint(sf::Vector2f(x+12, y-32+y_sneak));
        v_block_hand[2].position = block_hand.transformPoint(sf::Vector2f(x+12, y-16+y_sneak));
        v_block_hand[3].position = block_hand.transformPoint(sf::Vector2f(x-4, y-16+y_sneak));

        m_skinvertex[PLAYER_ARMBACK+0].position = arm2.transformPoint(sf::Vector2f(x-2, y-48+y_sneak));
        m_skinvertex[PLAYER_ARMBACK+1].position = arm2.transformPoint(sf::Vector2f(x+6, y-48+y_sneak));
        m_skinvertex[PLAYER_ARMBACK+2].position = arm2.transformPoint(sf::Vector2f(x+6, y-24+y_sneak));
        m_skinvertex[PLAYER_ARMBACK+3].position = arm2.transformPoint(sf::Vector2f(x-2, y-24+y_sneak));

        m_skinvertex[PLAYER_BODY+0].position = body.transformPoint(sf::Vector2f(x-4, y-48+y_sneak));
        m_skinvertex[PLAYER_BODY+1].position = body.transformPoint(sf::Vector2f(x+4, y-48+y_sneak));
        m_skinvertex[PLAYER_BODY+2].position = body.transformPoint(sf::Vector2f(x+4, y-24+y_sneak));
        m_skinvertex[PLAYER_BODY+3].position = body.transformPoint(sf::Vector2f(x-4, y-24+y_sneak));

        m_skinvertex[PLAYER_HEAD+0].position = head.transformPoint(sf::Vector2f(x-8, y-64+y_sneak));
        m_skinvertex[PLAYER_HEAD+1].position = head.transformPoint(sf::Vector2f(x+8, y-64+y_sneak));
        m_skinvertex[PLAYER_HEAD+2].position = head.transformPoint(sf::Vector2f(x+8, y-48+y_sneak));
        m_skinvertex[PLAYER_HEAD+3].position = head.transformPoint(sf::Vector2f(x-8, y-48+y_sneak));

        m_skinvertex[PLAYER_HOODIE+0].position = head.transformPoint(sf::Vector2f(x-8, y-64+y_sneak));
        m_skinvertex[PLAYER_HOODIE+1].position = head.transformPoint(sf::Vector2f(x+8, y-64+y_sneak));
        m_skinvertex[PLAYER_HOODIE+2].position = head.transformPoint(sf::Vector2f(x+8, y-48+y_sneak));
        m_skinvertex[PLAYER_HOODIE+3].position = head.transformPoint(sf::Vector2f(x-8, y-48+y_sneak));
    }

    else if (m_dir < 0)
    {
        // texture coordinates
        m_skinvertex[PLAYER_HEAD+0].texCoords = sf::Vector2f(16, 8);
        m_skinvertex[PLAYER_HEAD+1].texCoords = sf::Vector2f(24, 8);
        m_skinvertex[PLAYER_HEAD+2].texCoords = sf::Vector2f(24, 16);
        m_skinvertex[PLAYER_HEAD+3].texCoords = sf::Vector2f(16, 16);

        v_arm1[0].texCoords = sf::Vector2f(48, 20);
        v_arm1[1].texCoords = sf::Vector2f(52, 20);
        v_arm1[2].texCoords = sf::Vector2f(52, 32);
        v_arm1[3].texCoords = sf::Vector2f(48, 32);

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

        m_skinvertex[PLAYER_HOODIE+0].texCoords = sf::Vector2f(48, 8);
        m_skinvertex[PLAYER_HOODIE+1].texCoords = sf::Vector2f(56, 8);
        m_skinvertex[PLAYER_HOODIE+2].texCoords = sf::Vector2f(56, 16);
        m_skinvertex[PLAYER_HOODIE+3].texCoords = sf::Vector2f(48, 16);


        // positions
        m_skinvertex[PLAYER_LEGFRONT+0].position = leg1.transformPoint(sf::Vector2f(x-4+x_sneak, y-24));
        m_skinvertex[PLAYER_LEGFRONT+1].position = leg1.transformPoint(sf::Vector2f(x+4+x_sneak, y-24));
        m_skinvertex[PLAYER_LEGFRONT+2].position = leg1.transformPoint(sf::Vector2f(x+4+x_sneak, y));
        m_skinvertex[PLAYER_LEGFRONT+3].position = leg1.transformPoint(sf::Vector2f(x-4+x_sneak, y));

        m_skinvertex[PLAYER_LEGBACK+0].position = leg2.transformPoint(sf::Vector2f(x-4+x_sneak, y-24));
        m_skinvertex[PLAYER_LEGBACK+1].position = leg2.transformPoint(sf::Vector2f(x+4+x_sneak, y-24));
        m_skinvertex[PLAYER_LEGBACK+2].position = leg2.transformPoint(sf::Vector2f(x+4+x_sneak, y));
        m_skinvertex[PLAYER_LEGBACK+3].position = leg2.transformPoint(sf::Vector2f(x-4+x_sneak, y));

        m_skinvertex[PLAYER_ARMFRONT+0].position = arm2.transformPoint(sf::Vector2f(x-2, y-48+y_sneak));
        m_skinvertex[PLAYER_ARMFRONT+1].position = arm2.transformPoint(sf::Vector2f(x+6, y-48+y_sneak));
        m_skinvertex[PLAYER_ARMFRONT+2].position = arm2.transformPoint(sf::Vector2f(x+6, y-24+y_sneak));
        m_skinvertex[PLAYER_ARMFRONT+3].position = arm2.transformPoint(sf::Vector2f(x-2, y-24+y_sneak));

        m_skinvertex[PLAYER_ARMBACK+0].position = sf::Vector2f(0, 0);
        m_skinvertex[PLAYER_ARMBACK+1].position = sf::Vector2f(0, 0);
        m_skinvertex[PLAYER_ARMBACK+2].position = sf::Vector2f(0, 0);
        m_skinvertex[PLAYER_ARMBACK+3].position = sf::Vector2f(0, 0);
        v_arm1[0].position = arm1.transformPoint(sf::Vector2f(x-6, y-48+y_sneak));
        v_arm1[1].position = arm1.transformPoint(sf::Vector2f(x+2, y-48+y_sneak));
        v_arm1[2].position = arm1.transformPoint(sf::Vector2f(x+2, y-24+y_sneak));
        v_arm1[3].position = arm1.transformPoint(sf::Vector2f(x-6, y-24+y_sneak));

        v_block_hand[0].position = block_hand.transformPoint(sf::Vector2f(x-16, y-32+y_sneak));
        v_block_hand[1].position = block_hand.transformPoint(sf::Vector2f(x-0, y-32+y_sneak));
        v_block_hand[2].position = block_hand.transformPoint(sf::Vector2f(x-0, y-16+y_sneak));
        v_block_hand[3].position = block_hand.transformPoint(sf::Vector2f(x-16, y-16+y_sneak));

        m_skinvertex[PLAYER_BODY+0].position = body.transformPoint(sf::Vector2f(x-4, y-48+y_sneak));
        m_skinvertex[PLAYER_BODY+1].position = body.transformPoint(sf::Vector2f(x+4, y-48+y_sneak));
        m_skinvertex[PLAYER_BODY+2].position = body.transformPoint(sf::Vector2f(x+4, y-24+y_sneak));
        m_skinvertex[PLAYER_BODY+3].position = body.transformPoint(sf::Vector2f(x-4, y-24+y_sneak));

        m_skinvertex[PLAYER_HEAD+0].position = head.transformPoint(sf::Vector2f(x-8, y-64+y_sneak));
        m_skinvertex[PLAYER_HEAD+1].position = head.transformPoint(sf::Vector2f(x+8, y-64+y_sneak));
        m_skinvertex[PLAYER_HEAD+2].position = head.transformPoint(sf::Vector2f(x+8, y-48+y_sneak));
        m_skinvertex[PLAYER_HEAD+3].position = head.transformPoint(sf::Vector2f(x-8, y-48+y_sneak));

        m_skinvertex[PLAYER_HOODIE+0].position = head.transformPoint(sf::Vector2f(x-8, y-64+y_sneak));
        m_skinvertex[PLAYER_HOODIE+1].position = head.transformPoint(sf::Vector2f(x+8, y-64+y_sneak));
        m_skinvertex[PLAYER_HOODIE+2].position = head.transformPoint(sf::Vector2f(x+8, y-48+y_sneak));
        m_skinvertex[PLAYER_HOODIE+3].position = head.transformPoint(sf::Vector2f(x-8, y-48+y_sneak));
    }

    if (m_dir < 0)
    {
        engine->m_window.draw(v_arm1, state);
        engine->m_window.draw(v_block_hand, &engine->m_blocks);
    }
    engine->m_window.draw(m_skinvertex, state);
    if (m_dir > 0)
    {
        engine->m_window.draw(v_block_hand, &engine->m_blocks);
        engine->m_window.draw(v_arm1, state);
    }
}
