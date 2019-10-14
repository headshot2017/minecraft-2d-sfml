#include "animated_sprite.h"

AnimatedSprite::AnimatedSprite(GameEngine *engine, sf::Texture texture, sf::Vector2f pos, sf::Vector2u frame_size, int num_frames, int delay, bool loop)
{
    m_engine = engine;
    m_currframe = m_ticks = m_framedelay = m_numframes = 0;

    setTexture(texture);
    setPosition(pos);
    setFrameSize(frame_size);
    setNumFrames(num_frames);
    setFrameDelay(delay);
    setLoop(loop);
}

AnimatedSprite::AnimatedSprite(GameEngine *engine, sf::Texture texture, float x, float y, unsigned width, unsigned height, int num_frames, int delay, bool loop)
{
    m_engine = engine;
    m_currframe = m_ticks = m_framedelay = m_numframes = 0;

    setTexture(texture);
    setPosition(x,y);
    setFrameSize(width,height);
    setNumFrames(num_frames);
    setFrameDelay(delay);
    setLoop(loop);
}

AnimatedSprite::AnimatedSprite(GameEngine *engine, sf::Texture texture, sf::Vector2f pos, unsigned width, unsigned height, int num_frames, int delay, bool loop)
{
    m_engine = engine;
    m_currframe = m_ticks = m_framedelay = m_numframes = 0;

    setTexture(texture);
    setPosition(pos);
    setFrameSize(width,height);
    setNumFrames(num_frames);
    setFrameDelay(delay);
    setLoop(loop);
}

AnimatedSprite::AnimatedSprite(GameEngine *engine, sf::Texture texture, float x, float y, sf::Vector2u frame_size, int num_frames, int delay, bool loop)
{
    m_engine = engine;
    m_currframe = m_ticks = m_framedelay = m_numframes = 0;

    setTexture(texture);
    setPosition(x,y);
    setFrameSize(frame_size);
    setNumFrames(num_frames);
    setFrameDelay(delay);
    setLoop(loop);
}

void AnimatedSprite::update()
{
    m_ticks++;
    if (m_ticks % m_framedelay == 0)
    {
        m_currframe++;
        if (isFinished())
        {
            if (m_loop)
                m_currframe = 0;
        }
        else
        {

        }
    }
    sf::Vector2u size = m_texture.getSize();

    int cols = size.x / m_framesize.x;
    int rows = size.y / m_framesize.y;
    int left = (m_currframe % cols) * m_framesize.x;
    int top = (m_currframe / rows) * m_framesize.y;

    m_sprite.setTexture(m_texture);
    m_sprite.setTextureRect(sf::IntRect(left, top, m_framesize.x, m_framesize.y));
}

void AnimatedSprite::draw()
{
    m_engine->m_window.draw(m_sprite);
}
