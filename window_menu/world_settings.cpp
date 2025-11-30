#include "world_settings.hpp"
#include "play_window.hpp"

namespace WorldSettingsWindow{
    sf::Vector2i mouse;

    std::vector<int> terrain_gen_values = {1, 3, 5, 7, 10, 13, 15, 18, 20, 23};
    std::vector<int> zone_update_values = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    int terrain_current_choice = 0;
    int zone_current_choice = 0;
    long long seed = 0;
    bool show_keys = true;
    bool window_resized = false;

    std::vector<std::pair<std::string, ContainManager::Types>> all_elements{
        {ElementsNames::settings_rect, ContainManager::Types::Rect},
        {ElementsNames::title, ContainManager::Types::Text},
        {ElementsNames::terrain_generation_txt, ContainManager::Types::Text},
        {ElementsNames::zone_update_txt, ContainManager::Types::Text},
        {ElementsNames::show_keys_txt, ContainManager::Types::Text},
        {ElementsNames::save_button, ContainManager::Types::Buttons},
        {ElementsNames::reset_button, ContainManager::Types::Buttons},
        {ElementsNames::back_button, ContainManager::Types::Buttons},
        {ElementsNames::terrain_generation_selector, ContainManager::Types::Selector},
        {ElementsNames::zone_update_selector, ContainManager::Types::Selector},
    };

    void save_settings_from(std::string player_name){
        std::fstream file("../Players/" + player_name + "/World/settings.save", std::ios::binary | std::ios::out);
        std::fstream file_read("../Players/" + player_choosed + "/data.save", std::ios::in | std::ios::binary);
        if(file_read.is_open()){
            file_read.seekg(sizeof(int)*3 + sizeof(bool)*2, std::ios::beg);
            file_read.read(reinterpret_cast<char*>(&seed), sizeof(long long));
            file_read.close();
        }
        if(file.is_open()){
            file.write(reinterpret_cast<char*>(&terrain_gen_values[terrain_current_choice]), sizeof(int));
            file.write(reinterpret_cast<char*>(&zone_update_values[zone_current_choice]), sizeof(int));
            file.write(reinterpret_cast<char*>(&seed), sizeof(long long));
            file.write(reinterpret_cast<char*>(&show_keys), sizeof(bool));
            file.close();
        }
    }

    Settings get_settings_from(std::string player_name){
        Settings world_settings;
        bool settings_file_exists = std::filesystem::exists("../Players/" + player_name + "/World/settings.save");
        if(!settings_file_exists){
            std::fstream file_read("../Players/" + player_choosed + "/data.save", std::ios::in | std::ios::binary);
            if(file_read.is_open()){
                file_read.seekg(sizeof(int)*3 + sizeof(bool)*2, std::ios::beg);
                file_read.read(reinterpret_cast<char*>(&world_settings.seed), sizeof(long long));
                file_read.close();
            }
            if(!std::filesystem::exists("../Players/" + player_name + "/World"))
                std::filesystem::create_directory("../Players/" + player_name + "/World");
            std::fstream file("../Players/" + player_name + "/World/settings.save", std::ios::out);
            if(file.is_open()) file.close();

            terrain_current_choice = 3;
            zone_current_choice = 0;
            seed = world_settings.seed;
            show_keys = true;
            save_settings_from(player_name);

            world_settings.terrain_generation_size = 5;
            world_settings.zone_update_size = 1;
            world_settings.show_keys = true;
        }
        else if(settings_file_exists){
            std::fstream file("../Players/" + player_name + "/World/settings.save", std::ios::binary | std::ios::in);
            if(file.is_open()){
                file.read(reinterpret_cast<char*>(&world_settings.terrain_generation_size), sizeof(int));
                file.read(reinterpret_cast<char*>(&world_settings.zone_update_size), sizeof(int));
                file.read(reinterpret_cast<char*>(&world_settings.seed), sizeof(long long));
                file.read(reinterpret_cast<char*>(&world_settings.show_keys), sizeof(bool));
                file.close();
            }
        }
        return world_settings;
    }

