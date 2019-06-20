#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <SFML/Graphics.hpp>
#include "world.h"

enum
{
    PLAYER_ARMBACK,
    PLAYER_BODY=4,
    PLAYER_ARMFRONT=8,
    PLAYER_LEGBACK=12,
    PLAYER_LEGFRONT=16,
    PLAYER_HEAD=20
};

class Player
{
public:
    Player();
    Player(World& world, GameEngine* engine);
    ~Player();
    void setSkin(const char *name);

    void update(GameEngine* engine);
    void process_input(GameEngine* engine);
    void draw(GameEngine* engine);
    void moveToGround();
    void adjustSkinDir();
    void move(float _x, float _y) {x = _x; y = _y;}

    void setPlayer(bool t) {m_isPlayer = t;}
    bool isPlayer() {return m_isPlayer;}
    bool blockCollide(int x, int y);

    void placeBlock(int x, int y, int block);

    sf::VertexArray& getSkinVertex() {return m_skinvertex;}
    sf::Vector2f getPos() const {return sf::Vector2f(x,y);}
    sf::Vector2f getSpeed() const {return sf::Vector2f(hspeed,vspeed);}

private:
    sf::Texture m_skin;
    sf::VertexArray m_skinvertex;
    sf::Vector2f mousepos;

    World m_world;

    float x, y, new_x, new_y, hspeed, vspeed, x_acc, gravity, m_angle;
    int m_dir = 1;
    bool m_isPlayer = false, can_move = true;
    double m_footstepticks = 0.0; // for footstep animations
    int m_ticks = 0; // ticks elapsed since creation (mainly for footstep sounds)
    int m_armtick = 0; // arm swing (when clicking)
    int m_footstepwait = 0; // ticks to wait until next footstep sound

    bool lmb = false, rmb = false, mmb = false; // mouse buttons
    int lmb_tick = 0;

    GameEngine *m_engine;

};

#endif // PLAYER_H_INCLUDED