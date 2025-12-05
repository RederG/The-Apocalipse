#include "../Help/the_apocalipse.hpp"
#include "game_window.hpp"
#include "world_settings.hpp"

namespace GameWindow{
    std::vector<std::pair<std::string, ContainManager::Types>> all_elements{
        {"position_text", ContainManager::Text},
        {"fps_text", ContainManager::Text},
        {"update_map_time_text", ContainManager::Text},
        {"game_timer_txt", ContainManager::Text},
        {"zombie_killed_txt", ContainManager::Text},
    };
    
    const std::map<Main::Languages, std::string> score_txt{
        {Main::Languages::english, "Score : "},
        {Main::Languages::french, "Score : "},
        {Main::Languages::malagasy, "Maty : "}
    };

    const std::map<Main::Languages, std::string> position_txt{
        {Main::Languages::english, "Position : "},
        {Main::Languages::french, "Position : "},
        {Main::Languages::malagasy, "Toearana : "}
    };

    int default_seed = 0;

    sf::Clock game_timer;

    bool first_zombies = false;
    bool show_keys = true;
    
    sf::Clock fps_clock;
    float fps;
    sf::Clock FPS_update;

    sf::Clock time_update;
    bool time_updating = false;

    sf::Font font;

    sf::Vector2f player_position;

    int time = 0;
    int zombie_killed = 0;
    
    const int DISTANCE_MIN = 4;

    void Text(sf::Text &txt_name, std::string text, sf::Font &font, int size, sf::Color color){
        txt_name.setString(text);
        txt_name.setFont(font);
        txt_name.setCharacterSize(size);
        txt_name.setFillColor(color);
    }

    int power(int number, int power){
        int new_number = number;
        int power_now = 1;
        while (power_now < power){
            new_number = new_number * number;
            power_now += 1;
        }
        return new_number;
    }

    void trunc_to(int decimal_to_reach, float number, std::string &result){
        std::string num_str = std::to_string(number);
        std::string truncked_number;
        int decimal_now = 0;
        bool truncature = false;

        for (char integer : num_str){
            if (truncature == false){
                truncked_number += integer;
            }
            if (truncature == true && decimal_now < decimal_to_reach){
                truncked_number += integer;
                decimal_now += 1;
            }
            if (integer == '.'){
                truncature = true;
            }
            if (decimal_now >= decimal_to_reach){
                result = truncked_number;
                break;
            }
        }
    }
    
    void draw(bool pause_animation){
        if(Main::context() == Main::Context::TheGame || Main::context() == Main::Context::GamePauseMenu || Main::context() == Main::Context::GameOver){
            Map::get(Map::NAME)->draw();
            
            Life_Bar::set_pause_drawing_to(pause_animation);
            Viewer::Items::draw();
            for(auto player : Player::container){
                
                if(player->get_state() == Player::State::Look_at_Inventory)
                    Viewer::draw_container(player->get_inventory(), &*player, Map::get(Map::NAME), true);
                Tomb::Object* tomb = player->get_nearest_tomb(DISTANCE_MIN);
                if(player->get_state() == Player::State::Interacting_with_objects && tomb != nullptr)
                    Viewer::draw_container(tomb->get_inventory(), &*tomb, Map::get(Map::NAME), true);

                Viewer::draw_effect(player->get_effect_manager(), 4);
                Viewer::draw_satiation(&*player);
                if(show_keys)
                    KeysManager::draw_all(&*player, DISTANCE_MIN);
            }
            if(Main::context_content()->get_text("game_timer_txt") != nullptr && Main::context_content()->get_text("zombie_killed_txt") != nullptr && Main::context_content()->get_text("position_text") != nullptr){
                Main::context_content()->get_text("game_timer_txt")->setPosition(sf::Vector2f(0, 0));
                Main::context_content()->set_position(Main::context_content()->get_text("game_timer_txt"), true);
                int score_txt_y = Main::context_content()->get_text("game_timer_txt")->getGlobalBounds().position.y + Main::context_content()->get_text("game_timer_txt")->getGlobalBounds().size.y;
                Main::context_content()->get_text("zombie_killed_txt")->setPosition(sf::Vector2f(0, score_txt_y));
                Main::context_content()->set_position(Main::context_content()->get_text("zombie_killed_txt"), true);
                Main::context_content()->get_text("position_text")->setString(position_txt.at(Main::lang()) + to_string(int(player_position.x)) + ", " + to_string(int(player_position.y)));
            }
            Main::context_content()->set_priority_list_to(&all_elements);
            Main::context_content()->draw_all();
        }
    }
    
