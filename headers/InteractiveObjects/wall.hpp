#ifndef __WALL__
    #define __WALL__

    #include "interactive_object.hpp"

    namespace InteractiveObjects{
        class Object;
    };

    namespace Wall{
        enum Type{
            simple, with_window, with_door, with_open_door, down_simple, down_with_door, down_with_open_door, down_with_window
        };

        class Object : public InteractiveObjects::Object{
            public:
                Object(Type wall_type, sf::Vector2i position);
                void act() override;
                sf::Sprite get_sprite(Map::Object* map, sf::Color color = sf::Color::White) override;
                sf::Vector2f get_map_position(bool virtual_position = false) override;
                sf::RectangleShape get_collision_rect(Map::Object* map, sf::Color color = sf::Color::White) override;
            private:
                Type wall_type;
        };

        Type get_from(sf::Color color);
    };

#endif