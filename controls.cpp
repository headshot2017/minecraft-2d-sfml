#include "controls.h"

Controls::Controls()
{
    MyKeys key;

    key.inputType = MouseInput;
    key.eventType = sf::Event::MouseButtonPressed;
    key.mouseButton = sf::Mouse::Left;
    m_keys["destroy"] = key;

    key.mouseButton = sf::Mouse::Right;
    m_keys["place"] = key;

    key.mouseButton = sf::Mouse::Middle;
    m_keys["pick"] = key;

    key.inputType = KeyboardInput;
    key.eventType = sf::Event::KeyPressed;
    key.key = sf::Keyboard::A;
    m_keys["left"] = key;

    key.key = sf::Keyboard::D;
    m_keys["right"] = key;

    key.key = sf::Keyboard::Space;
    m_keys["jump"] = key;

    key.key = sf::Keyboard::LShift;
    m_keys["sneak"] = key;

    key.key = sf::Keyboard::LControl;
    m_keys["run"] = key;

    key.key = sf::Keyboard::E;
    m_keys["inventory"] = key;

    key.key = sf::Keyboard::Q;
    m_keys["drop"] = key;

    key.key = sf::Keyboard::LAlt;
    m_keys["layerswap"] = key;

    key.key = sf::Keyboard::Z;
    m_keys["layer1_collide"] = key;

    key.key = sf::Keyboard::F2;
    m_keys["screenshot"] = key;

    key.key = sf::Keyboard::F11;
    m_keys["fullscreen"] = key;
}

