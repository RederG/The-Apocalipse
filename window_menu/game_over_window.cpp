#include "game_over_window.hpp"

namespace GameOverWindow{
    std::vector<std::pair<std::string, ContainManager::Types>> all_elements{
        {"game_over_black_rect", ContainManager::Rect},
        {"game_over_txt", ContainManager::Text},
        {"time_on_game_txt", ContainManager::Text},
        {"score_txt", ContainManager::Text},
        {"back_to_menu_button", ContainManager::Buttons},
    };

    // Save the progress of the player
    void save_progress(bool dead){
        Map::save_all("../Players/" + player_choosed + "/");

        std::string tombs_path, entities_path, inventories_path, items_path, effect_managers_path, effects_path;
        tombs_path = "../Players/" + player_choosed + "/InteractiveObjects/Tombs";
        entities_path = "../Players/" + player_choosed + "/Entities";
        inventories_path = "../Players/" + player_choosed + "/Containers";
        items_path = "../Players/" + player_choosed + "/Items";
        effect_managers_path = "../Players/" + player_choosed + "/Effect_managers";
        effects_path = "../Players/" + player_choosed + "/Effects";

        Entity::save_all_to(entities_path, inventories_path, items_path, effect_managers_path, effects_path);
        Tomb::save_all_to(tombs_path, inventories_path, items_path);

        std::string path = "../Players/" + player_choosed + "/data.save";
        std::fstream file;
        bool first_play = false;
        if(!std::filesystem::exists(path)){
            file.open(path, std::ios::out);
            file.close();
            first_play = true;
        }

        int best_score = 0;
        if(!file.eof()){
            file.open(path, std::ios::in | std::ios::binary);
            if(file.is_open()){
                file.seekg(sizeof(int), std::ios::beg);
                file.read(reinterpret_cast<char*>(&best_score), sizeof(int));

                file.close();
            }
        }
        file.open(path, std::ios::out | std::ios::binary);
        if(file.is_open()){
            int score = GameWindow::zombie_killed;
            file.write(reinterpret_cast<char*>(&score), sizeof(int));

            if(score > best_score)
                file.write(reinterpret_cast<char*>(&score), sizeof(int));
            else
                file.write(reinterpret_cast<char*>(&best_score), sizeof(int));
            
            int time = GameWindow::time;
            file.write(reinterpret_cast<char*>(&time), sizeof(int));

            bool is_dead = dead;
            file.write(reinterpret_cast<char*>(&is_dead), sizeof(bool));

            file.write(reinterpret_cast<char*>(&first_play), sizeof(bool));

            long long seed = Map::get(Map::NAME)->get_noise()->get_seed();
            file.write(reinterpret_cast<char*>(&seed), sizeof(long long));

            file.close();
        }

        GameWindow::player_position = sf::Vector2f(0, 0);
        GameWindow::time = 0;
        GameWindow::zombie_killed = 0; 
    }

    void Render(){
        Main::window()->clear();
            
        Entity::set_entities_movement_to(false);
        GameWindow::draw(true);
        Main::context_content()->draw_all();
        
        Main::window()->display();
    }

