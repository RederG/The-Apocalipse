#include "../../headers/InteractiveObjects/wall.hpp"

namespace Wall{
    std::map<Type, sf::IntRect> textures{
        {simple,                {{0, 0}, {32, 48}}},
        {with_window,           {{32, 0}, {32, 48}}},
        {with_door,             {{64, 0}, {32, 48}}},
        {with_open_door,        {{96, 0}, {32, 48}}},
        {down_simple,           {{0, 0}, {32, 48}}},
        {down_with_door,        {{128, 0}, {32, 48}}},
        {down_with_open_door,   {{160, 0}, {32, 48}}},
        {down_with_window,      {{32, 0}, {32, 48}}},
    };

    Object::Object(Type wall_type, sf::Vector2i position){
        this->wall_type = wall_type;
        this->element_type = MapElement::Type::World;
        this->map_position = {float(position.x), float(position.y)};

        if(this->wall_type == Type::simple || this->wall_type == Type::with_window || this->wall_type == Type::down_simple || this->wall_type == Type::down_with_window)
            this->number_of_animation = 0;
        else if(this->wall_type == Type::with_door || this->wall_type == Type::down_with_door)
            this->number_of_animation = 2;

        if(this->wall_type == Type::simple || this->wall_type == Type::down_simple)
            this->interaction_type = InteractiveObjects::Type::nothing;
        else
            this->interaction_type = InteractiveObjects::Type::simple;
    }

    sf::Sprite Object::get_sprite(Map::Object* map, sf::Color color){
        sf::Sprite wall_sprite(*AllTextures::get("struct_default_wall"));
        wall_sprite.setTextureRect(textures.at(this->wall_type));
        wall_sprite.setScale(map->get_scale());
        wall_sprite.setColor(color);
        wall_sprite.setOrigin({0, 16});
        wall_sprite.setPosition(this->get_window_position_on(map));
        return wall_sprite;
    }

    sf::Vector2f Object::get_map_position(bool virtual_position){
        if(virtual_position)
            return {this->map_position.x, this->map_position.y + 1};
        return this->map_position;
    }

    sf::RectangleShape Object::get_collision_rect(Map::Object* map, sf::Color color){
        sf::RectangleShape rect;
        sf::Sprite obj_sprite = this->get_sprite(map, color);

        sf::Vector2f size = {obj_sprite.getGlobalBounds().size};
        if(this->wall_type == down_simple || this->wall_type == down_with_door || this->wall_type == down_with_window)
            size.y /= 2;
        else
            size.y /= 8;

        sf::Vector2f position = obj_sprite.getPosition();
        position.y += 32*map->get_scale().y;

        rect.setSize(size);
        rect.setOrigin({0, size.y});
        rect.setPosition(position);

        if(this->wall_type != down_with_open_door || this->wall_type != with_open_door)
            return rect;
        else
            return *new sf::RectangleShape({0, 0});
    }

    Type get_from(sf::Color color){
        if(color == sf::Color::Black)
            return simple;
        else if(color == sf::Color::Blue)
            return with_window;
        else if(color == sf::Color(100, 100, 100))
            return with_door;
        else if(color == sf::Color(0, 255, 0))
            return down_simple;
        else if(color == sf::Color(255, 255, 255))
            return down_with_door;
        else if(color == sf::Color(255, 0, 0))
            return down_with_window;
        return simple;
    }

};