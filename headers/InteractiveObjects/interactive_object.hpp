#ifndef __INTERACTIVE_OBJECT__
    #define __INTERACTIVE_OBJECT__

    #include "../Map/map_element.hpp"

    namespace MapElement{
        class Animated;
    };

    namespace InteractiveObjects{

        enum Type{
            nothing, simple, container
        };

        class Object : public MapElement::Animated{
            public:
                Object();
                Type get_interaction_type();
                sf::RectangleShape get_collision_rect(Map::Object* map, sf::Color color = sf::Color::White) override;
            protected:
                Type interaction_type;
        };

        void event_detected(bool event_detection);
        
    }

#endif