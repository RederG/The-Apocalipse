#include "weapon.hpp"

namespace Weapon{

    Object::Object(string weapon_name, int weapon_attack, string weapon_texture_path, string weapon_texture_on_invnetory_path){
        this->id = to_string(Main::random_value() % 1000) + "_weapon_" + to_string(Main::random_value() % 1000);
        this->attack = weapon_attack;
        this->type = Item::Type::weapon;
        sf::IntRect sprite_rect({0,0}, {32,32});
        this->item_texture = {
            {Item::Direction::left, sf::IntRect({0,0}, {32,32})},
            {Item::Direction::right, sf::IntRect({0,64}, {32,32})},
            {Item::Direction::up, sf::IntRect({0,96}, {32,32})},
            {Item::Direction::down, sf::IntRect({0,32}, {32,32})},
        };
    }

    void Object::set_durability(){
        std::string durability = Probability::get_universe("durability")->get_result();
        if(durability == "max"){
            this->durability = this->max_durability;
        }
        if(durability == "half"){
            this->durability = this->max_durability/2;
        }
        if(durability == "low"){
            this->durability = this->max_durability/3;
        }
        if(durability == "very low"){
            this->durability = this->max_durability/5;
        }

        int random;
        // Reducing or adding
        if(random % 2 == 0){
            this->durability -= Main::random_value() % 5;
        }
        else if(random % 2 == 1){
            this->durability += Main::random_value() % 5;
        }

        // Verifing the value
        if(this->durability > this->max_durability){
            this->durability = max_durability;
        }
        if(this->durability <= 0){
            this->durability = 1;
        }
    }

}

