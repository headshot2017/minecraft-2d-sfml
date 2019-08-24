#include "ingame_gui.h"

void GameGUI::update(GameEngine *engine)
{

}

void GameGUI::event_input(GameEngine *engine, sf::Event& event)
{

}

void GameGUI::draw(GameEngine *engine)
{
    sf::Vector2f view = engine->m_window.getView().getCenter();
    if (m_gui == GUI_INVENTORY)
    {
        int width = 400, height = 300;
        view.x -= width/2;
        view.y -= height/2;
        engine->m_window.draw(engine->getGUIWindow(sf::Vector2f(width, height)));

        int x_spacing = 12, y_spacing = 12;
        int x_mod_count = 0, y_mod_count = 0;
        int left = 8, top = 48;
        int b_width = 32, b_height = 32;
        int columns = (width - b_width) / (x_spacing + b_width) + 1;
        int rows = (height - b_height) / (y_spacing + b_height) + 1;
        int max_blocks_on_page = columns * rows;

        for (int i=1; i<max_blocks_on_page; i++)
        {
            if (i >= BLOCK_TOTAL) break;
            int x_pos = (b_width + x_spacing) * x_mod_count;
            int y_pos = (b_height + y_spacing) * y_mod_count;

            sf::Sprite block(engine->m_blocks, sf::IntRect(i*32, 0, 32, 32));
            block.setPosition(view.x+left+x_pos, view.y+top+y_pos);
            engine->m_window.draw(block);

            x_mod_count += 1;
			if (x_mod_count == columns)
            {
				y_mod_count += 1;
				x_mod_count = 0;
            }
        }
    }
}
