#ifndef __ROOF__
    #define __ROOF__

    #include "interactive_object.hpp"

    namespace Roof{
        enum Type{
            default_roof
        };

        class Object : public InteractiveObjects::Object{
            public:
                Object(Type roof_type, sf::Vector2i position);
                sf::Sprite get_sprite(Map::Object* map, sf::Color color = sf::Color::White) override;
            private:
                Type roof_type;
        };
    };

#endif