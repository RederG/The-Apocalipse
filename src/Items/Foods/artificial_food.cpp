#include "../../../headers/Items/Foods/artificial_food.hpp"

namespace Artificial_food{
    Object::Object(Foods_names food_name){
        this->item_name = Names.at(food_name);
        this->id = std::to_string(Main::random_value() % 1000) + "_artificial_food_item_" + std::to_string(Main::random_value() % 1000);
        this->food_type = Food::artificial;
        this->food_name = food_name;
        this->attack = 0;
        this->direction = Item::right;
        this->type = Item::Type::food;
        sf::IntRect sprite_rect({0,0}, {32,32});
        this->item_texture = {
            {Item::left, textures_location.at(food_name)},
            {Item::right, textures_location.at(food_name)},
            {Item::up, textures_location.at(food_name)},
            {Item::down, textures_location.at(food_name)},
        };
        this->number = 1;
        this->max_number = 10;
        this->texture_name = Food::food_type_textures.at(this->food_type);
        this->HandleZone = {
            {Item::down, sf::Vector2f(16,16)},
            {Item::up, sf::Vector2f(16,16)},
            {Item::left, sf::Vector2f(16,16)},
            {Item::right, sf::Vector2f(16,16)},
        };
        this->satiation_value = satiation_values.at(this->food_name);
        this->set_number();
    }

    void Object::add_to(Container::Object* container, int location){
        if(location >= 0 && location < container->get_max_size() && container->get_item_at(location) == nullptr){
            container->get_contain()[location] = std::make_shared<Object>(*this);
        }
    }

    bool Object::add_to_container(Container::Object* container, bool aleatory){
        bool added = false;
        std::shared_ptr<Object> same_artificial_food = nullptr;
        for(int container_location = 0; container_location < container->get_max_size(); container_location++){
            if(container->get_item_at(container_location) != nullptr && typeid(*this) == typeid(*container->get_item_at(container_location))){
                same_artificial_food = std::static_pointer_cast<Object>(container->get_contain()[container_location]);
                if(same_artificial_food != nullptr && same_artificial_food->food_name != this->food_name){
                    same_artificial_food = nullptr;
                }
            }
        }
        if(same_artificial_food == nullptr && container->is_full() == false){
            if(aleatory == false){
                for(int location = 0; location < container->get_max_size(); location++){
                    if(container->get_contain()[location] == nullptr){
                        auto food_ptr = Food::get_food(this->get_name(), this->get_id());
                        container->get_contain()[location] = static_pointer_cast<Object>(food_ptr);
                        added = true;
                        break;
                    }
                }
            }
            else{
                while(added == false){
                    int location = std::rand() % container->get_max_size();
                    if(container->get_contain()[location] == nullptr){
                        added = true;
                        auto food_ptr = Food::get_food(this->get_name(), this->get_id());
                        container->get_contain()[location] = static_pointer_cast<Object>(food_ptr);
                    }
                }
            }
        }
        else if(same_artificial_food != nullptr){
            same_artificial_food->number += this->number;
            this->number = 0;
            if(same_artificial_food->number > same_artificial_food->max_number){
                int extra_number = same_artificial_food->number - same_artificial_food->max_number;
                this->number += extra_number;
                same_artificial_food->number = same_artificial_food->max_number;
                for(int i = 0; i < container->get_max_size(); i++){
                    if(container->get_contain()[i] == nullptr){
                        auto food_ptr = Food::get_food(this->get_name(), this->get_id());
                        container->get_contain()[i] = static_pointer_cast<Object>(food_ptr);
                        break;
                    }
                }
            }
            added = true;
            if(this->number > 0 && !container->is_full()){
                for(int container_location = 0; container_location < container->get_max_size(); container_location++){
                    if(container->get_contain()[container_location] == nullptr){
                        auto food_ptr = Food::get_food(this->get_name(), this->get_id());
                        container->get_contain()[container_location] = static_pointer_cast<Object>(food_ptr);
                        break;
                    }
                }
            }
            added = true;
        }

        return added;
    }

    std::string Object::get_extra_info(){
        return Extra_info.at(Main::lang()) + std::to_string(int(this->satiation_value)) + Extra_info_suite.at(Main::lang());
    }

    std::string Object::get_file_name(){
        return Food::Names.at(this->food_type).at(Main::english);
    }

    Object* create_new(Foods_names food_name){
        std::shared_ptr<Object> new_artificial_food = std::make_shared<Object>(*new Object(food_name));
        Food::all_foods.push_back(new_artificial_food);
        return &*new_artificial_food;
    }
};