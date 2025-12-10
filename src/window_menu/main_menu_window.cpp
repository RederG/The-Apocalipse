#include "../../headers/window_menu/main_menu_window.hpp"

namespace MainMenuWindow{
    // All elements
    std::vector<std::pair<std::string, ContainManager::Types>> all_elements{
        {"title", ContainManager::Text},
        {"play", ContainManager::Buttons},
        {"set_lang", ContainManager::Buttons},
        {"made_by", ContainManager::Text},
        {"game_version", ContainManager::Text},
    };
    
    // The coordinates of the mouse
    sf::Vector2i mouse;

    float Color_alpha;

    void Render(){
        
        // The soldier's texture
        sf::Texture soldier_image;
        // The soldier's sprite
        sf::Sprite soldier_sprite(soldier_image);
        // The zombie's texture
        sf::Texture zombie_image;
        // The zombie's sprite
        sf::Sprite zombie_sprite(zombie_image);

        // Setting the soldier's sprite 
        if(!soldier_image.loadFromFile("../res/images/Entities/soldier.png"))
            Debugging::write("Soldier image not loaded");
        soldier_sprite.setTexture(soldier_image);
        soldier_sprite.setTextureRect(sf::IntRect({32,0},{32,32}));
        float x = Main::window()->getSize().x/4 - soldier_sprite.getGlobalBounds().size.x;
        float y = Main::window()->getSize().y/2 - soldier_sprite.getGlobalBounds().size.y*3;
        soldier_sprite.setPosition({x, y});
        soldier_sprite.setScale({5,5});

        // Setting the zombie's sprite 
        if(!zombie_image.loadFromFile("../res/images/Entities/zombie.png"))
            Debugging::write("Zombie image not loaded");
        zombie_sprite.setTexture(zombie_image);
        zombie_sprite.setTextureRect(sf::IntRect({32,64},{32,32}));
        x = (Main::window()->getSize().x/2 + Main::window()->getSize().x/4) - zombie_sprite.getGlobalBounds().size.x*4;
        y = Main::window()->getSize().y/2 - zombie_sprite.getGlobalBounds().size.y*3;
        zombie_sprite.setPosition({x, y});
        zombie_sprite.setScale({5,5});

        soldier_sprite.setColor(sf::Color(255,255,255,Color_alpha));
        zombie_sprite.setColor(sf::Color(255,255,255,Color_alpha));

        Main::window()->clear();

        Main::window()->draw(*Main::background());
        Main::context_content()->draw_all();
        Main::window()->draw(soldier_sprite);
        Main::window()->draw(zombie_sprite);

        Main::window()->display();
    }

    void Input(){
        while(std::optional main_menu_event = Main::window()->pollEvent()){
            Main::check_resize_events(main_menu_event);
            // if the user want to close the window
            if(main_menu_event->is<sf::Event::Closed>())
                Main::window()->close();

            // if a key is released
            if(main_menu_event->is<sf::Event::MouseButtonPressed>()){
                if(Main::context_content()->get_button("play")->contains(mouse)){
                    Debugging::write("Button : PLAY\n", Debugging::get_state_of(Debugging::Button));
                    Main::context_content()->get_button("play")->get_text()->setFillColor(sf::Color(255,0,0,255));
                    Main::window()->setFramerateLimit(60);
                    
                    while (Color_alpha > 0){
                        Main::context_content()->get_text("title")->setFillColor(sf::Color(255,100,20,Color_alpha));
                        Main::context_content()->get_text("play")->setFillColor(sf::Color(255,200,20,Color_alpha));
                        Main::context_content()->get_text("set_lang")->setFillColor(sf::Color(255,200,20,Color_alpha));
                        Main::context_content()->get_text("made_by")->setFillColor(sf::Color(255,255,255,Color_alpha));
                        Main::context_content()->get_text("game_version")->setFillColor(sf::Color(255,255,255,Color_alpha));

                        Render();
                        Color_alpha -= 10 ;
                    }
                    Color_alpha = 0;
                    Main::first_drawing(true);
                    Main::context(Main::Context::PlayMenu);
                }

                if(Main::context_content()->get_button("set_lang")->contains(mouse)){
                    Debugging::write("Button : SETTING LANGUAGE\n", Debugging::get_state_of(Debugging::Button));
                    Main::context_content()->get_button("set_lang")->get_text()->setFillColor(sf::Color(255,0,0));
                    Main::window()->setFramerateLimit(60);
                    
                    while (Color_alpha > 0){
                        Main::context_content()->get_text("title")->setFillColor(sf::Color(255,100,20,Color_alpha));
                        Main::context_content()->get_text("play")->setFillColor(sf::Color(255,200,20,Color_alpha));
                        Main::context_content()->get_text("set_lang")->setFillColor(sf::Color(255,200,20,Color_alpha));
                        Main::context_content()->get_text("made_by")->setFillColor(sf::Color(255,255,255,Color_alpha));
                        Main::context_content()->get_text("game_version")->setFillColor(sf::Color(255,255,255,Color_alpha));

                        Render();
                        Color_alpha -= 10 ;
                    }
                    Main::first_drawing(true);
                    Color_alpha = 0;
                    Main::context(Main::Context::SetLangMenu);
                }
            }
        }
    }

