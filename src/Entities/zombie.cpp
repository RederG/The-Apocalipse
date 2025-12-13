#include <cmath>
#include "../../headers/Help/the_apocalipse.hpp"

#define PI 3.14

namespace Zombie{
    std::vector<Zombie::Object*> container;

    bool searching = false;
    bool algo_paused = false;

    Object::Object(){
        id = "entity_" + std::to_string(Main::random_value()%1000) + "_zombie_" + std::to_string(Main::random_value()%1000);
        name = "zombie";
        attack_value = 2;
        life = 100;
        type = Entity::Zombie;
        direction = Entity::Direction::right;
        animation_state = Entity::AnimationState::Default;
        element_type = MapElement::Type::Entity_being;
        velocity = Entity::Velocity.at(Entity::Zombie);
        life_bar = new Life_Bar::Object(100);
        life_bar->set_new_life_to(life);
    }

    void Object::fill_inventory(){
        std::string have_knife = Probability::get_universe("have_knife")->get_result();
        std::string have_gun = Probability::get_universe("have_gun")->get_result();
        std::string have_specific_gun = Probability::get_universe("have_specific_gun")->get_result();
        std::string have_cartridge = Probability::get_universe("have_cartridge")->get_result();
        std::string have_specific_cartridge = Probability::get_universe("have_specific_cartridge")->get_result();
        std::string have_syringe = Probability::get_universe("have_syringe")->get_result();
        std::string have_specific_syringe = Probability::get_universe("have_specific_syringe")->get_result();
        std::string have_food = Probability::get_universe("have_food")->get_result();
        std::string have_specific_food = Probability::get_universe("have_specific_food")->get_result();

        if(have_knife == "yes"){
            Knife* new_knife = new Knife();
            new_knife->add_to_container(this->get_inventory(), true);
            Debugging::write(this->get_id() + " gets a knife", Debugging::get_state_of(Debugging::In_game));
        }
        if(have_cartridge == "yes"){
            if(have_specific_cartridge == "pistolet"){
                Cartridge::Object* new_cartridge = Cartridge::create_new(Cartridge::pistolet);
                new_cartridge->add_to_container(this->get_inventory(), true);
                Debugging::write(this->get_id() + " gets a pistolet munition", Debugging::get_state_of(Debugging::In_game));
            }
            if(have_specific_cartridge == "AK47"){
                Cartridge::Object* new_cartridge = Cartridge::create_new(Cartridge::AK_47);
                new_cartridge->add_to_container(this->get_inventory(), true);
                Debugging::write(this->get_id() + " gets an AK47 munition", Debugging::get_state_of(Debugging::In_game));
            }
        }
        if(have_gun == "yes"){
            if(have_specific_gun == "pistolet"){
                Gun::Object* new_gun = new Gun::Object(Gun::pistolet);
                new_gun->add_to_container(this->get_inventory(), true);
                Debugging::write(this->get_id() + " gets a pistolet", Debugging::get_state_of(Debugging::In_game));
            }
            if(have_specific_gun == "AK47"){
                Gun::Object* new_gun = new Gun::Object(Gun::AK_47);
                new_gun->add_to_container(this->get_inventory(), true);
                Debugging::write(this->get_id() + " gets an AK47", Debugging::get_state_of(Debugging::In_game));
            }
        }
        if(have_syringe == "yes"){
            if(have_specific_syringe == "zombie_syringe"){
                Syringe::Object* zombie_syringe = Syringe::create_new(Syringe::Type::zombie, Syringe::Type::clean);
                zombie_syringe->add_to_container(this->get_inventory(), true);
                Debugging::write(this->get_id() + " gets a syringe", Debugging::get_state_of(Debugging::In_game));
            }
        }
        if(have_food == "yes"){
            if(have_specific_food == "damaged_food_bag"){
                Artificial_food::Object* food = Artificial_food::create_new(Artificial_food::Foods_names::damaged_food_bag);
                food->add_to_container(this->get_inventory(), true);
                Debugging::write(this->get_id() + " gets a damaged food bag", Debugging::get_state_of(Debugging::In_game));
            }
        }
    }

