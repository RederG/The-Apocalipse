#include "game_window.hpp"
#include "game_over_window.hpp"

namespace GamePauseWindow{
    // The 'background rectangle'
    sf::RectangleShape bg_rect;

    // The continue button background (a rect)
    sf::RectangleShape Continue_rect;

    // The quit button background (a rect)
    sf::RectangleShape Quit_rect;

    // The 'Menu rectangle'
    sf::RectangleShape menu_rect(sf::Vector2f(450,250));

    // The title
    sf::Text Title(*new sf::Font());

    // The continue button
    sf::Text Continue(*new sf::Font());

    // The quit button
    sf::Text Quit(*new sf::Font());

    sf::Font font;

    // The buttons' text color
    sf::Color button_color = sf::Color(0,255,255); 
    // The buttons' rect color
    sf::Color button_rect_color = sf::Color(0,0,255);

    // The mouse position
    sf::Vector2f mouse;

    void Text(sf::Text &txt_name, std::string text, std::string font_file_path, int size, sf::Color color){
        if(!font.openFromFile(font_file_path))
            return;

        txt_name.setString(text);
        txt_name.setFont(font);
        txt_name.setCharacterSize(size);
        txt_name.setFillColor(color);
    }

    void Render(){
        Main::window()->clear();

        Entity::set_entities_movement_to(false);
        GameWindow::draw(true);
        Main::window()->draw(menu_rect);
        Main::window()->draw(Title);
        Main::window()->draw(Continue_rect);
        Main::window()->draw(Quit_rect);
        Main::window()->draw(Continue);
        Main::window()->draw(Quit);

        Main::window()->display();
    }

    void Set_content_position(){
        // The "x coords" for calculating the "x middle"
        int x = 0;
        // The "y coords" for calculating the "y middle"
        int y = 0;
        
        bg_rect.setPosition(sf::Vector2f(0,0));

        x  = Main::window()->getSize().x/2 - menu_rect.getGlobalBounds().size.x/2;
        y  = Main::window()->getSize().y/2 - menu_rect.getGlobalBounds().size.y/2;
        menu_rect.setPosition(sf::Vector2f(x, y));

        x  = Main::window()->getSize().x/2 - Title.getGlobalBounds().size.x/2;
        y  = menu_rect.getPosition().y+5;
        Title.setPosition(sf::Vector2f(x, y));

        x  = Main::window()->getSize().x/2 - Continue.getGlobalBounds().size.x/2;
        y  = Title.getPosition().y + Title.getGlobalBounds().size.y + 30;
        Continue.setPosition(sf::Vector2f(x, y));

        Continue_rect.setPosition(sf::Vector2f(Continue.getGlobalBounds().position));

        x  = Main::window()->getSize().x/2 - Quit.getGlobalBounds().size.x/2;
        y  = Continue.getPosition().y + Continue_rect.getGlobalBounds().size.y + 10;
        Quit.setPosition(sf::Vector2f(x, y));

        Quit_rect.setPosition(sf::Vector2f(Quit.getGlobalBounds().position));
    }

    void Input(){
        while(std::optional event = Main::window()->pollEvent()){
            Main::check_resize_events(event);
            if(event->is<sf::Event::Closed>()){
                Main::window()->close();
                TheApocalipse::terminates();
                ThreadManager::terminates_all();
            }

            if(event->is<sf::Event::MouseButtonPressed>()){
                if(Continue_rect.getGlobalBounds().contains({mouse.x, mouse.y})){
                    Main::context(Main::Context::TheGame);
                    ThreadManager::active_all();
                    Debugging::write("Button : CONTINUE GAME\n", Debugging::get_state_of(Debugging::Button));
                    Debugging::write("----THE GAME----", Debugging::get_state_of(Debugging::Description));
                }
                
                if(Quit_rect.getGlobalBounds().contains({mouse.x, mouse.y})){
                    Debugging::write("Button : QUIT GAME\n", Debugging::get_state_of(Debugging::Button));
                    GameOverWindow::save_progress(false);
                    Main::context(Main::Context::PlayMenu);
                    Main::first_drawing(true);
                    player_choosed.clear();
                    GameWindow::time = 0;
                    TheApocalipse::terminates();
                }
            }
        }
    }

    void Update_content(){
        mouse.x = sf::Mouse::getPosition(*Main::window()).x;
        mouse.y = sf::Mouse::getPosition(*Main::window()).y;

        if(Continue_rect.getGlobalBounds().contains({mouse.x, mouse.y}))
            Continue.setFillColor(sf::Color(0,100,255));
        else
            Continue.setFillColor(button_color);
        if(Quit_rect.getGlobalBounds().contains({mouse.x, mouse.y}))
            Quit.setFillColor(sf::Color(0,100,255));
        else
            Quit.setFillColor(button_color);
    }

    void Event(){
        bg_rect.setSize(sf::Vector2f(Main::window()->getSize()));
        Main::window()->setFramerateLimit(60);
        
        // The path for the Arial's font
        std::string Arial_font_path = "../res/Fonts/Arial_regular.ttf";

        // ========== ALL LANGUAGES ========== // 

        std::map<Main::Languages, sf::String> title_lang{
            {Main::Languages::english, "PAUSE"},
            {Main::Languages::french, "PAUSE"},
            {Main::Languages::malagasy, "MIATO"}
        };

        // The continue button text languages
        std::map<Main::Languages, std::string> continue_lang{
            {Main::Languages::english, "CONTINUE"},
            {Main::Languages::french, "CONTINUEZ"},
            {Main::Languages::malagasy, "TOHIZANA"}
        };

        // The quit button text languages
        std::map<Main::Languages, std::string> quit_lang{
            {Main::Languages::english, "QUIT"},
            {Main::Languages::french, "QUITTER"},
            {Main::Languages::malagasy, "MIALA"}
        };

        // ========== ALL DRAWABLES ========== //

        bg_rect.setFillColor(sf::Color(255,255,255,25));

        menu_rect.setFillColor(sf::Color(100,100,100));

        Text(Title, title_lang.at(Main::lang()), Arial_font_path, 50, sf::Color(255,255,0));

        // The buttons' rect Outline size
        int outline_size = 5;

        Text(Continue, continue_lang.at(Main::lang()), Arial_font_path, 30, button_color);

        Continue_rect.setSize(sf::Vector2f(Continue.getGlobalBounds().size));
        Continue_rect.setFillColor(button_rect_color);
        Continue_rect.setOutlineThickness(outline_size);
        Continue_rect.setOutlineColor(Continue_rect.getFillColor());

        Text(Quit, quit_lang.at(Main::lang()), Arial_font_path, 30, button_color);

        Quit_rect.setSize(sf::Vector2f(Quit.getGlobalBounds().size));
        Quit_rect.setFillColor(button_rect_color);
        Quit_rect.setOutlineThickness(outline_size);
        Quit_rect.setOutlineColor(Quit_rect.getFillColor());

        Set_content_position();

        // ========== FIRST DRAWING ========== // 
        if (Main::first_drawing()){
            Debugging::write("----GAME'S PAUSE----\n", Debugging::get_state_of(Debugging::Description));
            Main::first_drawing(false);
        }   
        
        Input();
        Update_content();
        Render();
    }
}
