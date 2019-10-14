#ifndef ANIMATED_SPRITE_H_INCLUDED
#define ANIMATED_SPRITE_H_INCLUDED

#include "game_engine.h"

class AnimatedSprite
{
public:
    AnimatedSprite() {}
    AnimatedSprite(GameEngine *engine, sf::Texture texture, sf::Vector2f pos, sf::Vector2u frame_size, int num_frames, int delay, bool loop=true);
    AnimatedSprite(GameEngine *engine, sf::Texture texture, sf::Vector2f pos, unsigned width, unsigned height, int num_frames, int delay, bool loop=true);
    AnimatedSprite(GameEngine *engine, sf::Texture texture, float x, float y, sf::Vector2u frame_size, int num_frames, int delay, bool loop=true);
    AnimatedSprite(GameEngine *engine, sf::Texture texture, float x, float y, unsigned width, unsigned height, int num_frames, int delay, bool loop=true);

    void update();
    void draw();

    void setTexture(sf::Texture texture) {m_texture = texture; m_sprite.setTexture(texture, false);}
    void setPosition(sf::Vector2f pos) {m_sprite.setPosition(pos);}
    void setPosition(float x, float y) {m_sprite.setPosition(x,y);}
    void setFrameSize(sf::Vector2u frame_size) {m_framesize = frame_size;}
    void setFrameSize(unsigned w, unsigned h) {m_framesize = sf::Vector2u(w,h);}
    void setNumFrames(int t) {m_numframes = t;}
    void setFrameDelay(int t) {m_framedelay = t;}
    void setLoop(bool t) {m_loop = t; m_currframe = 0;}
    void setScale(float f) {m_sprite.setScale(f,f);}
    void setColor(sf::Color col) {m_sprite.setColor(col);}

    bool isFinished() {return m_currframe >= m_numframes-1;}

private:
    GameEngine *m_engine;

    sf::Sprite m_sprite;
    sf::Texture m_texture;
    sf::Vector2u m_framesize;
    int m_numframes, m_currframe, m_framedelay, m_ticks;
    bool m_loop;
};

#endif // ANIMATED_SPRITE_H_INCLUDED
