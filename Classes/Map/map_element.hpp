#ifndef __MAP_ELEMENT__
    #define __MAP_ELEMENT__

    #include "../../main.hpp"

    namespace InteractiveObjects{
        class Object;
    };

    namespace Map{
        class Object;
    };

    // All map's elements
    namespace MapElement{
        
        enum WorldContent{
            nothing,
            border_map,
            walls,
            soil_rocks_1, soil_rocks_2, soil_rocks_3,
            soil_herb_1, soil_herb_2, soil_herb_3,
            spawn_zombie_1, spawn_zombie_2, spawn_zombie_3,
        };
        
        // All types of elements
        enum Type{
            Entity_being, World
        };
    
        // A map element that can have animation
        class Animated;

        // An environment element
        class Environment;
        
        // All elements for a map (Not animated)
        class Object{
            public:
                // The constructor of a map's element
                Object();

                // Draws the element on the map
                void virtual draw_to(Map::Object* map, sf::Color color = sf::Color::White);

                // Moves the element on the map
                void virtual move_on_map(float x, float y);

                // Moves the element on the map
                void move_on_map(sf::Vector2f distance);

                // Sets the new position of the element on the map
                void set_map_position_to(float x, float y);

                // Sets the new position of the element on the map
                void set_map_position_to(sf::Vector2f new_position);

                // Sets the nearest interactive object of the element
                void set_nearest_interactive_object_on(Map::Object* map, float minimal_distance);

                // Updates the element
                void virtual update();

                // Gets the position of the element on the map
                sf::Vector2f virtual get_map_position(bool virtual_position = false);

                // Gets the new position of the element on the window
                sf::Vector2f get_window_position_on(Map::Object* map);

                // Gets the nearest interactive object of the element
                InteractiveObjects::Object* get_nearest_interactive_object();

                // Gets the type of the element
                Type get_element_type();

                // Verifies if this element is colliding with another element
                bool collides_with(Object* element, Map::Object* map);

                // Gets the sprite of the element
                sf::Sprite virtual get_sprite(Map::Object* map, sf::Color color = sf::Color::White); 

                // Gets the rect that is used for collision with other elements
                sf::RectangleShape virtual get_collision_rect(Map::Object* map, sf::Color color = sf::Color::White);

            protected:
                sf::Vector2f map_position;
                Type element_type;
                InteractiveObjects::Object* nearest_object = nullptr;
        };

        class Environment : public Object{
            public:
                // The constructor of an environment element
                Environment(WorldContent element, sf::Vector2f map_position);

                // Gets the sprite of the element environment
                sf::Sprite get_sprite(Map::Object* map, sf::Color color = sf::Color::White) override;

                // Gets the type of environment element
                WorldContent get_environment_element();

                // Verifies if the entities can move on this environment element
                bool can_move_on();

                // Verifies if this environment is a zombie path
                // - Returns True if yes
                // - Returns False if not
                bool is_zombie_path();

                // Verifies if this environment is a path finding
                // - Returns True if yes
                // - Returns False if not
                bool is_path_finding();

                // Sets if the environment is a zombie path or not
                void set_zombie_path_to(bool zombie_path);

                // Sets if the environment is a zombie path or not
                void set_path_finding_to(bool path_finding);

            protected:
                WorldContent environment_type;
                bool zombie_path = false;
                bool path_finding = false;
        };

        class Animated : public Object{
            public:
                // The default constructor
                Animated();

                // The constructor of an animated element
                Animated(sf::Vector2f map_position, sf::IntRect sprite_sheet_position = sf::IntRect({0, 0}, {32, 32}));

                // Draws the element on the map
                virtual sf::Sprite get_sprite(Map::Object* map, sf::Color color = sf::Color::White) override;

                // Animate the element
                void animate();

                // Sets the animation to the default
                void reset_animation();
                
                // Gets the animation rect
                sf::IntRect get_animation_rect();

            protected:
                sf::IntRect animation_rect;
                sf::IntRect sprite_sheet_position;
                sf::Clock animation_clock;
                bool animating = false;
                int number_of_animation;
        };

        // All things that an animated elements need
        namespace Help{
            const std::map<WorldContent, sf::IntRect> elements_position{
                {soil_rocks_1,      sf::IntRect({0, 0}, {32, 32})},
                {soil_rocks_2,      sf::IntRect({32, 0}, {32, 32})},
                {soil_rocks_3,      sf::IntRect({32*2, 0}, {32, 32})},
                {border_map,        sf::IntRect({32*3, 0}, {32, 32})},
                {spawn_zombie_1,    sf::IntRect({32*4, 0}, {32, 32})},
                {spawn_zombie_2,    sf::IntRect({32*5, 0}, {32, 32})},
                {spawn_zombie_3,    sf::IntRect({32*6, 0}, {32, 32})},
                {walls,             sf::IntRect({32*7, 0}, {32, 32})},
                {nothing,           sf::IntRect({32*8, 0}, {32, 32})},
                {soil_herb_1,       sf::IntRect({32*9, 0}, {32, 32})},
                {soil_herb_2,       sf::IntRect({32*10, 0}, {32, 32})},
                {soil_herb_3,       sf::IntRect({32*11, 0}, {32, 32})},
            };

            // Gets the environment element associated to the color
            WorldContent get_element_at(sf::Color color);

            // gets the laternative version of an element
            WorldContent get_element_alternative(WorldContent element, char i);

            // Gets the nearest MapElement of the given element
            InteractiveObjects::Object* get_nearest_interactive_object_of(MapElement::Object* element, Map::Object* map, float minimal_distance);

            // Gets the color associated to the environment element
            sf::Color get_color_of(WorldContent element);

            // Sort all object by their positions
            void sort_by_positions(std::vector<Object*>& element_list);
            
            // All time value for animating an element
            const std::map<Type, float> animation_time{
                {Type::Entity_being, 0.2},
                {Type::World, 0.2}
            };

            // All animation number for animating an element
            const std::map<Type, int> animation_number{
                {Type::Entity_being, 4},
                {Type::World, 2}
            };
        };

    };
    
    #include "map_manager.hpp"
    #include "../InteractiveObjects/interactive_object.hpp"

#endif