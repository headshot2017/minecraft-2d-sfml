#include "ingame_gui.h"

void GameGUI::update(GameEngine *engine)
{

}

int GameGUI::event_input(GameEngine *engine, sf::Event& event)
{
    sf::Vector2f view = engine->m_window.getView().getCenter();

    if (m_gui == GUI_INVENTORY)
    {
        int width = 400, height = 300;
        view.x -= width/2.0f;
        view.y -= height/2.0f;

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

            float real_x = view.x+left+x_pos;
            float real_y = view.y+top+y_pos;

            if (event.type == sf::Event::MouseButtonPressed)
            {
                sf::Vector2f mousepos = engine->m_window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), engine->m_window.getView());
                if (event.mouseButton.button == sf::Mouse::Left and
                    mousepos.x >= real_x and mousepos.y >= real_y and
                    mousepos.x <= real_x+32 and mousepos.y <= real_y+32)
                    return i;
            }

            x_mod_count += 1;
			if (x_mod_count == columns)
            {
				y_mod_count += 1;
				x_mod_count = 0;
            }
        }
    }

    return -1;
}

void GameGUI::draw(GameEngine *engine)
{
    sf::Vector2f view = engine->m_window.getView().getCenter();
    sf::Vector2f mousepos = engine->m_window.mapPixelToCoords(sf::Mouse::getPosition(engine->app), engine->m_window.getView());

    if (m_gui == GUI_INVENTORY)
    {
        int width = 400, height = 300;
        view.x -= width/2;
        view.y -= height/2;

        int x_spacing = 12, y_spacing = 12;
        int x_mod_count = 0, y_mod_count = 0;
        int left = 8, top = 48;
        int b_width = 32, b_height = 32;
        int columns = (width - b_width) / (x_spacing + b_width) + 1;
        int rows = (height - b_height) / (y_spacing + b_height) + 1;
        int max_blocks_on_page = columns * rows;

        engine->m_window.draw(engine->getGUIWindow(sf::Vector2f(width, height)));
        for (int i=1; i<max_blocks_on_page; i++)
        {
            if (i >= BLOCK_TOTAL) break;
            int x_pos = (b_width + x_spacing) * x_mod_count;
            int y_pos = (b_height + y_spacing) * y_mod_count;

            sf::Sprite block(engine->m_blocks, sf::IntRect(i*32, 0, 32, 32));
            if (mousepos.x >= view.x+left+x_pos and mousepos.y >= view.y+top+y_pos and
                mousepos.x <= view.x+left+x_pos+32 and mousepos.y <= view.y+top+y_pos+32)
                block.setScale(1.5f, 1.5f);
            float amnt = (32*block.getScale().x)/2.0f;
            block.setPosition(view.x+left+x_pos + 16-amnt, view.y+top+y_pos + 16-amnt);
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
