#include "entity.hpp"
#include "zombie.hpp"
#include "player.hpp"
#include "../InteractiveObjects/tomb.hpp"
#include "../../window_menu/game_window.hpp"

namespace Entity{
    std::list< std::shared_ptr<Entity::Object> > container;
    Object* temporary_entity = nullptr;
    Map::Object* the_map = nullptr;

    Object::Object(){
        name = "entity";
        id = "entity_" + *&name + "_e_" + std::to_string(Main::random_value()%1000);
        life = 100;
        direction = Entity::Direction::right;
        this->clock_hurting;
        animation_state = Entity::AnimationState::Default;
        element_type = MapElement::Type::Entity_being;
        velocity = Entity::Velocity.at(Entity);
        life_bar = new Life_Bar::Object(100);
        life_bar->set_new_life_to(life);
    }

    std::string Object::get_name(){
        return name;
    }

    std::string Object::get_id(){
        return id;
    }

    Type Object::get_type(){
        return this->type;
    }

    Container::Object* Object::get_inventory(){
        return this->inventory;
    }

    float Object::get_life(){
        return this->life;
    }

    float Object::get_max_life(){
        return this->max_life;
    }

    Entity::State Object::get_state(){
        return state;
    }

    Entity::Direction Object::get_direction(){
        return this->direction;
    }

    Life_Bar::Object* Object::get_life_bar(){
        return this->life_bar;
    }

    sf::Clock* Object::get_hurting_time_clock(){
        sf::Clock *clock = &this->clock_hurting;
        return clock;
    }

    bool Object::is_moving(){
        return this->moving;
    }

    bool Object::is_colliding_with_environment(){
        bool collision = false;
        if(Entity::get_map() != nullptr){
            sf::Vector2i entity_area = Entity::get_map()->get_area_position(this->map_position.x, this->map_position.y);
            for(int y = entity_area.y - 1; y <= entity_area.y + 1; y++){
                for(int x = entity_area.x - 1; x <= entity_area.x + 1; x++){
                    if(StructureManager::is_structure({x, y})){
                        Structure::Instance& structure = StructureManager::get_structure(sf::Vector2i({x, y}));
                        for(int i = 0; i < structure.contain.walls.size(); i++){
                            if(structure.contain.walls[i]->collides_with(this, Entity::get_map())){
                                collision = true;
                                break;
                            }
                        }
                    }
                    if(collision)
                        break;
                }
                if(collision)
                    break;
            }
        }
        return collision;
    }

    bool Object::is_colliding_with_other_entities(){
        bool collision = false;
        for(auto entity : Entity::container){
            if((this->id != entity->id || this->name != entity->name) && this->collides_with(&*entity, Entity::get_map())){
                collision = true;
            }
            for(auto tomb : Tomb::container){
                if(this->collides_with(&*tomb, Entity::get_map())){
                    collision = true;
                    break;
                }
            }
            if(collision == true)
                break;
        }
        return collision;
    }

    bool Object::is_alive(){
        bool alive = true;
        if(this->life <= 0)
            alive = false;
        return alive;
    }

    sf::Sprite Object::get_sprite(Map::Object* map, sf::Color color){
        return sf::Sprite(*new sf::Texture());
    }

    sf::RectangleShape Object::get_collision_rect(Map::Object* map, sf::Color color){
        sf::Sprite entity_sprite = this->get_sprite(map);
        sf::Vector2f entity_scale = entity_sprite.getScale();
        sf::RectangleShape rect;

        sf::Vector2f rect_size = entity_sprite.getGlobalBounds().size;
        rect_size.x /= 2;
        rect_size.y /= 4; rect_size.y -= 2*map->get_scale().y;
        rect.setSize(rect_size);

        sf::Vector2f rect_origin;
        rect_origin.x = rect.getSize().x/2;
        rect_origin.y = rect.getSize().y + entity_scale.y;
        rect.setOrigin(rect_origin);

        rect.setPosition(entity_sprite.getPosition());
        rect.setFillColor(color);
        return rect;
    }

    sf::Vector2f Object::get_hand_location(Map::Object* map){
        sf::Vector2f entity_scale = this->get_sprite(map).getScale();

        std::map<Direction, sf::Vector2f> hand_position{
            {right, sf::Vector2f(22, 18)},
            {down, sf::Vector2f(12, 20)},
            {left, sf::Vector2f(8, 18)},
            {up, sf::Vector2f(24, 20)}
        };

        sf::Vector2f hand_position_scaled = hand_position.at(this->direction);
        hand_position_scaled.x *= entity_scale.x;
        hand_position_scaled.y *= entity_scale.y;

        return hand_position_scaled;
    }

    Effect_manager* Object::get_effect_manager(){
        return this->effect_manager;
    }

    void Object::set_movement_to(bool movement_state){
        this->moving = movement_state;
    }

    void Object::set_direction_to(Entity::Direction new_direction){
        direction = new_direction;
    }