    void Set_content_position(){
        sf::Vector2f position_text_pos = Main::context_content()->get_text("fps_text")->getPosition();
        position_text_pos.y += Main::context_content()->get_text("fps_text")->getGlobalBounds().size.y + 10;
        Main::context_content()->get_text("position_text")->setPosition(position_text_pos);

        Main::context_content()->set_position(Main::context_content()->get_text("game_timer_txt"), true);

        int score_txt_y = Main::context_content()->get_text("game_timer_txt")->getGlobalBounds().position.y + Main::context_content()->get_text("game_timer_txt")->getGlobalBounds().size.y;
        Main::context_content()->get_text("zombie_killed_txt")->setPosition(sf::Vector2f(0, score_txt_y));
        Main::context_content()->set_position(Main::context_content()->get_text("zombie_killed_txt"), true);

        Main::context_content()->set_position(Main::context_content()->get_text("game_over"), true, true);
    }

    void Input(){
        while(std::optional event = Main::window()->pollEvent()){
            Main::check_resize_events(event);
            if(event->is<sf::Event::Closed>()){
                Main::window()->close();
                TheApocalipse::terminates();
                ThreadManager::terminates_all();
            }

            if(auto* key = event->getIf<sf::Event::KeyPressed>()){
                if(KeysManager::try_to(KeysManager::Action::pause, key->control, key->alt, key->shift)){
                    ThreadManager::pause_all();
                    Main::context(Main::Context::GamePauseMenu);
                    Main::first_drawing(true);
                }

                if(KeysManager::try_to(KeysManager::Action::open_inventory, key->control, key->alt, key->shift)){
                    for (auto player : Player::container){
                        if(player->get_state() == Player::State::Playing){
                            Player::set_state(Player::State::Look_at_Inventory);
                            break;
                        }
                        if(player->get_state() == Player::State::Look_at_Inventory){
                            Player::set_state(Player::State::Playing);
                            break;
                        }
                    }
                }

                if(KeysManager::try_to(KeysManager::Action::remove_item, key->control, key->alt, key->shift)){
                    for(auto player : Player::container){
                        if(player->get_state() == Player::State::Look_at_Inventory){
                            player->get_inventory()->remove_current_item();
                        }
                        break;
                    }
                }
                
                if(KeysManager::try_to(KeysManager::Action::interaction, key->control, key->alt, key->shift)){
                    for(auto player : Player::container){
                        if(player->Player::Object::get_state() == Player::State::Playing){
                            if(player->get_nearest_interactive_object() != nullptr){
                                
                            }
                            break;
                        }
                        if(player->Player::Object::get_state() == Player::State::Interacting_with_objects){
                            Player::set_state(Player::State::Playing);
                            break;
                        }
                    }
                }
                
                if(KeysManager::try_to(KeysManager::Action::use_item, key->control, key->alt, key->shift)){
                    for(auto player : Player::container){
                        if(player->get_inventory()->get_current_item() != nullptr && player->get_state() == Player::State::Playing){
                            Entity::set_temporary_entity(&*player);
                            player->get_inventory()->get_current_item()->use_item(*player);
                            Entity::reset_temporary_entity();
                        }
                        break;
                    }
                }

                if(KeysManager::try_to(KeysManager::Action::take_item, key->control, key->alt, key->shift)){
                    for(auto player : Player::container){
                        if(player->get_state() == Player::State::Interacting_with_objects){
                            if(player->get_nearest_tomb(DISTANCE_MIN)->get_inventory()->get_current_item() != nullptr
                                && player->get_inventory()->is_full() == false){

                                player->take_item_of(*player->get_nearest_tomb(DISTANCE_MIN));
                            }
                            else if(player->get_inventory()->is_full()){
                                Debugging::write("Your inventory is full", Debugging::get_state_of(Debugging::In_game));
                            }
                        }
                    }
                }

                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::NumpadPlus)){
                    Map::get(Map::NAME)->increase_scale();
                }

                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::NumpadMinus)){
                    Map::get(Map::NAME)->reduce_scale();
                }

                for (auto player : Player::container){
                    if (player->get_state() == Player::State::Look_at_Inventory){
                        //player->get_inventory()->switch_case(event.text.unicode);
                        if(KeysManager::try_to(KeysManager::Action::move_right))
                            player->get_inventory()->move_selector_to(Container::Direction::Right);
                        if(KeysManager::try_to(KeysManager::Action::move_left))
                            player->get_inventory()->move_selector_to(Container::Direction::Left);
                    }

                    if (player->get_state() == Player::State::Interacting_with_objects){
                        if(player->get_nearest_tomb(DISTANCE_MIN) != nullptr){
                            if(KeysManager::try_to(KeysManager::Action::move_right))
                                player->get_nearest_tomb(DISTANCE_MIN)->get_inventory()->move_selector_to(Container::Direction::Right);
                            if(KeysManager::try_to(KeysManager::Action::move_left))
                                player->get_nearest_tomb(DISTANCE_MIN)->get_inventory()->move_selector_to(Container::Direction::Left);
                        }
                        else{
                            player->set_state(Player::State::Playing);
                        }
                    }
                }
            }
        }
    }

    void UpdateOnThread(){
        Debugging::write("(Game updater) - THREAD INITIATED");
        bool paused = false;
        while(ThreadManager::get_state_of(ThreadManager::Thread::game_thread) != ThreadManager::State::Terminated){
            if(ThreadManager::get_state_of(ThreadManager::Thread::game_thread) == ThreadManager::State::Actived && TheApocalipse::is_running()){
                if(paused)
                    Debugging::write("(Game updater) - THREAD CONTINUED");
                paused = false;
                if(Main::context() == Main::Context::TheGame && TheApocalipse::is_running() && Map::get(Map::NAME) != nullptr){
                    Map::get(Map::NAME)->set_elements_drawing_order();
                    
                    if(Player::container.empty() == true){
                        Main::context(Main::Context::GameOver);
                        Viewer::Items::change_to(nullptr);
                        ThreadManager::pause_all();
                        Main::first_drawing(true);
                    }
                    
                    if(TheApocalipse::is_running())
                        Bullet::verify_collision();
                    if(TheApocalipse::is_running())
                        Entity::set_all_nearest_interactive_objects();
                }
            }
            else if(ThreadManager::get_state_of(ThreadManager::Thread::game_thread) == ThreadManager::State::Paused && !paused){
                Debugging::write("(Game updater) - THREAD PAUSED");
                paused = true;
            }
        }
        Debugging::write("(Game updater) - THREAD TERMINATED");
    }

    void Update(){
        for(auto player : Player::container){
            if(player->is_alive() == false){
                Player::container.remove(player);
                for(auto entity : Entity::container){
                    if(entity->get_name() == player->get_name() && entity->get_id() == player->get_id()){
                        Entity::container.remove(entity);
                        break;
                    }
                }
                Tomb::set_tomb_of(*player);
                break;
            }
            player_position = player->get_map_position();

            if(player->get_state() == Player::State::Playing){
                if(player->get_inventory()->get_current_item() == nullptr)
                    Viewer::Items::change_to(nullptr);
                if(player->get_inventory()->get_current_item() != nullptr)
                    Viewer::Items::change_to(&*player->get_inventory()->get_current_item());
            }
            
            if (player->get_state() == Player::State::Look_at_Inventory){
                Viewer::draw_container(player->get_inventory(), &*player, Entity::get_map(), true);
                player->switch_animation_to(Entity::AnimationState::Default);
                if(player->get_inventory()->get_current_item() != nullptr){
                    Viewer::Items::change_to(&*player->get_inventory()->get_current_item());
                }
                else Viewer::Items::change_to(nullptr);
            }
            
            else if(player->Player::Object::get_state() == Player::State::Interacting_with_objects){
                if(player->get_nearest_tomb(DISTANCE_MIN) != nullptr){
                    Viewer::draw_container(player->get_nearest_tomb(DISTANCE_MIN)->get_inventory(), player->get_nearest_tomb(DISTANCE_MIN), Entity::get_map(), true);
                    if(player->get_nearest_tomb(DISTANCE_MIN)->get_inventory()->get_current_item()!= nullptr)
                        Viewer::Items::change_to(&*player->get_nearest_tomb(DISTANCE_MIN)->get_inventory()->get_current_item());
                    if(player->get_nearest_tomb(DISTANCE_MIN)->get_inventory()->get_current_item() == nullptr)
                        Viewer::Items::change_to(nullptr);
                }
                else{
                    player->set_state(Player::State::Playing);
                }
            }
            
            if(player->get_inventory()->get_current_item() != nullptr && player->get_inventory()->get_current_item()->is_usable() == false){
                player->get_inventory()->remove_current_item();
            }

        }
        
        Entity::update_all();
                
        if(Map::get(Map::NAME) != nullptr)
            Map::get(Map::NAME)->update();

        if(Entity::verify_entity() == true)
            zombie_killed += 1;
        Gun::verify_guns_usability();
        Gun::verify_all_fire_mode_clocks();
        Bullet::verify_bullets_lives();

        Bullet::move_all(fps);

        const int MAX_ZOMBIES = 5;

        if(Tomb::verify_all_life_time() == true)
            Zombie::create_multiple(1, Map::get(Map::NAME));
        if(Zombie::container.size() < MAX_ZOMBIES && Map::get(Map::NAME) != nullptr && Map::get(Map::NAME)->is_updated() && first_zombies == true)
            Zombie::create_multiple(1, Map::get(Map::NAME));
        if(Zombie::container.size() >= MAX_ZOMBIES && first_zombies == true)
            first_zombies = false;
    }

    void Render(){
        Main::window()->clear();
        draw(false);
        Main::window()->display();
    }

    void Event(){
        if(Main::clock_seconds(1, time_update) == Main::GameTimer::Reached)
            time_updating = true;
        else
            time_updating = false;

        // The font for the texts
        sf::Font font = *FontManager::get_font(FontManager::Fonts::Arial);

        // The game fps
        Main::window()->setFramerateLimit(Main::MAX_FPS);

        // The FPS
        std::string fps_str;
        trunc_to(2, fps, fps_str);

        if(Main::clock_seconds(1, game_timer) == Main::GameTimer::Reached)
            time += 1;
        
        std::string time_string = "";
        Main::sec_conversion(time, time_string);

        Main::context_content()->clear();
        Main::context_content()->set_window_to(*Main::window());
        
        Main::context_content()->add_text("fps_text");
        Main::context_content()->add_text("game_timer_txt");
        Main::context_content()->add_text("position_text");
        Main::context_content()->add_text("zombie_killed_txt");
        Main::context_content()->add_text("game_over");

        Main::context_content()->configure_text("fps_text", "FPS : "+fps_str, FontManager::get_font(FontManager::Arial), 25, sf::Color::White);
        Main::context_content()->configure_text("position_text", "", FontManager::get_font(FontManager::Arial), 25, sf::Color::White);
        Main::context_content()->configure_text("game_timer_txt", time_string, FontManager::get_font(FontManager::Arial), 30, sf::Color::White);
        Main::context_content()->configure_text("zombie_killed_txt", score_txt.at(Main::lang()) + to_string(zombie_killed), FontManager::get_font(FontManager::Arial), 30, sf::Color::White);
        Main::context_content()->configure_text("game_over", "GAME OVER", FontManager::get_font(FontManager::Arial), 50, sf::Color::Red);

        Set_content_position();
        
        // ========== THE FIRST DRAWING ========== //
        if (Main::first_drawing()){
            Debugging::write("----THE GAME----\n", Debugging::get_state_of(Debugging::Description));

            PlayerDataWindow::Data player_data = PlayerDataWindow::get_player_data(player_choosed);
            WorldSettingsWindow::Settings world_settings = WorldSettingsWindow::get_settings_from(player_choosed);
            Debugging::write("Game's seed : " + std::to_string(player_data.world_seed));

            GameWindow::time = 0;
            GameWindow::zombie_killed = 0;
            show_keys = world_settings.show_keys;
            
            TheApocalipse::init();

            default_seed = 23254841;

            PerlinNoise::new_noise(Map::NAME, 1, 1, 1, player_data.world_seed, 20, sf::Vector2i(400, 400));
            PerlinNoise::get(Map::NAME)->configure_color(PerlinNoise::Interval(0, 85), MapElement::Help::get_color_of(MapElement::WorldContent::soil_rocks_1));
            PerlinNoise::get(Map::NAME)->configure_color(PerlinNoise::Interval(85, 90), MapElement::Help::get_color_of(MapElement::WorldContent::spawn_zombie_1));
            PerlinNoise::get(Map::NAME)->configure_color(PerlinNoise::Interval(90, 150), MapElement::Help::get_color_of(MapElement::WorldContent::soil_rocks_1));
            PerlinNoise::get(Map::NAME)->configure_color(PerlinNoise::Interval(150, 255), MapElement::Help::get_color_of(MapElement::WorldContent::soil_herb_1));

            PerlinNoise::new_noise(Map::NAME_ALEATORY_NOISE, 2, 4, 2, player_data.world_seed);
            
            if(player_data.first_play){
                Map::create(Map::NAME, *Main::window(), PerlinNoise::get(Map::NAME), 
                            sf::Vector2i(18, 18), sf::Vector2i(10, 10), sf::Vector2f(2, 2), 
                            sf::Vector2i(world_settings.terrain_generation_size, world_settings.terrain_generation_size), 
                            sf::Vector2i(world_settings.zone_update_size, world_settings.zone_update_size));
                    
                Player::create_player(player_choosed, Map::get(Map::NAME));
                Player::set_state(Player::State::Playing);
            }

            else if(!player_data.dead){
                GameWindow::time = player_data.time;
                GameWindow::zombie_killed = player_data.score;

                Map::load_from("../Players/" + player_choosed + "/", Map::NAME, *Main::window());

                std::string tombs_path, entities_path, inventories_path, items_path, effect_managers_path, effects_path;
                tombs_path = "../Players/" + player_choosed + "/InteractiveObjects/Tombs";
                entities_path = "../Players/" + player_choosed + "/Entities";
                inventories_path = "../Players/" + player_choosed + "/Containers";
                items_path = "../Players/" + player_choosed + "/Items";
                effects_path = "../Players/" + player_choosed + "/Effects";
                effect_managers_path = "../Players/" + player_choosed + "/Effect_managers";
                Entity::load_all_from(entities_path, inventories_path, items_path, effect_managers_path, effects_path);
                Tomb::load_all_from(tombs_path, inventories_path, items_path);
            }

            else if(player_data.dead){
                Player::container.clear();
                Entity::container.clear();
                Tomb::container.clear();
                std::filesystem::remove_all("../Players/" + player_choosed);

                std::filesystem::create_directory("../Players/" + player_choosed);
                std::filesystem::create_directory("../Players/" + player_choosed + "/Entities");
                std::filesystem::create_directory("../Players/" + player_choosed + "/Tombs");
                std::filesystem::create_directory("../Players/" + player_choosed + "/Containers");
                std::filesystem::create_directory("../Players/" + player_choosed + "/Items");
                std::filesystem::create_directory("../Players/" + player_choosed + "/Effects_managers");
                std::filesystem::create_directory("../Players/" + player_choosed + "/Effects");
                std::filesystem::create_directory("../Players/" + player_choosed + "/World");
                
                PerlinNoise::new_noise(Map::NAME, 1, 1, 1, player_data.world_seed, 20, sf::Vector2i(400, 400));
                PerlinNoise::get(Map::NAME)->configure_color(PerlinNoise::Interval(0, 85), MapElement::Help::get_color_of(MapElement::WorldContent::soil_rocks_1));
                PerlinNoise::get(Map::NAME)->configure_color(PerlinNoise::Interval(85, 90), MapElement::Help::get_color_of(MapElement::WorldContent::spawn_zombie_1));
                PerlinNoise::get(Map::NAME)->configure_color(PerlinNoise::Interval(90, 150), MapElement::Help::get_color_of(MapElement::WorldContent::soil_rocks_1));
                PerlinNoise::get(Map::NAME)->configure_color(PerlinNoise::Interval(150, 255), MapElement::Help::get_color_of(MapElement::WorldContent::soil_herb_1));

                Map::create(Map::NAME, *Main::window(), PerlinNoise::get(Map::NAME), 
                            sf::Vector2i(18, 18), sf::Vector2i(10, 10), sf::Vector2f(2, 2), 
                            sf::Vector2i(world_settings.terrain_generation_size, world_settings.terrain_generation_size), 
                            sf::Vector2i(world_settings.zone_update_size, world_settings.zone_update_size));

                Player::create_player(player_choosed, Map::get(Map::NAME));
                Player::set_state(Player::State::Playing);
            }
            
            StructureManager::set_up(player_data.world_seed);
            Entity::set_map_to(Map::get(Map::NAME));
            ThreadManager::set_state_of(ThreadManager::Thread::game_thread, ThreadManager::State::Actived);
            
            Main::first_drawing(false);
        }
        sf::Clock clock;
        Input();
        if(time_updating)
            Debugging::write("Input time : " + std::to_string(clock.getElapsedTime().asSeconds()));
        
        if(!Main::window()->isOpen())
            return;
        
        clock.restart();
        Update();
        if(time_updating)
            Debugging::write("Update time : " + std::to_string(clock.getElapsedTime().asSeconds()));  
        clock.restart();
        Render();
        if(time_updating)
            Debugging::write("Render time : " + std::to_string(clock.getElapsedTime().asSeconds()) + "\n");
    }
}
