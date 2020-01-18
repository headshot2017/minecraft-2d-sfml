#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <SFML/Graphics.hpp>
#include "world.h"

enum
{
    PLAYER_LEGBACK,
    PLAYER_LEGFRONT=4,
    PLAYER_ARMBACK=8,
    PLAYER_BODY=12,
    PLAYER_ARMFRONT=16,
    PLAYER_HEAD=20,
    PLAYER_HOODIE=24
};

class World;

class Player
{
public:
    Player();
    Player(World* world, GameEngine* engine);
    ~Player();
    void setSkin(const char *name);

    void update(GameEngine* engine);
    void event_input(GameEngine* engine, sf::Event &event);
    void process_input(GameEngine* engine);
    void draw(GameEngine* engine);
    void moveToGround();
    void moveToRoof(); // move up until head collides with block
    void adjustSkinDir();
    void move(float _x, float _y) {x = _x; y = _y;}

    void setCanMove(bool t) {can_move = t;}
    void setPlayer(bool t) {m_isPlayer = t;}
    void setFlying(bool t) {m_fly = t;}
    void setCurrBlock(int t) {m_currblock = t;}
    void setAngle(float t) {m_angle = t;}
    void setLayer1Collisions(bool t) {m_layer1_collide = t;}
    float getAngle() {return m_angle;}
    int getCurrBlock() {return m_currblock;}
    bool getLayer1Collisions() {return m_layer1_collide;}
    bool isPlayer() {return m_isPlayer;}
    bool isFlying() {return m_fly;}
    bool blockCollide(int x, int y);
    bool groundCollide();
    bool inWater();
    void knockBack(float xx, float yy, int maxdist);

    bool getCanMove() {return can_move;}
    bool canBuild(int x, int y);
    void placeBlock(int x, int y, int block, int layer);
    void destroyBlock(int x, int y);
    void interactBlock(int x, int y);

    sf::VertexArray& getSkinVertex() {return m_skinvertex;}
    sf::Vector2f getPos() const {return sf::Vector2f(x,y);}
    sf::Vector2f getSpeed() const {return sf::Vector2f(hspeed,vspeed);}
    sf::Vector2f getMouse() {return mousepos;}
    int getBuildLayer() {return m_layer;}

private:
    sf::Texture m_skin;
    sf::VertexArray m_skinvertex;
    sf::Vector2f mousepos;

    World *m_world;

    float x, y, new_x, new_y, hspeed, vspeed, x_acc, gravity, m_angle;
    int m_dir = 1;
    int m_layer = LAYER_BUILD;
    bool m_sneak = false;

    bool m_isPlayer = false, can_move = true, m_layer1_collide = false, m_fly = false;
    bool m_inWater2 = false;
    double m_footstepticks = 0.0; // for footstep animations
    int m_ticks = 0; // ticks elapsed since creation (mainly for footstep sounds)
    int m_armtick = 0; // arm swing (when clicking)
    int m_footstepwait = 0; // ticks to wait until next footstep sound

    int m_currblock = 1;

    GameEngine *m_engine;

};

#endif // PLAYER_H_INCLUDED
