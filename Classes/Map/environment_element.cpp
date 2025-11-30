#include "map_element.hpp"

namespace MapElement{

    Environment::Environment(WorldContent element, sf::Vector2f map_position){
        this->environment_type = element;
        this->element_type = Type::World;
        this->map_position = map_position;
    }

    WorldContent Environment::get_environment_element(){
        return this->environment_type;
    }

    bool Environment::can_move_on(){
        bool can_move = true;
        switch(this->environment_type){
            case WorldContent::walls:
                can_move = false;
                break;
            case WorldContent::border_map:
                can_move = false;
                break;
        }
        return can_move;
    }

    bool Environment::is_zombie_path(){
        return this->zombie_path;
    }

    bool Environment::is_path_finding(){
        return this->path_finding;
    }

    sf::Sprite Environment::get_sprite(Map::Object* map, sf::Color color){
        sf::Sprite element_sprite(*AllTextures::get("the_apocalipse_map_elements"));
        element_sprite.setTextureRect(Help::elements_position.at(this->environment_type));
        element_sprite.setScale(map->get_scale());
        element_sprite.setPosition(this->get_window_position_on(map));
        element_sprite.setColor(color);
        return element_sprite;
    }

    void Environment::set_zombie_path_to(bool zombie_path){
        this->zombie_path = zombie_path;
    }

    void Environment::set_path_finding_to(bool path_finding){
        this->path_finding = path_finding;
    }

}