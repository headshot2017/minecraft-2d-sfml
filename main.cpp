#include "game_engine.h"
#include "menu_state.h"
#include <stdlib.h>

int main(int argc, char *argv[])
{
    GameEngine engine;
    engine.init();
    engine.changeState(new MenuState(&engine));

    while (engine.isRunning())
    {
        engine.update();
        engine.process_input();
        engine.draw();
        engine.m_ticks++;

    }

    printf("game closed, cleaning up...\n");
    engine.cleanup();
    printf("done.\n");
    return 0;
}