    void Object::set_name(std::string new_name){
        name = new_name;
    }

    void Object::set_animation_by_handling_item(){}

    void Object::set_state_to(Entity::State new_state){
        state = new_state;
        if (new_state == Entity::State::Hurting)
            clock_hurting.restart();
    }

    void Object::get_attacked(int attack){
        this->life -= attack;
        this->life_bar->set_new_life_to(this->life);
        this->life_bar->reset_timer();
        this->clock_hurting.restart();
        if(this->life <= 0)
            this->set_state_to(Entity::State::Died);
    }

    void Object::get_healed(float value){
        this->life += value;
        if(this->life > this->max_life){
            this->life = this->max_life;
        }
        this->state = Entity::State::Healed;
        this->clock_healing.restart();
        this->life_bar->set_new_life_to(this->life);
        this->life_bar->reset_timer();
    }

    void Object::draw_with_item(Map::Object* map, sf::Color color){
        if(this->get_inventory()->get_current_item() != nullptr){
            if(this->direction == Entity::Direction::down){
                map->get_window()->draw(this->get_sprite(map, color));
                this->get_inventory()->get_current_item()->draw_to(this, map);
            }
            else if(this->direction != Entity::Direction::down){
                this->get_inventory()->get_current_item()->draw_to(this, map);
                map->get_window()->draw(this->get_sprite(map, color));
            }
        }
        else{
            map->get_window()->draw(this->get_sprite(map, color));
        }
    }
    
    void Object::draw_to(Map::Object* map, sf::Color color){
        if(this->get_state() == Entity::State::Hurting){
            this->draw_with_item(map, sf::Color::Red);
            if(Main::clock_seconds(0.1, *this->get_hurting_time_clock()))
                this->set_state_to(Entity::State::Alive);
        }
        else if(this->get_state() == Entity::State::Healed){
            this->draw_with_item(map, sf::Color::Green);
            if(Main::clock_seconds(0.1, this->clock_healing))
                this->set_state_to(Entity::State::Alive);
        }
        else
            this->draw_with_item(map);
        this->get_life_bar()->draw_to(&*this, map);
    }

    void Object::switch_animation_to(Entity::AnimationState new_animation_state){
        animation_state = new_animation_state;
    }

    void Object::fill_inventory(){}

    void Object::move_on_map(float x, float y){
        if(Main::clock_millisec(5, this->clock_moving) == Main::GameTimer::Reached){
            sf::Vector2f old_position = this->map_position;
            this->map_position.x += x;
            this->map_position.y += y;

            if(this->is_colliding_with_environment() || this->is_colliding_with_other_entities()){
                this->map_position = old_position;
                this->moving = false;
            }
        }
    }

    void Object::destroy(){
        delete this->life_bar;
        delete this->inventory;
        delete this->effect_manager;
        this->life_bar = nullptr;
        this->inventory = nullptr;
        this->effect_manager = nullptr;
    }

    void Object::update(){
        this->move();
        this->inventory->update();
        this->effect_manager->update_on(this);
        this->effect_manager->update();
    }

    float Object::get_satiation(){
        return this->satiation;
    }

    float Object::get_max_satiation(){
        return this->max_satiation;
    }

    sf::Clock Object::get_satiation_clock(){
        return this->satiation_clock;
    }

    void Object::get_satiated(float satiation_value){
        this->satiation += satiation_value;
        if(this->satiation > this->max_satiation)
            this->satiation = this->max_satiation;
    }

