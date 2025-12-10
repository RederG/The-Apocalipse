#include "../../headers/main.hpp"
#include "../../headers/window_menu/player_data_window.hpp"
#include "../../headers/window_menu/world_settings.hpp"

namespace PlayerDataWindow{
    std::string player_gaming_time;

    sf::Font Arial;

    const int TITLE_SIZE = 50;
    const int INFO_SIZE = 30;
    const int BUTTON_SIZE = 25;

    bool start_game = false;
    bool continue_game = false;
    bool replay_game = false;

    // This function constructs a SFML text
    void Text(sf::Text &txt_name, std::string text, sf::Font &font, int size, sf::Color color){
        txt_name.setString(text);
        txt_name.setFont(font);
        txt_name.setCharacterSize(size);
        txt_name.setFillColor(color);
    }

    void delete_player(std::string player){
        std::string file_path = "../Players/" + player + "/data.save";
        std::filesystem::remove(file_path);
        Debugging::write("REMOVING " + file_path + " terminated", Debugging::get_state_of(Debugging::File));
        std::filesystem::remove_all("../Players/"+player);
        Debugging::write("REMOVING " + player + " terminated", Debugging::get_state_of(Debugging::File));
    }

    Data get_player_data(std::string player){
        Data player_data;
        std::string path = "../Players/" + player + "/data.save";
        std::fstream file;
        if(!std::filesystem::exists(path)){
            file.open(path, std::ios::out);
            file.close();
        }

        file.open(path, std::ios::in | std::ios::binary);

        int score = 0;
        file.read(reinterpret_cast<char*>(&score), sizeof(int));
        player_data.score = score;

        int best_score = 0;
        file.read(reinterpret_cast<char*>(&best_score), sizeof(int));
        player_data.best_score = best_score;

        int time = 0;
        file.read(reinterpret_cast<char*>(&time), sizeof(int));
        player_data.time = time;

        bool dead = false;
        file.read(reinterpret_cast<char*>(&dead), sizeof(bool));
        player_data.dead = dead;

        file.read(reinterpret_cast<char*>(&player_data.first_play), sizeof(bool));
        file.read(reinterpret_cast<char*>(&player_data.world_seed), sizeof(long long));

        return player_data;
    }

    void Render(){
        Main::context_content()->draw_all();
        Main::window()->display();
    }