    void Set_content_position(){
        float y = 0;

        Main::context_content()->get_text("game_over_txt")->setPosition({0, 100});

        Main::context_content()->set_position(Main::context_content()->get_text("game_over_txt"), true);
        y = Main::context_content()->get_text("game_over_txt")->getGlobalBounds().position.y + Main::context_content()->get_text("game_over_txt")->getGlobalBounds().size.y;
        Main::context_content()->get_text("time_on_game_txt")->setPosition({0, y});

        y = Main::context_content()->get_text("time_on_game_txt")->getGlobalBounds().position.y + Main::context_content()->get_text("time_on_game_txt")->getGlobalBounds().size.y;
        Main::context_content()->get_text("score_txt")->setPosition({0, y});

        y = Main::context_content()->get_text("score_txt")->getGlobalBounds().position.y + Main::context_content()->get_text("score_txt")->getGlobalBounds().size.y;
        Main::context_content()->get_text("back_to_menu_txt")->setPosition({0, y});

        Main::context_content()->set_position(Main::context_content()->get_text("time_on_game_txt"), true);
        Main::context_content()->set_position(Main::context_content()->get_text("score_txt"), true);
        Main::context_content()->set_position(Main::context_content()->get_text("back_to_menu_txt"), true);

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
                if(Main::context_content()->get_button("back_to_menu_button")->contains(sf::Mouse::getPosition(*Main::window()))){
                    save_progress();
                    TheApocalipse::terminates();
                    Main::context(Main::Context::PlayMenu);
                    player_choosed = "";

                    Main::first_drawing(true);
                }
            }
        }
    }

    void Update_content(){
        Main::context_content()->get_rect("game_over_black_rect")->setFillColor(sf::Color(0,0,0,150));
        Main::context_content()->get_rect("game_over_black_rect")->setSize(sf::Vector2f(Main::window()->getSize()));

        if(Main::context_content()->get_button("back_to_menu_button")->contains(sf::Mouse::getPosition(*Main::window())))
            Main::context_content()->get_button("back_to_menu_button")->get_text()->setFillColor(sf::Color(255,10,10));
        else
            Main::context_content()->get_button("back_to_menu_button")->get_text()->setFillColor(sf::Color(255,255,0));
    }

    void Event(){
        Main::window()->setFramerateLimit(60);

        sf::Font* font = FontManager::get_font(FontManager::Arial);

        std::string time;
        Main::sec_conversion(GameWindow::time, time);

        const std::map<Main::Languages, sf::String> game_over_txt{
            {Main::Languages::english, " GAME OVER "},
            {Main::Languages::french, L" PARTIE TERMINé "},
            {Main::Languages::malagasy, " VITA NY LALAO "}
        };

        const std::map<Main::Languages, sf::String> time_on_game_txt{
            {Main::Languages::english, "Time on game : "},
            {Main::Languages::french, L"Temps de jeu : "},
            {Main::Languages::malagasy, "Aelan'ny lalao : "}
        };

        const std::map<Main::Languages, sf::String> score_txt{
            {Main::Languages::english, "Score : "},
            {Main::Languages::french, L"Score : "},
            {Main::Languages::malagasy, "Maty : "}
        };

        const std::map<Main::Languages, sf::String> back_to_menu_txt{
            {Main::Languages::english, " BACK "},
            {Main::Languages::french, L" RETOUR "},
            {Main::Languages::malagasy, " MIVERINA "}
        };

        Main::context_content()->clear();
        Main::context_content()->set_priority_list_to(&all_elements);
        
        Main::context_content()->add_text("game_over_txt");
        Main::context_content()->add_text("time_on_game_txt");
        Main::context_content()->add_text("score_txt");
        Main::context_content()->add_text("back_to_menu_txt");
        Main::context_content()->add_button("back_to_menu_button", Main::context_content()->get_text("back_to_menu_txt"));
        Main::context_content()->add_rect("game_over_black_rect");

        Main::context_content()->configure_text("game_over_txt", game_over_txt.at(Main::lang()), &*font, 50, sf::Color::Red);
        Main::context_content()->configure_text("time_on_game_txt", time_on_game_txt.at(Main::lang()) + time, &*font, 30, sf::Color::White);
        Main::context_content()->configure_text("score_txt", score_txt.at(Main::lang()) + to_string(GameWindow::zombie_killed), &*font, 30, sf::Color::White);
        Main::context_content()->configure_text("back_to_menu_txt", back_to_menu_txt.at(Main::lang()), &*font, 30, sf::Color(255,255,0));

        Set_content_position();
        
        if(Main::first_drawing()){
            Main::first_drawing(false);
        }

        Input();
        Update_content();
        Render();
    }
}