    void Object::get_hungry(float hungry_value){
        this->satiation -= hungry_value;
        if(this->satiation < 0)
            this->satiation = 0;
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

            file.write(reinterpret_cast<char*>(&this->state), sizeof(this->state));
            file.write(reinterpret_cast<char*>(&this->type), sizeof(this->type));

            file.write(reinterpret_cast<char*>(&this->satiation), sizeof(float));
            file.write(reinterpret_cast<char*>(&this->max_satiation), sizeof(float));

            file.write(reinterpret_cast<char*>(&this->satiation_diminution_lapse_time), sizeof(float));
            file.write(reinterpret_cast<char*>(&this->satiation_diminution_lapse_time_original), sizeof(float));

            file.write(reinterpret_cast<char*>(&this->map_position.x), sizeof(float));
            file.write(reinterpret_cast<char*>(&this->map_position.y), sizeof(float));

            file.close();
        }
    }

    void Object::load_from(std::string location_path){
        if(!std::filesystem::exists(location_path))
            Debugging::write(location_path + " does not exists (entity loading)");
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

                file.close();
            }
            this->life_bar->set_new_life_to(this->life);
        }
    }

    void init(){
        Debugging::write("Entities initialisation");
        Entity::container.clear();
        Player::container.clear();
        Zombie::container.clear();
        the_map = nullptr;
        Debugging::write("Entities initialisation terminated\n");
    }

    void terminates(){
        Debugging::write("Removing all entities", Debugging::get_state_of(Debugging::In_game));
        for(auto& player : Player::container){
            player.reset();
        }
        Player::container.clear();
        for(auto& zombie : Zombie::container){
            zombie.reset();
        }
        Zombie::container.clear();
        for(auto entity : Entity::container){
            entity->destroy();
            entity.reset();
        }
        Entity::container.clear();
        the_map = nullptr;
        Debugging::write("Removing all entities terminated", Debugging::get_state_of(Debugging::In_game));
    }

    void draw_all_to(Map::Object* map){
        if(container.empty() == false){
            for(auto entity : container){
                entity->draw_with_item(map);
            }
        }
    }

    bool verify_entity(){
        bool there_is_a_dead_entity = false;
        std::shared_ptr<Zombie::Object> zombie_ptr = nullptr;
        for (auto zombie : Zombie::container){
            if(zombie->is_alive() == false){
                Tomb::set_tomb_of(*zombie);
                Entity::container.remove(zombie);
                Zombie::container.remove(zombie);
                there_is_a_dead_entity = true;
                break;
            }
        }
        return there_is_a_dead_entity;
    }
    
    void set_temporary_entity(Object* entity){
        temporary_entity = entity;
    }

    Object* get_temporary_entity(){
        return temporary_entity;
    }

    void reset_temporary_entity(){
        temporary_entity = nullptr;
    }

    void set_all_nearest_interactive_objects(){
        if(Entity::get_map() != nullptr){
            for(auto entity : Entity::container)
                entity->set_nearest_interactive_object_on(Entity::get_map(), 2.0f);
        }
    }

    void set_map_to(Map::Object* map){
        the_map = map;
    }

    Map::Object* get_map(){
        return the_map;
    }

    void update_all(){
        if(Entity::get_map() != nullptr){
            for(auto entity : container)
                entity->update();
        }
    }

    void set_entities_movement_to(bool movement_allowed){
        for(auto entity : container){
            entity->set_movement_to(movement_allowed);
        }
    }

    void save_all_to(std::string location_path, std::string inventories_path, std::string items_path, std::string effect_managers_path, std::string effects_path){
        std::filesystem::remove_all(location_path);
        std::filesystem::create_directory(location_path);

        std::filesystem::remove_all(inventories_path);
        std::filesystem::create_directory(inventories_path);

        std::filesystem::remove_all(items_path);
        std::filesystem::create_directory(items_path);

        std::filesystem::remove_all(effect_managers_path);
        std::filesystem::create_directory(effect_managers_path);

        std::filesystem::remove_all(effects_path);
        std::filesystem::create_directory(effects_path);
        for(auto entity : container){
            entity->save_to(location_path);
            entity->get_inventory()->save_to(inventories_path, entity->get_id(), items_path);
            entity->get_effect_manager()->save_to(effect_managers_path, entity->get_id(), effects_path);
        }
    }

    void load_all_from(std::string location_path, std::string inventories_path, std::string items_path, std::string effect_managers_path, std::string effects_path){
        if(!std::filesystem::exists(location_path))
            Debugging::write(location_path + " does not exists (loading all  entities)");
        else{
            std::list<std::string> entities_files;
            Main::get_files_in(location_path + "/", entities_files);

            Zombie::container.clear();
            Player::container.clear();
            Entity::container.clear();

            std::string path = location_path;
            for(std::string entity_file : entities_files){

                std::string entity_file_path = path + "/" + entity_file;

                std::fstream file(entity_file_path, std::ios::in | std::ios::binary);
                if(file.is_open()){
                    std::size_t name_size = 0;
                    file.read(reinterpret_cast<char*>(&name_size), sizeof(std::size_t));
                    
                    std::string name = "";
                    name.resize(name_size);
                    file.read(&name[0], sizeof(char)*name_size);

                    if(name == "zombie"){
                        std::shared_ptr<Zombie::Object> zombie = std::make_shared<Zombie::Object>(*new Zombie::Object());
                        Entity::container.push_back(zombie);
                        Zombie::container.push_back(zombie);
                        zombie->load_from(entity_file_path);
                    }
                    else{
                        std::shared_ptr<Player::Object> player = std::make_shared<Player::Object>(*new Player::Object(""));
                        Entity::container.push_back(player);
                        Player::container.push_back(player);
                        player->load_from(entity_file_path);
                    }

                    file.close();
                }
            }
            for(auto entity : Entity::container){
                if(std::filesystem::exists(inventories_path + "/" + entity->get_id() + ".inventory") && std::filesystem::exists(items_path))
                    entity->get_inventory()->load_from(inventories_path + "/" + entity->get_id() + ".inventory", items_path);
                if(std::filesystem::exists(effect_managers_path + "/" + entity->get_id() + ".effman") && std::filesystem::exists(effects_path))
                    entity->get_effect_manager()->load_from(effect_managers_path + "/" + entity->get_id() + ".effman", effects_path);
            }
        }
    }

}