    void Set_content_position(){
        int space = 10;
        sf::Vector2f main_rect_size;
        sf::Vector2f title_txt_size    = Main::context_content()->get_text("title")->getGlobalBounds().size;
        sf::Vector2f score_txt_size    = Main::context_content()->get_text("score")->getGlobalBounds().size;
        sf::Vector2f best_score_txt_size   = Main::context_content()->get_text("best_score")->getGlobalBounds().size;
        sf::Vector2f time_txt_size     = Main::context_content()->get_text("time")->getGlobalBounds().size;
        sf::Vector2f start_txt_size    = Main::context_content()->get_text_rect("start")->getGlobalBounds().size;
        sf::Vector2f continue_txt_size = Main::context_content()->get_text_rect("continue")->getGlobalBounds().size;
        sf::Vector2f replay_txt_size   = Main::context_content()->get_text_rect("replay")->getGlobalBounds().size;
        sf::Vector2f settings_txt_size    = Main::context_content()->get_text_rect("settings")->getGlobalBounds().size;
        sf::Vector2f delete_txt_size   = Main::context_content()->get_text_rect("delete")->getGlobalBounds().size;
        sf::Vector2f back_txt_size   = Main::context_content()->get_text("back")->getGlobalBounds().size;

        main_rect_size.y = TITLE_SIZE + INFO_SIZE*3 + BUTTON_SIZE + space*6;

        if(start_game == true)
            main_rect_size.x = start_txt_size.x;
        
        if(continue_game == true)
            main_rect_size.x = continue_txt_size.x;
        
        if(replay_game == true)
            main_rect_size.x = replay_txt_size.x;
        
        main_rect_size.x += settings_txt_size.x + delete_txt_size.x + space*3;

        Main::context_content()->configure_rect("main_rect", main_rect_size, sf::Color(100,100,100), sf::Color(100,100,100), 0);
        sf::Color color = Main::context_content()->get_rect("main_rect")->getFillColor();

        Main::context_content()->set_position(Main::context_content()->get_rect("main_rect"), true, true);
        sf::Vector2f main_rect_position = Main::context_content()->get_rect("main_rect")->getGlobalBounds().position;
        Main::context_content()->get_text("back")->setPosition({main_rect_position.x - back_txt_size.x - space, main_rect_position.y});

        float y = 0;
        Main::context_content()->get_text("title")->setPosition({0, main_rect_position.y - 20});
        y = Main::context_content()->get_text("title")->getGlobalBounds().position.y + title_txt_size.y + space;
        Main::context_content()->get_text("score")->setPosition(sf::Vector2f(0, y));
        y = Main::context_content()->get_text("score")->getGlobalBounds().position.y + score_txt_size.y + space;
        Main::context_content()->get_text("best_score")->setPosition(sf::Vector2f(0, y));
        y = Main::context_content()->get_text("best_score")->getGlobalBounds().position.y + best_score_txt_size.y + space;
        Main::context_content()->get_text("time")->setPosition(sf::Vector2f(0, y));
        y = Main::context_content()->get_text("time")->getGlobalBounds().position.y + time_txt_size.y + space;

        Main::context_content()->set_position(Main::context_content()->get_text("title"), true);
        Main::context_content()->set_position(Main::context_content()->get_text("score"), true);
        Main::context_content()->set_position(Main::context_content()->get_text("best_score"), true);
        Main::context_content()->set_position(Main::context_content()->get_text("time"), true);
        
        if(start_game == true){
            Main::context_content()->get_text("start")->setPosition({main_rect_position.x + 10, y});
            Main::context_content()->get_text("settings")->setPosition({main_rect_position.x + start_txt_size.x + space*2, y});
            float x = main_rect_position.x + start_txt_size.x + settings_txt_size.x + space*3;
            Main::context_content()->get_text("delete")->setPosition({x, y});
        }

        if(continue_game == true){
            Main::context_content()->get_text("continue")->setPosition({main_rect_position.x + 10, y});
            Main::context_content()->get_text("settings")->setPosition({main_rect_position.x + continue_txt_size.x + space*2, y});
            float x = main_rect_position.x + continue_txt_size.x + settings_txt_size.x + space*3;
            Main::context_content()->get_text("delete")->setPosition({x, y});
        }
        
        if(replay_game == true){
            Main::context_content()->get_text("replay")->setPosition({main_rect_position.x + 10, y});
            Main::context_content()->get_text("settings")->setPosition({main_rect_position.x + replay_txt_size.x + space*2, y});
            float x = main_rect_position.x + replay_txt_size.x + settings_txt_size.x + space*3;
            Main::context_content()->get_text("delete")->setPosition({x, y});
        }

        Main::context_content()->updade_text_rect("start");
        Main::context_content()->updade_text_rect("continue");
        Main::context_content()->updade_text_rect("settings");
        Main::context_content()->updade_text_rect("delete");
        Main::context_content()->updade_text_rect("replay");
        Main::context_content()->updade_text_rect("back");
    }