    void Render(){
        Main::context_content()->draw_all();
        Main::window()->display();
    }

    void Set_content_position(){
        const int SPACE = 20;
        Main::context_content()->configure_rect(ElementsNames::settings_rect, {550, 380}, sf::Color(100,100,100), sf::Color(100,100,100), 0);
        Main::context_content()->set_position(Main::context_content()->get_rect(ElementsNames::settings_rect), true, true);

        Main::context_content()->get_text(ElementsNames::title)->setPosition({0, Main::context_content()->get_rect(ElementsNames::settings_rect)->getPosition().y});
        Main::context_content()->set_position(Main::context_content()->get_text(ElementsNames::title), true, false);
        Main::context_content()->set_position(Main::context_content()->get_selector(ElementsNames::terrain_generation_selector), true, false);
        Main::context_content()->set_position(Main::context_content()->get_selector(ElementsNames::zone_update_selector), true, false);

        sf::Vector2f position = {Main::context_content()->get_selector(ElementsNames::terrain_generation_selector)->get_position().x, Main::context_content()->get_text(ElementsNames::title)->getPosition().y};
        position.y += Main::context_content()->get_text(ElementsNames::title)->getGlobalBounds().size.y + SPACE;

        Main::context_content()->get_text(ElementsNames::terrain_generation_txt)->setPosition(position);
        position.y += Main::context_content()->get_text(ElementsNames::terrain_generation_txt)->getGlobalBounds().size.y + SPACE;

        Main::context_content()->get_selector(ElementsNames::terrain_generation_selector)->set_position_to(position);
        Main::context_content()->set_position(Main::context_content()->get_selector(ElementsNames::terrain_generation_selector), true, false);
        position.y += Main::context_content()->get_selector(ElementsNames::terrain_generation_selector)->get_size().y + SPACE;

        Main::context_content()->get_text(ElementsNames::zone_update_txt)->setPosition(position);
        position.y += Main::context_content()->get_text(ElementsNames::zone_update_txt)->getGlobalBounds().size.y + SPACE;

        Main::context_content()->get_selector(ElementsNames::zone_update_selector)->set_position_to(position);
        Main::context_content()->set_position(Main::context_content()->get_selector(ElementsNames::zone_update_selector), true, false);
        position.y += Main::context_content()->get_selector(ElementsNames::zone_update_selector)->get_size().y + SPACE;

        Main::context_content()->get_text(ElementsNames::show_keys_txt)->setPosition(position);
        position.y += Main::context_content()->get_text(ElementsNames::show_keys_txt)->getGlobalBounds().size.y + SPACE*2;

        position.x = Main::window()->getSize().x/2 - Main::context_content()->get_text(ElementsNames::save_button)->getGlobalBounds().size.x - SPACE;
        Main::context_content()->get_text(ElementsNames::save_button)->setPosition(position);
        position.x = Main::window()->getSize().x/2 + SPACE;
        Main::context_content()->get_text(ElementsNames::reset_button)->setPosition(position);

        position = Main::context_content()->get_rect(ElementsNames::settings_rect)->getPosition();
        position.x -= Main::context_content()->get_text(ElementsNames::back_button)->getGlobalBounds().size.x + SPACE;
        Main::context_content()->get_text(ElementsNames::back_button)->setPosition(position);
    }

    void Update_content(){
        mouse = sf::Mouse::getPosition(*Main::window());
        
        if(Main::context_content()->get_button(ElementsNames::back_button)->contains(mouse))
            Main::context_content()->get_text(ElementsNames::back_button)->setFillColor(sf::Color::Black);
        else
            Main::context_content()->get_text(ElementsNames::back_button)->setFillColor(sf::Color::White);

        if(Main::context_content()->get_button(ElementsNames::save_button)->contains(mouse))
            Main::context_content()->get_text(ElementsNames::save_button)->setFillColor(sf::Color(0, 100, 255));
        else
            Main::context_content()->get_text(ElementsNames::save_button)->setFillColor(sf::Color(0, 255, 255));

        if(Main::context_content()->get_button(ElementsNames::reset_button)->contains(mouse))
            Main::context_content()->get_text(ElementsNames::reset_button)->setFillColor(sf::Color(0, 100, 255));
        else
            Main::context_content()->get_text(ElementsNames::reset_button)->setFillColor(sf::Color(0, 255, 255));
    }