    void Object::locates_player(){
        this->locating_player = true;
        if(Entity::get_map() != nullptr){
            sf::Vector2f zombie_position = this->get_map_position();
            sf::Vector2f player_position = Entity::get_map()->get_player_map_pos();

            this->A_star_path_finder->set_up(zombie_position, player_position, Entity::get_map());
            this->A_star_path_finder->find_path();
        }
        this->locating_player = false;
    }

    void Object::move(){
        if(!this->locating_player){
            if(this->A_star_path_finder->found_path()){
                this->path_to_something = this->A_star_path_finder->get_path();
                this->A_star_path_finder->clear_path();
                this->moving = true;
            }
            if(this->path_to_something.empty() == true)
                this->moving = false;
            if(this->path_to_something.empty() == false){
                sf::Vector2f point_to_reach = this->path_to_something.front();
                sf::Vector2f end_point = this->path_to_something.back();
                point_to_reach.x += 0.5;
                point_to_reach.y += 0.5;
                bool can_go_on_x = true;
                bool can_go_on_y = true;
                if(round(point_to_reach.x) > round(this->map_position.x)){
                    if(this->direction == Entity::Direction::left)
                        can_go_on_x = false;
                    this->direction = Entity::Direction::right;
                    if(!this->is_colliding_with_other_entities() && can_go_on_x)
                        this->move_on_map(this->velocity, 0);
                    else 
                        this->path_to_something.erase(this->path_to_something.begin());
                }
                else if(round(point_to_reach.x) < round(this->map_position.x)){
                    if(this->direction == Entity::Direction::right)
                        can_go_on_x = false;
                    this->direction = Entity::Direction::left;
                    if(!this->is_colliding_with_other_entities() && can_go_on_x)
                        this->move_on_map(-this->velocity, 0);
                    else
                        this->path_to_something.erase(this->path_to_something.begin());
                }
                else if(round(point_to_reach.y) > round(this->map_position.y)){
                    if(this->direction == Entity::Direction::up)
                        can_go_on_y = false;
                    this->direction = Entity::Direction::down;
                    if(!this->is_colliding_with_other_entities() && can_go_on_y)
                        this->move_on_map(0, this->velocity);
                    else 
                        this->path_to_something.erase(this->path_to_something.begin());
                }
                else if(round(point_to_reach.y) < round(this->map_position.y)){
                    if(this->direction == Entity::Direction::down)
                        can_go_on_y = false;
                    this->direction = Entity::Direction::up;
                    if(!this->is_colliding_with_other_entities() && can_go_on_y)
                        this->move_on_map(0, -this->velocity);
                    else 
                        this->path_to_something.erase(this->path_to_something.begin());
                }
                if(round(point_to_reach.x) == round(this->map_position.x) && round(point_to_reach.y) == round(this->map_position.y))
                    this->path_to_something.erase(this->path_to_something.begin());
            }
        }
    }

    sf::Sprite Object::get_sprite(Map::Object* map, sf::Color color){
        Map::Area* entity_area = map->get_area_of(this);

        sf::Vector2f entity_window_position = this->get_window_position_on(map);

        sf::Sprite entity_sprite(*AllTextures::get("zombie"));
        this->animation_rect.position.y = int(this->direction);
        if (this->moving == true){
            this->animate();
        }
        if(this->moving == false || animation_state == Entity::AnimationState::Default)
            entity_sprite.setTextureRect(this->animation_rect);

        sf::Vector2f zombie_scale = map->get_scale();
        zombie_scale.x *= Entity::ENTITY_SCALE.x;
        zombie_scale.y *= Entity::ENTITY_SCALE.y;
        
        sf::Vector2f origin;
        origin.x = this->animation_rect.size.x/2;
        origin.y = this->animation_rect.size.y - 4;
        
        entity_sprite.setScale(zombie_scale);
        entity_sprite.setPosition(entity_window_position);
        entity_sprite.setOrigin(origin);
        entity_sprite.setColor(color);
        
        return entity_sprite;
    }