    void Update_content(){
        if(Main::context_content()->get_button("back")->contains(sf::Mouse::getPosition(*Main::window())))
            Main::context_content()->get_button("back")->get_text()->setFillColor(sf::Color::Black);
        else
            Main::context_content()->get_button("back")->get_text()->setFillColor(sf::Color::White);

        if(Main::context_content()->get_button("settings")->contains(sf::Mouse::getPosition(*Main::window())))
            Main::context_content()->get_button("settings")->get_text()->setFillColor(sf::Color(0,100,255));
        else
            Main::context_content()->get_button("settings")->get_text()->setFillColor(sf::Color(0,255,255));

        if(Main::context_content()->get_button("delete")->contains(sf::Mouse::getPosition(*Main::window())))
            Main::context_content()->get_button("delete")->get_text()->setFillColor(sf::Color(0,100,255));
        else
            Main::context_content()->get_button("delete")->get_text()->setFillColor(sf::Color(0,255,255));

        if(start_game == true){
            if(Main::context_content()->get_button("start")->contains(sf::Mouse::getPosition(*Main::window())))
                Main::context_content()->get_button("start")->get_text()->setFillColor(sf::Color(0,100,255));
            else
                Main::context_content()->get_button("start")->get_text()->setFillColor(sf::Color(0,255,255));
        }
        if(continue_game == true){
            if(Main::context_content()->get_button("continue")->contains(sf::Mouse::getPosition(*Main::window())))
                Main::context_content()->get_button("continue")->get_text()->setFillColor(sf::Color(0,100,255));
            else
                Main::context_content()->get_button("continue")->get_text()->setFillColor(sf::Color(0,255,255));
        }
        if(replay_game == true){
            if(Main::context_content()->get_button("replay")->contains(sf::Mouse::getPosition(*Main::window())))
                Main::context_content()->get_button("replay")->get_text()->setFillColor(sf::Color(0,100,255));
            else
                Main::context_content()->get_button("replay")->get_text()->setFillColor(sf::Color(0,255,255));
        }

        if(WorldSettingsWindow::window_resized){
            Main::context(Main::Context::WorldSettings);
            Main::first_drawing(true);
            Render();
        }
    }

    void Input(){
        while(std::optional event = Main::window()->pollEvent()){
            if(Main::check_resize_events(event)){
                Main::context(Main::Context::PlayMenu);
                Main::first_drawing(true);
            }

            if(event->is<sf::Event::Closed>())
                Main::window()->close();

            if(event->is<sf::Event::MouseButtonPressed>()){
                if(start_game == true){
                    if(Main::context_content()->get_button("start")->contains(sf::Mouse::getPosition(*Main::window()))){
                        Main::context(Main::Context::TheGame);
                        Main::first_drawing(true);
                    }
                }

                if(continue_game == true){
                    if(Main::context_content()->get_button("continue")->contains(sf::Mouse::getPosition(*Main::window()))){
                        Main::context(Main::Context::TheGame);
                        Main::first_drawing(true);
                    }
                }

                if(replay_game == true){
                    if(Main::context_content()->get_button("replay")->contains(sf::Mouse::getPosition(*Main::window()))){
                        Main::context(Main::Context::TheGame);
                        Main::first_drawing(true);
                    }
                }

                if(Main::context_content()->get_button("settings")->contains(sf::Mouse::getPosition(*Main::window()))){
                    Main::context(Main::Context::WorldSettings);
                    Main::first_drawing(true);
                }
                
                if(Main::context_content()->get_button("delete")->contains(sf::Mouse::getPosition(*Main::window()))){
                    delete_player(player_choosed);
                    Main::context(Main::Context::PlayMenu);
                    Main::first_drawing(true);
                    player_choosed.clear();
                }

                if(Main::context_content()->get_button("back")->contains(sf::Mouse::getPosition(*Main::window()))){
                    Main::context(Main::Context::PlayMenu);
                    Main::first_drawing(true);
                    player_choosed.clear();
                }
            }
        }
    }

