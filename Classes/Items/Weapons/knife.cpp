#include "knife.hpp"
#include "../../Entities/player.hpp"
#include "cmath"

Knife::Knife(){
    item_name = Item::Names::knife;
    id = to_string(Main::random_value() % 1000) + "_weapon_knife_" + to_string(Main::random_value() % 1000);
    attack = 10;
    type = Item::Type::weapon;
    number = 1;
    max_number = 1;
    durability = 30;
    max_durability = 30;
    item_texture = Weapon::sprite_sheet_position;
    HandleZone = {
        {Item::Direction::left, sf::Vector2f(22, 24)},
        {Item::Direction::right, sf::Vector2f(10, 25)},
        {Item::Direction::down, sf::Vector2f(17, 23)},
        {Item::Direction::up, sf::Vector2f(17, 23)}
    };
    texture_name = "knife";
}

void Knife::draw_attack_rect(sf::RenderWindow &window){
    window.draw(this->attack_rect);
}

void Knife::use_item(Entity::Object &entity, bool special_use){
    if(Main::clock_seconds(0.5, *this->get_using_timelapse_clock()) == Main::Reached){
        sf::Vector2f entity_position = entity.get_map_position();

        const float DISTANCE_MIN = 1.5;
        for(auto entity_ptr : Entity::container){
            sf::Vector2f entity_ptr_position = entity_ptr->get_map_position();

            sf::Vector2f vector_distance = entity_position;
            vector_distance.x -= entity_ptr_position.x;
            vector_distance.y -= entity_ptr_position.y;

            float distance = sqrt(vector_distance.x*vector_distance.x + vector_distance.y*vector_distance.y);
            Entity::Direction entity_direction = entity.get_direction();

            if((entity.get_name() != entity_ptr->get_name() || entity.get_id() != entity_ptr->get_id()) && distance <= DISTANCE_MIN){
                if(entity_direction == Entity::Direction::up && entity_ptr_position.y){
                    this->cut(entity_ptr.get());
                }
                if(entity_direction == Entity::Direction::down && entity_ptr_position.y){
                    this->cut(entity_ptr.get());
                }
                if(entity_direction == Entity::Direction::right && entity_ptr_position.x){
                    this->cut(entity_ptr.get());
                }
                if(entity_direction == Entity::Direction::left && entity_ptr_position.x){
                    this->cut(entity_ptr.get());
                }
            }
        }
    }
}

void Knife::add_to(Container::Object* container, int location){
    if(location >= 0 && location < container->get_max_size() && container->get_item_at(location) == nullptr){
        container->get_contain()[location] = std::make_shared<Knife>(*this);
    }
}

std::string Knife::get_extra_info(){
    return std::to_string(this->attack) + Item::Extra_info::weapon.at(Main::lang());
}

std::string Knife::get_file_name(){
    return Item::Names::knife.at(Main::english);
}

void Knife::cut(Entity::Object* entity){
    entity->set_state_to(Entity::State::Hurting);
    entity->get_attacked(this->attack);
    this->durability--;
}

Knife* new_knife(){
    auto knife(std::make_shared<Knife>(*new Knife()));
    Knife* pointer = &*knife;
    return pointer;
}