    void Object::random_pos(Map::Object* map){
        Map::Area* player_area = map->get_player_area();
        Map::Area* spawn_area = nullptr;
        std::vector<sf::Vector2i> all_spawn_point;
        for(auto area : map->get_all_updating_areas()){
            bool spawn_point_exists = false;
            for(int x = 0; x < area->get_all_elements().size() && TheApocalipse::is_running() && area != player_area; x++){
                for(int y = 0; y < area->get_all_elements()[x].size() && TheApocalipse::is_running() && area != player_area; y++){
                    MapElement::WorldContent element = area->get_all_elements()[x][y];
                    if(element == MapElement::WorldContent::spawn_zombie_1 || 
                            element == MapElement::WorldContent::spawn_zombie_2 || 
                            element == MapElement::WorldContent::spawn_zombie_3){
                        all_spawn_point.push_back({
                            area->get_size().x*area->get_position().x + x, 
                            area->get_size().y*area->get_position().y + y
                        });
                    }
                }
            }
        }
        if(all_spawn_point.empty() == false){
            sf::Vector2i spawn_pos = all_spawn_point[Main::random_value() % all_spawn_point.size()];
            this->map_position = {float(spawn_pos.x), float(spawn_pos.y)};
            this->map_position.x = int(this->map_position.x) + 0.5f;
            this->map_position.y = int(this->map_position.y) + 0.5f;
        }
    }

    void Object::attack(){
        if(Main::clock_seconds(0.5, this->attack_timer) == Main::Reached){
            for(auto entity : Entity::container){
                if(typeid(*entity) == typeid(Player::Object)){
                    sf::Vector2f player_position = entity->get_map_position();
                    sf::Vector2f distance_to_player = this->map_position;
                    distance_to_player.x -= player_position.x;
                    distance_to_player.y -= player_position.y;

                    float distance = sqrt(distance_to_player.x*distance_to_player.x + distance_to_player.y*distance_to_player.y);
                    if(distance <= 1.5){
                        entity->set_state_to(Entity::State::Hurting);
                        entity->get_attacked(this->attack_value);
                        break;
                    }
                }
            }
        }
    }

    void Object::destroy(){
        delete this->inventory;
        delete this->life_bar;
        delete this->A_star_path_finder;
        delete this->effect_manager;
        this->inventory = nullptr;
        this->life_bar= nullptr;
        this->A_star_path_finder = nullptr;
        this->effect_manager = nullptr;
    }

    void Object::update(){
        this->move();
        this->attack();
        this->inventory->update();
        this->effect_manager->update_on(this);
        this->effect_manager->update();
    }

    void Object::save_to(std::string location_path){
        std::string path = location_path + "/" + this->id + ".entity";
        std::fstream file(path, std::ios::out | std::ios::binary);
        if(file.is_open()){
            std::size_t size = this->name.size();
            file.write(reinterpret_cast<char*>(&size), sizeof(std::size_t));
            file.write(this->name.c_str(), sizeof(char)*this->name.size());

            size = this->id.size();
            file.write(reinterpret_cast<char*>(&size), sizeof(std::size_t));
            file.write(this->id.c_str(), sizeof(char)*this->id.size());

            file.write(reinterpret_cast<char*>(&this->life), sizeof(float));
            file.write(reinterpret_cast<char*>(&this->max_life), sizeof(float));

            file.write(reinterpret_cast<char*>(&this->Entity::Object::state), sizeof(this->Entity::Object::state));
            file.write(reinterpret_cast<char*>(&this->type), sizeof(this->type));

            file.write(reinterpret_cast<char*>(&this->satiation), sizeof(float));
            file.write(reinterpret_cast<char*>(&this->max_satiation), sizeof(float));

            file.write(reinterpret_cast<char*>(&this->satiation_diminution_lapse_time), sizeof(float));
            file.write(reinterpret_cast<char*>(&this->satiation_diminution_lapse_time_original), sizeof(float));

            file.write(reinterpret_cast<char*>(&this->map_position.x), sizeof(float));
            file.write(reinterpret_cast<char*>(&this->map_position.y), sizeof(float));

            file.write(reinterpret_cast<char*>(&this->attack_value), sizeof(int));

            file.close();
        }
    }

