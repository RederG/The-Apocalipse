#include "effect_item.hpp"

namespace Effect_item{
    Object::Object(){
        this->item_name = Names;
        this->id = std::to_string(Main::random_value() % 1000) + "_helping_item_" + std::to_string(Main::random_value() % 1000);
        attack = 0;
        direction = Item::right;
        type = Item::Type::effect;
        sf::IntRect sprite_rect({0,0}, {32,32});
        item_texture = {
            {Item::left, sf::IntRect({0,0}, {32,32})},
            {Item::right, sf::IntRect({0,64}, {32,32})},
            {Item::up, sf::IntRect({0,96}, {32,32})},
            {Item::down, sf::IntRect({0,32}, {32,32})}
        };
        number = 1;
        max_number = 1;
        texture_name = "knife";
    }

    void Object::use_item(Entity::Object &entity, bool special_use){
        this->create_effect_for(&entity);
    }

    unsigned int Object::get_number(bool special){
        int number = 1;
        if(special == true){
            number = this->durability;
        }
        else{
            number = this->number;
        }
        return number;
    }

    void Object::create_effect_for(Entity::Object* entity){}

};