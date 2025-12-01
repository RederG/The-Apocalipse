#include "roof.hpp"

namespace Roof{
    Object::Object(Type roof_type, sf::Vector2i position){
        this->roof_type = roof_type;
        this->map_position = {float(position.x), float(position.y)};
    }

    sf::Sprite Object::get_sprite(Map::Object* map, sf::Color color){
        sf::Sprite roof_sprite(*AllTextures::get("struct_default_roof"));
        sf::Vector2f scale = map->get_scale();
        scale.x *= 18;
        scale.y *= 18;

        sf::Vector2f position = this->get_window_position_on(map);
        position.y -= 32*map->get_scale().y*2;

        roof_sprite.setScale(scale);
        roof_sprite.setColor(color);
        roof_sprite.setPosition(position);
        return roof_sprite;
    }
};