#include "interactive_object.hpp"

namespace InteractiveObjects{
    bool event_exists = false;

    void event_detected(bool event_detection){
        event_exists = event_detection;
    }

    Object::Object(){}

    Type Object::get_interaction_type(){
        return this->interaction_type;
    }

    sf::RectangleShape Object::get_collision_rect(Map::Object* map, sf::Color color){
        sf::RectangleShape rect;
        sf::Sprite obj_sprite = this->get_sprite(map, color);

        sf::Vector2f size = {obj_sprite.getGlobalBounds().size};
        size.y /= 4;

        sf::Vector2f position = obj_sprite.getPosition();
        position.y += obj_sprite.getOrigin().y*map->get_scale().x;

        rect.setSize(size);
        rect.setOrigin({0, size.y});
        rect.setPosition(position);

        return rect;
    }

}