    void Event(){
        Main::window()->setFramerateLimit(60);
        Main::context_content()->clear();
        Main::context_content()->set_window_to(*Main::window());

        sf::Font font = *FontManager::get_font(FontManager::Arial);

        // ========== ALL TEXTS' LANGUAGES ========= //

        // The player's score text languages
        std::map<Main::Languages, std::string> score_txt{
            {Main::Languages::english, "Score : "},
            {Main::Languages::french, "Score : "},
            {Main::Languages::malagasy, "Maty : "}
        };

        // The player's best score text languages
        std::map<Main::Languages, std::string> best_score_txt{
            {Main::Languages::english, "Best score : "},
            {Main::Languages::french, "Meilleur score : "},
            {Main::Languages::malagasy, "Maty lehibe : "}
        };

        // The player's gaming time text languages
        std::map<Main::Languages, std::string> gaming_time_txt{
            {Main::Languages::english, "Gaming time : "},
            {Main::Languages::french, "Temps de jeu : "},
            {Main::Languages::malagasy, "Lera lany : "}
        };

        // The continue button text languages
        std::map<Main::Languages, std::string> continue_txt{
            {Main::Languages::english, "CONTINUE"},
            {Main::Languages::french, "CONTINUEZ"},
            {Main::Languages::malagasy, "TOHIZANA"}
        };

        // The continue button text languages
        std::map<Main::Languages, std::string> replay_txt{
            {Main::Languages::english, "REPLAY"},
            {Main::Languages::french, "REFAIRE"},
            {Main::Languages::malagasy, "AVERINA"}
        };

        // The begin button text languages
        std::map<Main::Languages, std::string> start_txt{
            {Main::Languages::english, "START"},
            {Main::Languages::french, "COMMENCER"},
            {Main::Languages::malagasy, "MANOMBOKA"}
        };

        // The reset button text languages
        std::map<Main::Languages, sf::String> settings_txt{
            {Main::Languages::english, "SETTINGS"},
            {Main::Languages::french, L"PARAM."},
            {Main::Languages::malagasy, "TOETRA"}
        };

        // The delete button text languages
        std::map<Main::Languages, std::string> delete_txt{
            {Main::Languages::english, "DELETE"},
            {Main::Languages::french, "SUPPRIMER"},
            {Main::Languages::malagasy, "ESORINA"}
        };

        // ========= CONFIGURATIONS ========= // 

        Main::context_content()->add_rect("main_rect");
        Main::context_content()->add_text("title", false);
        Main::context_content()->add_text("back", true);
        Main::context_content()->add_text("score", false);
        Main::context_content()->add_text("best_score", false);
        Main::context_content()->add_text("time", false);
        Main::context_content()->add_text("start", true);
        Main::context_content()->add_text("continue", true);
        Main::context_content()->add_text("replay", true);
        Main::context_content()->add_text("settings", true);
        Main::context_content()->add_text("delete", true);
        Main::context_content()->add_button("start", Main::context_content()->get_text("start"));
        Main::context_content()->add_button("continue", Main::context_content()->get_text("continue"));
        Main::context_content()->add_button("replay", Main::context_content()->get_text("replay"));
        Main::context_content()->add_button("settings", Main::context_content()->get_text("settings"));
        Main::context_content()->add_button("delete", Main::context_content()->get_text("delete"));
        Main::context_content()->add_button("back", Main::context_content()->get_text("back"));

        // Getting all player's data
        Data player_data = get_player_data(player_choosed);
        Main::sec_conversion(player_data.time, player_gaming_time);

        if(player_data.time == 0){
            start_game = true;
            continue_game = false;
            replay_game = false;
        }
        else if(player_data.dead){
            start_game = false;
            continue_game = false;
            replay_game = true;
        }
        else {
            start_game = false;
            continue_game = true;
            replay_game = false;
        }

        if(player_data.time == 0 && (player_data.score > 0 || player_data.best_score > 0)){
            std::fstream file("../Players/" + player_choosed + "/data.save", std::ios::binary | std::ios::out);
            if(file.is_open()) file.close();
        }
        
        Main::context_content()->configure_text("title", player_choosed, &font, TITLE_SIZE, sf::Color(255,255,0));
        Main::context_content()->configure_text("back", " < ", &font, 35, sf::Color::White);
        Main::context_content()->configure_text("score", score_txt.at(Main::lang()) + std::to_string(player_data.score), &font, INFO_SIZE, sf::Color::White);
        Main::context_content()->configure_text("best_score", best_score_txt.at(Main::lang()) + std::to_string(player_data.best_score), &font, INFO_SIZE, sf::Color::White);
        Main::context_content()->configure_text("time", gaming_time_txt.at(Main::lang()) + player_gaming_time, &font, INFO_SIZE, sf::Color::White);
        Main::context_content()->configure_text("start", start_txt.at(Main::lang()), &font, BUTTON_SIZE, sf::Color(0,255,255));
        Main::context_content()->configure_text("continue", continue_txt.at(Main::lang()), &font, BUTTON_SIZE, sf::Color(0,255,255));
        Main::context_content()->configure_text("replay", replay_txt.at(Main::lang()), &font, BUTTON_SIZE, sf::Color(0,255,255));
        Main::context_content()->configure_text("settings", settings_txt.at(Main::lang()), &font, BUTTON_SIZE, sf::Color(0,255,255));
        Main::context_content()->configure_text("delete", delete_txt.at(Main::lang()), &font, BUTTON_SIZE, sf::Color(0,255,255));
        
        Main::context_content()->configure_text_rect("start", sf::Color(0,0,255), sf::Color(0,0,255), 5);
        Main::context_content()->configure_text_rect("continue", sf::Color(0,0,255), sf::Color(0,0,255), 5);
        Main::context_content()->configure_text_rect("settings", sf::Color(0,0,255), sf::Color(0,0,255), 5);
        Main::context_content()->configure_text_rect("delete", sf::Color(0,0,255), sf::Color(0,0,255), 5);
        Main::context_content()->configure_text_rect("replay", sf::Color(0,0,255), sf::Color(0,0,255), 5);
        Main::context_content()->configure_text_rect("back", sf::Color(100,100,100), sf::Color(100,100,100), 5);

        Main::context_content()->updade_text_rect("start");
        Main::context_content()->updade_text_rect("continue");
        Main::context_content()->updade_text_rect("settings");
        Main::context_content()->updade_text_rect("delete");
        Main::context_content()->updade_text_rect("replay");
        Main::context_content()->updade_text_rect("back");

        std::vector<std::pair<std::string, ContainManager::Types>> elements_start_game{
            {"main_rect", ContainManager::Rect},
            {"title", ContainManager::Text},
            {"time", ContainManager::Text},
            {"back", ContainManager::Buttons},
            {"delete", ContainManager::Buttons},
            {"settings", ContainManager::Buttons},
            {"start", ContainManager::Buttons},
            {"best_score", ContainManager::Text},
            {"score", ContainManager::Text},
        };

        std::vector<std::pair<std::string, ContainManager::Types>> elements_continue_game{
            {"main_rect", ContainManager::Rect},
            {"time", ContainManager::Text},
            {"back", ContainManager::Buttons},
            {"delete", ContainManager::Buttons},
            {"settings", ContainManager::Buttons},
            {"continue", ContainManager::Buttons},
            {"best_score", ContainManager::Text},
            {"score", ContainManager::Text},
            {"title", ContainManager::Text},
        };

        std::vector<std::pair<std::string, ContainManager::Types>> elements_replay_game{
            {"main_rect", ContainManager::Rect},
            {"time", ContainManager::Text},
            {"back", ContainManager::Buttons},
            {"delete", ContainManager::Buttons},
            {"settings", ContainManager::Buttons},
            {"replay", ContainManager::Buttons},
            {"best_score", ContainManager::Text},
            {"score", ContainManager::Text},
            {"title", ContainManager::Text},
        };

        if(start_game == true)
            Main::context_content()->set_priority_list_to(&elements_start_game);
        if(continue_game == true)
            Main::context_content()->set_priority_list_to(&elements_continue_game);
        if(replay_game == true)
            Main::context_content()->set_priority_list_to(&elements_replay_game);

        Set_content_position();
            
        if(Main::first_drawing()){
            Debugging::write("----PLAYER DATA----");
            Main::first_drawing(false);
        }

        Input();
        Update_content();
        Render();
    }
}