    void Object::load_from(std::string location_path){
        if(!std::filesystem::exists(location_path))
            Debugging::write(location_path + " does not exists (zombie loading)");
        else{
            std::fstream file(location_path, std::ios::in | std::ios::binary);
            if(file.is_open()){
                std::size_t size = 0;
                file.read(reinterpret_cast<char*>(&size), sizeof(std::size_t));

                std::string name;
                name.resize(size);
                file.read(&name[0], sizeof(char)*size);
                this->name = name;

                size = 0;
                file.read(reinterpret_cast<char*>(&size), sizeof(std::size_t));

                std::string id;
                id.resize(size);
                file.read(&id[0], sizeof(char)*size);
                this->id = id;

                file.read(reinterpret_cast<char*>(&this->life), sizeof(float));
                file.read(reinterpret_cast<char*>(&this->max_life), sizeof(float));

                file.read(reinterpret_cast<char*>(&this->state), sizeof(this->state));
                file.read(reinterpret_cast<char*>(&this->type), sizeof(this->type));

                file.read(reinterpret_cast<char*>(&this->satiation), sizeof(float));
                file.read(reinterpret_cast<char*>(&this->max_satiation), sizeof(float));

                file.read(reinterpret_cast<char*>(&this->satiation_diminution_lapse_time), sizeof(float));
                file.read(reinterpret_cast<char*>(&this->satiation_diminution_lapse_time_original), sizeof(float));

                file.read(reinterpret_cast<char*>(&this->map_position.x), sizeof(float));
                file.read(reinterpret_cast<char*>(&this->map_position.y), sizeof(float));

                file.read(reinterpret_cast<char*>(&this->attack_value), sizeof(int));

                file.close();
            }
            this->life_bar->set_new_life_to(this->life);
        }
    }

    void create_multiple(unsigned int number_of_zombies, Map::Object* map){
        for (int i = 0; i < number_of_zombies && TheApocalipse::is_running(); i++){
            Object* zombie = new Object();
            container.push_back(zombie);
            Entity::container.push_back(zombie);
            zombie->fill_inventory();
            zombie->random_pos(map);
        }
    }

    void everyone_search_player(){
        Debugging::write("(Zombie pathfinder manager) - Algorythm initiated", "../algo.log", true, true);
        searching = true;
        while(ThreadManager::get_state_of(ThreadManager::Thread::zombie_pathfinding) != ThreadManager::State::Terminated){
            if(ThreadManager::get_state_of(ThreadManager::Thread::zombie_pathfinding) == ThreadManager::State::Actived && 
                Entity::get_map() != nullptr && Entity::get_map()->is_updated() && TheApocalipse::is_running()){
                algo_paused = false;
                for(int i = 0; i < Zombie::container.size() && ThreadManager::get_state_of(ThreadManager::Thread::zombie_pathfinding) == ThreadManager::State::Actived; i++)
                    Zombie::container[i]->locates_player();
            }
            else if(ThreadManager::get_state_of(ThreadManager::Thread::zombie_pathfinding) == ThreadManager::State::Paused &&
                !algo_paused){
                algo_paused = true;
                Debugging::write("(Zombie pathfinder manager) - Algorythm paused", "../algo.log", true, true);
            }
        }
        searching = false;
        Debugging::write("(Zombie pathfinder manager) - Algorythm terminated", "../algo.log", true, true);
    }

    bool are_searching(){
        return searching;
    }

}