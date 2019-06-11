#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <SFML/Graphics.hpp>
#include "world.h"

enum
{
    PLAYER_HEAD,
    PLAYER_ARMBACK=4,
    PLAYER_BODY=8,
    PLAYER_ARMFRONT=12,
    PLAYER_LEGBACK=16,
    PLAYER_LEGFRONT=20
};

class Player
{
public:
    Player();
    Player(World& world);
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
    bool blockBelow();

    sf::VertexArray& getSkinVertex() {return m_skinvertex;}
    sf::Vector2f getPos() const {return sf::Vector2f(x,y);}

private:
    sf::Texture m_skin;
    sf::VertexArray m_skinvertex;
    World m_world;
    float x, y, new_x, new_y, hspeed, vspeed, x_acc, gravity;
    int m_dir;
    bool m_isPlayer = false, can_move = true;

};

#endif // PLAYER_H_INCLUDED
