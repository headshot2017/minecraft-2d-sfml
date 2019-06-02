// if pyraq is reading this:
// Confusion 100

#include "game_engine.h" //import game_engine
#include "menu_state.h" //import menu_state

int main(int argc, char *argv[]) //def main(argc, argv):
{
    GameEngine engine;
    engine.init();
    engine.changeState(MenuState::Instance());

    while (engine.isRunning())
    {
        engine.update();
        engine.process_input();
        engine.draw();
    }

    engine.cleanup();
    return 0;
}