    void Input(){
        while(std::optional event = Main::window()->pollEvent()){
            if(Main::check_resize_events(event)){
                Main::context(Main::Context::PlayMenu);
                window_resized = true;
                Main::first_drawing(true);
            }

            if(event->is<sf::Event::Closed>()){
                Main::window()->close();
            }

            if(event->is<sf::Event::MouseButtonPressed>()){
                if(Main::context_content()->get_button(ElementsNames::back_button)->contains(mouse)){
                    Main::context(Main::Context::PlayMenu);
                    Main::first_drawing(true);
                }

                else if(Main::context_content()->get_button(ElementsNames::save_button)->contains(mouse)){
                    save_settings_from(player_choosed);
                    Main::context(Main::Context::PlayMenu);
                    Main::first_drawing(true);
                }

                else if(Main::context_content()->get_button(ElementsNames::reset_button)->contains(mouse)){
                    terrain_current_choice = 2;
                    zone_current_choice = 0;
                    show_keys = true;
                }

                else{
                    Main::context_content()->get_selector(ElementsNames::terrain_generation_selector)->update_current_choice_from({float(mouse.x), float(mouse.y)});
                    Main::context_content()->get_selector(ElementsNames::zone_update_selector)->update_current_choice_from({float(mouse.x), float(mouse.y)});

                    terrain_current_choice = Main::context_content()->get_selector(ElementsNames::terrain_generation_selector)->get_current_choice();
                    zone_current_choice = Main::context_content()->get_selector(ElementsNames::zone_update_selector)->get_current_choice();
                }
            }
        }
    }

