#ifndef CONTROLS_H_INCLUDED
#define CONTROLS_H_INCLUDED

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Event.hpp>
#include <map>
#include <string>

enum InputType
{
    KeyboardInput,
    MouseInput,
    JoystickInput
};

struct MyKeys
{
    InputType inputType;
    sf::Event::EventType eventType;
    sf::Keyboard::Key key;
    sf::Mouse::Button mouseButton;
};

typedef std::map<std::string, MyKeys> keysMap;

class Controls
{
public:
    Controls();

    const char* getKeyName(const char* action);

    void setKeyBind(const char* action, const sf::Keyboard::Key btn);
    sf::Keyboard::Key Key(const char* action) {return m_keys[action].key;}

    void setMouseBind(const char *action, const sf::Mouse::Button btn);
    sf::Mouse::Button MouseButton(const char* action) {return m_keys[action].mouseButton;}

    bool Pressed(const char *action);
    bool PressedEvent(const char *action, sf::Event& event);

    const keysMap& getKeys() {return m_keys;}

private:
    keysMap m_keys;
};

#endif // CONTROLS_H_INCLUDED
