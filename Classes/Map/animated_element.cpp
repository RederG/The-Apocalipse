#include "map_element.hpp"

namespace MapElement{

    Animated::Animated(){
        this->element_type = Type::World;
        this->map_position = sf::Vector2f(0, 0);
        this->animation_rect = sf::IntRect({0, 0}, {32, 32});
        this->sprite_sheet_position = sf::IntRect({0, 0}, {32, 32});
    }

    Animated::Animated(sf::Vector2f map_position, sf::IntRect sprite_sheet_position){
        this->element_type = Type::World;
        this->map_position = map_position;
        this->animation_rect = sprite_sheet_position;
        this->sprite_sheet_position = sprite_sheet_position;
    }

    void Animated::animate(){
        int nb_animation = Help::animation_number.at(this->element_type);
        float animation_time = Help::animation_time.at(this->element_type);
        if(Main::clock_seconds(animation_time, this->animation_clock) == Main::Reached){
            if(this->animation_rect.position.x == 32*(nb_animation - 1))
                this->animation_rect.position.x = 0;
            else
                this->animation_rect.position.x += 32;
        }
    }

    void Animated::reset_animation(){
        this->animation_rect = this->sprite_sheet_position;
        this->animating = false;
    }

    sf::Sprite Animated::get_sprite(Map::Object* map, sf::Color color){
        return sf::Sprite(*new sf::Texture());
    }

    sf::IntRect Animated::get_animation_rect(){
            return this->animation_rect;
        }

}