const char* Controls::getKeyName(const char *action)
{
    MyKeys key = m_keys[action];

    if (key.inputType == MouseInput)
    {
        switch(key.mouseButton)
        {
            case sf::Mouse::Left:
                return "MouseLeft";
            case sf::Mouse::Right:
                return "MouseRight";
            case sf::Mouse::Middle:
                return "MouseMiddle";
            case sf::Mouse::XButton1:
                return "XButton1";
            case sf::Mouse::XButton2:
                return "XButton2";
            default:
                return "Unknown";
        }
    }

    else if (key.inputType == KeyboardInput)
    {
        switch(key.key)
        {
            default:
            case sf::Keyboard::Unknown:
                    return "Unknown";
            case sf::Keyboard::A:
                    return "A";
            case sf::Keyboard::B:
                    return "B";
            case sf::Keyboard::C:
                    return "C";
            case sf::Keyboard::D:
                    return "D";
            case sf::Keyboard::E:
                    return "E";
            case sf::Keyboard::F:
                    return "F";
            case sf::Keyboard::G:
                    return "G";
            case sf::Keyboard::H:
                    return "H";
            case sf::Keyboard::I:
                    return "I";
            case sf::Keyboard::J:
                    return "J";
            case sf::Keyboard::K:
                    return "K";
            case sf::Keyboard::L:
                    return "L";
            case sf::Keyboard::M:
                    return "M";
            case sf::Keyboard::N:
                    return "N";
            case sf::Keyboard::O:
                    return "O";
            case sf::Keyboard::P:
                    return "P";
            case sf::Keyboard::Q:
                    return "Q";
            case sf::Keyboard::R:
                    return "R";
            case sf::Keyboard::S:
                    return "S";
            case sf::Keyboard::T:
                    return "T";
            case sf::Keyboard::U:
                    return "U";
            case sf::Keyboard::V:
                    return "V";
            case sf::Keyboard::W:
                    return "W";
            case sf::Keyboard::X:
                    return "X";
            case sf::Keyboard::Y:
                    return "Y";
            case sf::Keyboard::Z:
                    return "Z";
            case sf::Keyboard::Num0:
                    return "0";
            case sf::Keyboard::Num1:
                    return "1";
            case sf::Keyboard::Num2:
                    return "2";
            case sf::Keyboard::Num3:
                    return "3";
            case sf::Keyboard::Num4:
                    return "4";
            case sf::Keyboard::Num5:
                    return "5";
            case sf::Keyboard::Num6:
                    return "6";
            case sf::Keyboard::Num7:
                    return "7";
            case sf::Keyboard::Num8:
                    return "8";
            case sf::Keyboard::Num9:
                    return "9";
            case sf::Keyboard::Escape:
                    return "Escape";
            case sf::Keyboard::LControl:
                    return "LControl";
            case sf::Keyboard::LShift:
                    return "LShift";
            case sf::Keyboard::LAlt:
                    return "LAlt";
            case sf::Keyboard::LSystem:
                    return "LSystem";
            case sf::Keyboard::RControl:
                    return "RControl";
            case sf::Keyboard::RShift:
                    return "RShift";
            case sf::Keyboard::RAlt:
                    return "RAlt";
            case sf::Keyboard::RSystem:
                    return "RSystem";
            case sf::Keyboard::Menu:
                    return "Menu";
            case sf::Keyboard::LBracket:
                    return "LBracket";
            case sf::Keyboard::RBracket:
                    return "RBracket";
            case sf::Keyboard::SemiColon:
                    return "SemiColon";
            case sf::Keyboard::Comma:
                    return "Comma";
            case sf::Keyboard::Period:
                    return "Period";
            case sf::Keyboard::Quote:
                    return "Quote";
            case sf::Keyboard::Slash:
                    return "Slash";
            case sf::Keyboard::BackSlash:
                    return "BackSlash";
            case sf::Keyboard::Tilde:
                    return "Tilde";
            case sf::Keyboard::Equal:
                    return "Equal";
            case sf::Keyboard::Dash:
                    return "Dash";
            case sf::Keyboard::Space:
                    return "Space";
            case sf::Keyboard::Return:
                    return "Return";
            case sf::Keyboard::BackSpace:
                    return "BackSpace";
            case sf::Keyboard::Tab:
                    return "Tab";
            case sf::Keyboard::PageUp:
                    return "PageUp";
            case sf::Keyboard::PageDown:
                    return "PageDown";
            case sf::Keyboard::End:
                    return "End";
            case sf::Keyboard::Home:
                    return "Home";
            case sf::Keyboard::Insert:
                    return "Insert";
            case sf::Keyboard::Delete:
                    return "Delete";
            case sf::Keyboard::Add:
                    return "Add";
            case sf::Keyboard::Subtract:
                    return "Subtract";
            case sf::Keyboard::Multiply:
                    return "Multiply";
            case sf::Keyboard::Divide:
                    return "Divide";
            case sf::Keyboard::Left:
                    return "Left";
            case sf::Keyboard::Right:
                    return "Right";
            case sf::Keyboard::Up:
                    return "Up";
            case sf::Keyboard::Down:
                    return "Down";
            case sf::Keyboard::Numpad0:
                    return "Numpad0";
            case sf::Keyboard::Numpad1:
                    return "Numpad1";
            case sf::Keyboard::Numpad2:
                    return "Numpad2";
            case sf::Keyboard::Numpad3:
                    return "Numpad3";
            case sf::Keyboard::Numpad4:
                    return "Numpad4";
            case sf::Keyboard::Numpad5:
                    return "Numpad5";
            case sf::Keyboard::Numpad6:
                    return "Numpad6";
            case sf::Keyboard::Numpad7:
                    return "Numpad7";
            case sf::Keyboard::Numpad8:
                    return "Numpad8";
            case sf::Keyboard::Numpad9:
                    return "Numpad9";
            case sf::Keyboard::F1:
                    return "F1";
            case sf::Keyboard::F2:
                    return "F2";
            case sf::Keyboard::F3:
                    return "F3";
            case sf::Keyboard::F4:
                    return "F4";
            case sf::Keyboard::F5:
                    return "F5";
            case sf::Keyboard::F6:
                    return "F6";
            case sf::Keyboard::F7:
                    return "F7";
            case sf::Keyboard::F8:
                    return "F8";
            case sf::Keyboard::F9:
                    return "F9";
            case sf::Keyboard::F10:
                    return "F10";
            case sf::Keyboard::F11:
                    return "F11";
            case sf::Keyboard::F12:
                    return "F12";
            case sf::Keyboard::F13:
                    return "F13";
            case sf::Keyboard::F14:
                    return "F14";
            case sf::Keyboard::F15:
                    return "F15";
            case sf::Keyboard::Pause:
                    return "Pause";
        }
    }

    return "?";
}

void Controls::setMouseBind(const char *action, const sf::Mouse::Button btn)
{
    MyKeys key;
    key.inputType = MouseInput;
    key.eventType = sf::Event::MouseButtonPressed;
    key.mouseButton = btn;

    m_keys[action] = key;
}

void Controls::setKeyBind(const char *action, const sf::Keyboard::Key btn)
{
    MyKeys key;
    key.inputType = KeyboardInput;
    key.eventType = sf::Event::KeyPressed;
    key.key = btn;

    m_keys[action] = key;
}

bool Controls::Pressed(const char *action)
{
    if (m_keys[action].inputType == KeyboardInput)
        return sf::Keyboard::isKeyPressed(m_keys[action].key);
    if (m_keys[action].inputType == MouseInput)
        return sf::Mouse::isButtonPressed(m_keys[action].mouseButton);
    return false;
}

bool Controls::PressedEvent(const char *action, sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed and m_keys[action].eventType == event.type)
    {
        return event.key.code == m_keys[action].key;
    }
    else if (event.type == sf::Event::MouseButtonPressed and m_keys[action].eventType == event.type)
    {
        return event.mouseButton.button == m_keys[action].mouseButton;
    }

    return false;
}
