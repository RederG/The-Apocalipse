#include "food.hpp"

namespace Food{
    std::vector<std::shared_ptr<Object>> all_foods;

    Object::Object(){
        this->item_name = Names.at(natural);
        this->id = std::to_string(Main::random_value() % 1000) + "_food_item_" + std::to_string(Main::random_value() % 1000);
        this->food_type = natural;
        this->attack = 0;
        this->direction = Item::right;
        this->type = Item::Type::food;
        sf::IntRect sprite_rect({0,0},{32,32});
        this->item_texture = {
            {Item::left, sf::IntRect({0,0}, {32,32})},
            {Item::right, sf::IntRect({0,64}, {32,32})},
            {Item::up, sf::IntRect({0,96}, {32,32})},
            {Item::down, sf::IntRect({0,32}, {32,32})},
        };
        this->number = 1;
        this->max_number = 1;
        this->texture_name = food_type_textures.at(this->food_type);
        this->HandleZone = {
            {Item::down, sf::Vector2f(16,16)},
            {Item::up, sf::Vector2f(16,16)},
            {Item::left, sf::Vector2f(16,16)},
            {Item::right, sf::Vector2f(16,16)},
        };
    }

    Object::Object(Type food_type){
        this->item_name = Names.at(food_type);
        this->id = std::to_string(Main::random_value() % 1000) + "_food_item_" + std::to_string(Main::random_value() % 1000);
        this->food_type = food_type;
        attack = 0;
        direction = Item::right;
        type = Item::Type::simple;
        sf::IntRect sprite_rect({0,0},{32,32});
        item_texture = {
            {Item::left, sf::IntRect({0,0}, {32,32})},
            {Item::right, sf::IntRect({0,64},{ 32,32})},
            {Item::up, sf::IntRect({0,96}, {32,32})},
            {Item::down, sf::IntRect({0,32}, {32,32})}
        };
        number = 1;
        max_number = 1;
        texture_name = food_type_textures.at(this->food_type);
    }

    Type Object::get_food_type(){
        return this->food_type;
    }

    void Object::use_item(Entity::Object &entity, bool special_use){
        if(this->number > 0){
            entity.get_satiated(this->satiation_value);
            this->number--;
        }
    }

    void Object::set_number(){
        std::string number = Probability::get_universe("food_number")->get_result();
        if(number == "max")
            this->number = this->max_number;
        else if(number == "half")
            this->number = this->max_number/2;
        else if(number == "low")
            this->number = this->max_number/4;
        else if(number == "very low")
            this->number = 1;

        int random;
        // Reducing or adding
        if(random % 2 == 0){
            this->number -= Main::random_value() % 5;
        }
        else if(random % 2 == 1){
            this->number += Main::random_value() % 5;
        }

        // Verifing the value
        if(this->number > this->max_number){
            this->number = max_number;
        }
        if(this->number <= 0){
            this->number = 1;
        }
    }

    std::shared_ptr<Object> get_food(std::string name, std::string id){
        std::shared_ptr<Object> food = nullptr;
        for(int i = 0; i < all_foods.size(); i++){
            std::shared_ptr<Object> food_ptr = all_foods[i];
            if(food_ptr != nullptr && food_ptr->get_name() == name && food_ptr->get_id() == id){
                food = food_ptr;
                break;
            }
        }
        return food;
    }

};