    void Event(){
        std::map<Main::Languages, sf::String> txt_title{
            {Main::english, L"Settings"},
            {Main::french, L"Paramètre"},
            {Main::malagasy, L"Toetra"},
        };

        std::map<Main::Languages, sf::String> txt_terrain_gen{
            {Main::english, L"Terrain generation size : "},
            {Main::french, L"Taille de la génération de terrain : "},
            {Main::malagasy, L"Hangezan'ny fiforonan-tany : "},
        };

        std::map<Main::Languages, sf::String> txt_zone_update{
            {Main::english, L"Zone update size : "},
            {Main::french, L"Taille de la zone à maj : "},
            {Main::malagasy, L"Hangezan'ny toerana fiasana : "},
        };

        std::map<Main::Languages, sf::String> txt_show_keys{
            {Main::english, L"Show commands : "},
            {Main::french, L"Afficher les commandes : "},
            {Main::malagasy, L"Aseho ny fanampiana : "},
        };

        std::map<Main::Languages, sf::String> txt_back{
            {Main::english, L" < "},
            {Main::french, L" < "},
            {Main::malagasy, L" < "},
        };

        std::map<Main::Languages, sf::String> txt_save{
            {Main::english, L" Save "},
            {Main::french, L" Sauver "},
            {Main::malagasy, L" Tadidiana "},
        };

        std::map<Main::Languages, sf::String> txt_reset{
            {Main::english, L" Reset "},
            {Main::french, L" Reini. "},
            {Main::malagasy, L" Averina "},
        };

        Main::context_content()->clear();
        Main::context_content()->set_priority_list_to(&all_elements);

        Main::context_content()->add_rect(ElementsNames::settings_rect);
        Main::context_content()->add_text(ElementsNames::title);
        Main::context_content()->add_text(ElementsNames::terrain_generation_txt);
        Main::context_content()->add_text(ElementsNames::zone_update_txt);
        Main::context_content()->add_text(ElementsNames::show_keys_txt); 
        Main::context_content()->add_text(ElementsNames::back_button, true); 
        Main::context_content()->add_text(ElementsNames::save_button, true); 
        Main::context_content()->add_text(ElementsNames::reset_button, true); 

        Main::context_content()->add_button(ElementsNames::back_button, Main::context_content()->get_text(ElementsNames::back_button));
        Main::context_content()->add_button(ElementsNames::save_button, Main::context_content()->get_text(ElementsNames::save_button));
        Main::context_content()->add_button(ElementsNames::reset_button, Main::context_content()->get_text(ElementsNames::reset_button));

        const int GENERATION = 10, UPDATE = 10;
        Main::context_content()->add_selector(ElementsNames::terrain_generation_selector, GENERATION, {500, 12});
        Main::context_content()->add_selector(ElementsNames::zone_update_selector, UPDATE, {500, 12});

        Main::context_content()->configure_text(ElementsNames::title, txt_title.at(Main::lang()), FontManager::get_font(FontManager::Fonts::Arial), 50, sf::Color::Yellow);
        Main::context_content()->configure_text(ElementsNames::terrain_generation_txt, txt_terrain_gen.at(Main::lang()) + std::to_string(terrain_gen_values[terrain_current_choice]), FontManager::get_font(FontManager::Fonts::Arial), 30, sf::Color::White);
        Main::context_content()->configure_text(ElementsNames::zone_update_txt, txt_zone_update.at(Main::lang()) + std::to_string(zone_update_values[zone_current_choice]), FontManager::get_font(FontManager::Fonts::Arial), 30, sf::Color::White);
        Main::context_content()->configure_text(ElementsNames::show_keys_txt, txt_show_keys.at(Main::lang()), FontManager::get_font(FontManager::Fonts::Arial), 30, sf::Color::White);
        Main::context_content()->configure_text(ElementsNames::back_button, txt_back.at(Main::lang()), FontManager::get_font(FontManager::Fonts::Arial), 40, sf::Color::White);
        Main::context_content()->configure_text(ElementsNames::save_button, txt_save.at(Main::lang()), FontManager::get_font(FontManager::Fonts::Arial), 40, sf::Color(0, 255, 255));
        Main::context_content()->configure_text(ElementsNames::reset_button, txt_reset.at(Main::lang()), FontManager::get_font(FontManager::Fonts::Arial), 40, sf::Color(0, 255, 255));
        Main::context_content()->configure_text_rect(ElementsNames::save_button, sf::Color::Blue, sf::Color::Blue, 5);
        Main::context_content()->configure_text_rect(ElementsNames::reset_button, sf::Color::Blue, sf::Color::Blue, 5);
        Main::context_content()->configure_text_rect(ElementsNames::back_button, sf::Color(100, 100, 100), sf::Color(100, 100, 100), 8);

        Set_content_position();

        Main::context_content()->updade_text_rect(ElementsNames::back_button);
        Main::context_content()->updade_text_rect(ElementsNames::reset_button);
        Main::context_content()->updade_text_rect(ElementsNames::save_button);

        Main::context_content()->get_selector(ElementsNames::terrain_generation_selector)->set_current_choice_to(terrain_current_choice);
        Main::context_content()->get_selector(ElementsNames::zone_update_selector)->set_current_choice_to(zone_current_choice);

        if(Main::first_drawing()){
            Settings settings = get_settings_from(player_choosed);
            for(int i = 0; i < GENERATION; i++){
                if(terrain_gen_values[i] == settings.terrain_generation_size){
                    terrain_current_choice = i;
                    break;
                }
            }
            for(int i = 0; i < UPDATE; i++){
                if(zone_update_values[i] == settings.zone_update_size){
                    zone_current_choice= i;
                    break;
                }
            }
            Main::first_drawing(false);
            window_resized = false;
        }

        Input();
        Update_content();
        Render();
    }
};