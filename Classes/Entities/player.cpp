#include "player.hpp"
#include "cmath"
#include <unordered_map>
#include "../Items/all_items.hpp"
#include "../Graphicals/KeysManager.hpp"

#define PI 3.14

std::list< std::shared_ptr<Player::Object> > Player::container;

namespace Player{

    Object::Object(std::string player_name){
        this->id = "entity_" + player_name + "_player_" + std::to_string(Main::random_value()%1000);
        this->name = player_name;
        this->life = 100;
        this->type = Entity::Player;
        this->element_type = MapElement::Type::Entity_being;
        this->velocity = Entity::Velocity.at(Entity::Player);
        this->direction = Entity::Direction::right;
        this->animation_state = Entity::AnimationState::Default;
        this->animation_rect = sf::IntRect({32, 0}, {32, 32});
        this->sprite_sheet_position = sf::IntRect({32, 0}, {32, 32});
        this->life_bar = new Life_Bar::Object(100);
        this->life_bar->set_new_life_to(this->life);
    }

    void Object::move(){
        float x_map = 0;
        float y_map = 0;

        switch_animation_to(Entity::AnimationState::Default);
        this->moving = false;

        if (this->get_state() == Player::State::Playing){
            if(KeysManager::try_to(KeysManager::Action::move_up)){
                set_direction_to(Entity::Direction::up);
                switch_animation_to(Entity::AnimationState::Animated);
                this->moving = true;
                y_map = -this->velocity;
            }

            else if(KeysManager::try_to(KeysManager::Action::move_down)){
                set_direction_to(Entity::Direction::down);
                switch_animation_to(Entity::AnimationState::Animated);
                this->moving = true;
                y_map = this->velocity;
            }

            if(KeysManager::try_to(KeysManager::Action::move_left)){
                set_direction_to(Entity::Direction::left);
                switch_animation_to(Entity::AnimationState::Animated);
                this->moving = true;
                x_map = -this->velocity;
            }

            else if(KeysManager::try_to(KeysManager::Action::move_right)){
                set_direction_to(Entity::Direction::right);
                switch_animation_to(Entity::AnimationState::Animated);
                this->moving = true;
                x_map = this->velocity;
            }
        }

        this->move_on_map(x_map, y_map);
    }

    void Object::set_state(Player::State new_state){
        state = new_state;
    }

    sf::Sprite Object::get_sprite(Map::Object* map, sf::Color color){
        sf::Sprite entity_sprite(*AllTextures::get("soldier"));
        
        Item::Object *item_handling = &*this->get_inventory()->get_current_item();

        if(item_handling != nullptr && typeid(*this) == typeid(Player::Object)){
            if(item_handling->get_type() == Item::Type::weapon){
                this->switch_animation_to(Entity::AnimationState::Handling_weapon);
                entity_sprite.setTexture(*AllTextures::get("soldier_handling_item_or_weapon"));
            }
            else if(item_handling->get_type() == Item::Type::guns){
                this->switch_animation_to(Entity::AnimationState::Handling_weapon);
                entity_sprite.setTexture(*AllTextures::get("soldier_handling_guns"));
            }
            else{
                this->switch_animation_to(Entity::AnimationState::Handling_item);
                entity_sprite.setTexture(*AllTextures::get("soldier_handling_item_or_weapon"));
            }
        }
        else{
            entity_sprite.setTexture(*AllTextures::get("soldier"));
        }

        if (this->moving == true){
            this->animate();
            this->animation_rect.position.y = int(this->direction);
        }
        
        if(this->moving == false || animation_state == Entity::AnimationState::Default){
            this->reset_animation();
            this->animation_rect.position.y = int(this->direction);
        }
        
        sf::Vector2f scale = map->get_scale();
        scale.x *= Entity::ENTITY_SCALE.x;
        scale.y *= Entity::ENTITY_SCALE.y;

        entity_sprite.setScale(scale);
        entity_sprite.setTextureRect(this->animation_rect);
        entity_sprite.setPosition(this->get_window_position_on(map));
        entity_sprite.setColor(color);

        sf::Vector2f origin;
        origin.x = this->animation_rect.size.x/2;
        origin.y = this->animation_rect.size.y - 4;
        entity_sprite.setOrigin(origin);
        
        return entity_sprite;
    }

    void Object::take_item_of(Tomb::Object &tomb){
        if(tomb.get_inventory()->get_current_item()->add_to_container(this->inventory) == true)
            tomb.get_inventory()->remove_current_item();
    }

    Tomb::Object* Object::get_nearest_tomb(int minimal_distance){
        Tomb::Object* nearest_tomb = nullptr;
        std::map<float, Tomb::Object*> all_tombs;
        std::list<float> all_distances;
        for(auto tomb : Tomb::container){
            sf::Vector2f tomb_position = tomb->get_map_position();
            sf::Vector2f player_position = this->get_map_position();

            sf::Vector2f vector_distance;
            vector_distance.x = tomb_position.x - player_position.x;
            vector_distance.y = tomb_position.y - player_position.y;

            float real_distance = sqrt(vector_distance.x*vector_distance.x + vector_distance.y*vector_distance.y);
            all_distances.push_back(real_distance);
            all_tombs[real_distance] = &*tomb;
        }
        all_distances.sort();
        if(all_distances.front() <= float(minimal_distance))
            nearest_tomb = &*all_tombs[all_distances.front()];
        return nearest_tomb;
    }

    Player::State Object::get_state(){
        return this->state;
    }

    void Object::update(){
        this->move();
        this->inventory->update();
        this->effect_manager->update_on(this);
        this->effect_manager->update();
        if(this->satiation_clock.getElapsedTime().asSeconds() >= this->satiation_diminution_lapse_time){
            this->satiation_clock.restart();
            this->satiation -= 0.25f;
            if(this->satiation <= 0)
                this->life = 0;
            this->satiation_diminution_lapse_time = this->satiation_diminution_lapse_time_original;
        }
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

            file.write(reinterpret_cast<char*>(&this->state), sizeof(this->state));

            file.close();
        }
    }

    void Object::load_from(std::string location_path){
        if(!std::filesystem::exists(location_path))
            Debugging::write(location_path + " does not exists (player loading)");
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

                file.read(reinterpret_cast<char*>(&this->state), sizeof(this->state));

                file.close();
            }
            this->life_bar->set_new_life_to(this->life);
        }
    }

    void Object::fill_inventory(){}

    void move(int fps){
        //move(fps);
    }

    void create_player(std::string player_name, Map::Object* map){
        std::shared_ptr<Player::Object> player = std::make_shared<Player::Object>( *new Player::Object(player_name) );
        Entity::container.push_back(player);
        Player::container.push_back(player);
        Knife* knife = new Knife();
        knife->add_to_container(player->get_inventory());
        sf::Vector2i player_position = map->get_spawn_position(map->get_noise()->get_seed());
        player->set_map_position_to({float(player_position.x), float(player_position.y)});
    }

    void set_state(Player::State new_state){
        for (auto Player : Player::container){
            Player->set_state(new_state);
        }
    }

    void get_state(){
        for (auto player : Player::container)
            player->get_state();
    }

}