    void Update_content(){
        Color_alpha = 255;
        Main::context_content()->get_text("title")->setFillColor(sf::Color(255,100,20,Color_alpha));
        Main::context_content()->get_text("play")->setFillColor(sf::Color(255,200,20,Color_alpha));
        Main::context_content()->get_text("set_lang")->setFillColor(sf::Color(255,200,20,Color_alpha));
        Main::context_content()->get_text("made_by")->setFillColor(sf::Color(255,255,255,Color_alpha));
        Main::context_content()->get_text("game_version")->setFillColor(sf::Color(255,255,255,Color_alpha));

        mouse = sf::Mouse::getPosition(*Main::window());

        if(Main::context_content()->get_button("play")->contains(mouse))
            Main::context_content()->get_button("play")->get_text()->setFillColor(sf::Color(255,0,0));
        else
            Main::context_content()->get_button("play")->get_text()->setFillColor(sf::Color(255,200,20));
        
        if(Main::context_content()->get_button("set_lang")->contains(mouse))
            Main::context_content()->get_button("set_lang")->get_text()->setFillColor(sf::Color(255,0,0));
        else
            Main::context_content()->get_button("set_lang")->get_text()->setFillColor(sf::Color(255,200,20));
    }

    void Set_content_position(){
        // Setting all texts' parameters 
        Main::context_content()->get_text("title")->setPosition(sf::Vector2f(Main::window()->getSize().x/2-Main::context_content()->get_text("title")->getGlobalBounds().size.x/2, 20));
        Main::context_content()->get_text("play")->setPosition(sf::Vector2f(Main::window()->getSize().x/2-Main::context_content()->get_text("play")->getGlobalBounds().size.x/2, 200));
        Main::context_content()->get_text("set_lang")->setPosition(sf::Vector2f(Main::window()->getSize().x/2-Main::context_content()->get_text("set_lang")->getGlobalBounds().size.x/2, 300));
        Main::context_content()->get_text("made_by")->setPosition(sf::Vector2f(0, Main::window()->getSize().y-50));
        Main::context_content()->get_text("game_version")->setPosition(sf::Vector2f(Main::window()->getSize().x-Main::context_content()->get_text("game_version")->getGlobalBounds().size.x, Main::window()->getSize().y-50));
    }

    void Event(){
        Main::window()->setFramerateLimit(60);

        Main::context_content()->clear();
        Main::context_content()->set_window_to(*Main::window());
        Main::context_content()->set_priority_list_to(&all_elements);
            
        // All texts
        Main::context_content()->add_text("title");
        Main::context_content()->add_text("play");
        Main::context_content()->add_text("set_lang");
        Main::context_content()->add_text("made_by");
        Main::context_content()->add_text("game_version");
        Main::context_content()->add_button("play", Main::context_content()->get_text("play"));
        Main::context_content()->add_button("set_lang", Main::context_content()->get_text("set_lang"));

        // The arial font
        sf::Font Arial = *FontManager::get_font(FontManager::Arial);

        // all texts in their languages
        std::map<Main::Languages, sf::String> play_lang{
            {Main::Languages::english, L" Play "},
            {Main::Languages::french, L"Jouer"},
            {Main::Languages::malagasy, L"Milalao"}
        };

        std::map<Main::Languages, std::string> set_lang_lang{
            {Main::Languages::english, "Language"},
            {Main::Languages::french, "Language"},
            {Main::Languages::malagasy, "Fiteny"}
        };

        std::map<Main::Languages, sf::String> Made_by_lang{
            {Main::Languages::english, " Made by Reder"},
            {Main::Languages::french, L" Créé par Reder"},
            {Main::Languages::malagasy, " Vitan'i Reder"}
        };

        // All texts' configuration
        Main::context_content()->configure_text("title", "THE APOCALIPSE", &Arial, 100);
        Main::context_content()->configure_text("play", play_lang.at(Main::lang()), &Arial, 70);
        Main::context_content()->configure_text("set_lang", set_lang_lang.at(Main::lang()), &Arial, 70);
        Main::context_content()->configure_text("made_by", Made_by_lang.at(Main::lang()), &Arial, 30);
        Main::context_content()->configure_text("game_version", "v1.0.0 ", &Arial, 30);
        Main::context_content()->add_button("play", Main::context_content()->get_text("play"));
        Main::context_content()->add_button("set_lang", Main::context_content()->get_text("set_lang"));

        Set_content_position();

        if(Main::first_drawing()){
            Debugging::write("----MAIN MENU----\n", Debugging::get_state_of(Debugging::Description));

            Color_alpha = 0;
            Main::window()->setFramerateLimit(60);

            while (Color_alpha<255){
                Main::context_content()->get_text("title")->setFillColor(sf::Color(255,100,20,Color_alpha));
                Main::context_content()->get_text("play")->setFillColor(sf::Color(255,200,20,Color_alpha));
                Main::context_content()->get_text("set_lang")->setFillColor(sf::Color(255,200,20,Color_alpha));
                Main::context_content()->get_text("made_by")->setFillColor(sf::Color(255,255,255,Color_alpha));
                Main::context_content()->get_text("game_version")->setFillColor(sf::Color(255,255,255,Color_alpha));

                Render();
                Color_alpha += 5;
            };
            
            Main::first_drawing(false);
            Main::window()->setFramerateLimit(0);
        }

        Update_content();
        Input();
        Render();
    }
}
