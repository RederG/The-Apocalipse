#ifndef __ENTITY__
    #define __ENTITY__

    #include <string>
    #include <SFML/Graphics.hpp>
    #include <array>
    #include "../main.hpp"
    #include "../Graphicals/life_bar.hpp"
    #include "../Others/container.hpp"
    #include "../Others/probability.hpp"
    #include "../Map/map_manager.hpp"
    #include "../Others/pathfinding.hpp"
    #include "../Effects/effect_manager.hpp"

    namespace Life_Bar{
        class Object;
    };

    namespace Map{
        class Object;
    };

    namespace MapElement{
        class Animated;
    };

    namespace Container{
        class Object;
    };

    class Effect_manager;

    /* 
    All about the entities
    */
    namespace Entity{
        // The state of the entity
        enum State{
            // When he is alive
            Alive,
            // When he is hurting
            Hurting,
            // When he is died
            Died,
            // When he gets his tomb
            Disapeared,
            // When he gets healed
            Healed
        };

        // All differents entity
        enum Type{
            Entity, Player, Zombie
        };
        
        // The different coordinates of an Entity in relation with the window
        enum WindowPosition{
            middle,
            x_middle, 
            y_middle,
            middle_left, 
            middle_right
        };

        // The different direction of an entity
        enum Direction{
            up = 96,
            down = 32,
            right = 0,
            left = 64
        };

        // The different state of animation of the entity
        enum AnimationState{
            // The default animation
            Animated,
            // Animation when the entity don't move
            Default,
            // When the entity is hadling an item
            Handling_item,
            // When the entity is handling a weapon
            Handling_weapon,
            // When the entity is handling a gun
            Handling_gun,
        };

        // All direction that the entity can move (for the zombies)
        struct CanMoveOn {
            bool up = true, down = true, right = true, left = true;
        }; 
        
        // The Entity's class object (a class)
        class Object : public MapElement::Animated{
            public:

                // The default constructor
                Object();

                // This function gets the name of the entity
                std::string get_name();

                // Gets the id of the entity
                std::string get_id();

                // Gets the type of the entity
                Type get_type();
                
                // Gets the inventory of the entity
                Container::Object* get_inventory();

                // Gets the life of the entity
                float get_life();

                // Gets the max life of the entity
                float get_max_life();

                // Gets the direction of the entity
                Direction get_direction();

                // Gets the state of the entity
                State get_state();

                // Gets the life bar of the entity
                Life_Bar::Object* get_life_bar();

                // gets the clock when the entity is hurt
                sf::Clock* get_hurting_time_clock();

                // Gets the movement state of the entity
                bool is_moving();

                // Gets the sprite of the entity
                sf::Sprite virtual get_sprite(Map::Object* map, sf::Color color = sf::Color::White) override;

                // Gets the sprite for verifying collisions on the map
                sf::RectangleShape get_collision_rect(Map::Object* map, sf::Color color = sf::Color::White) override;

                // Gets the location ( with the scale ) of the hand of the entity
                sf::Vector2f get_hand_location(Map::Object* map);

                // Gets the effect manager of the entity
                Effect_manager* get_effect_manager();

                // Verifies if the entity is colliding with the environment
                // - Returns True if one of the environment's element's can_move_on = false
                // - Returns False if not
                bool is_colliding_with_environment();

                // Verifies if the entity is colliding with one of the entity
                // - Returns True if this entity is colliding with one of the entities
                // - Returns False if not
                bool is_colliding_with_other_entities();

                // Verifies if the entity is alive or not
                bool is_alive();

                // this function switches animation in relation with the handling item
                void set_animation_by_handling_item();

                // Draws the entity on the map
                void draw_to(Map::Object* map, sf::Color color = sf::Color::White) override;

                // This function draws the entity with his item
                void draw_with_item(Map::Object* map, sf::Color color = sf::Color::White);

                // This function sets the name of the entity
                void set_name(std::string name);

                // This function sets a new direction for the entity
                void set_direction_to(Direction new_direction);

                // Sets the state of the entity
                void set_state_to(State new_state);
                
                // Sets the movement state of the entity
                void set_movement_to(bool movement_state);

                // This function switches the animation
                void switch_animation_to(AnimationState new_animation_state);

                // This function allows the entity to move
                void virtual move(){};

                // Moves the entity on the map
                void move_on_map(float x, float y) override;

                // This function appears when the entity is attacked
                void get_attacked(int attack);

                // Heal the entity with a certain value
                void get_healed(float value);

                // Fills the inventory of the entity
                void virtual fill_inventory();

                // Destroys all different object of the entity
                void virtual destroy();

                // Updates the entity
                void update() override;

                // Gets the satiation value of the entity
                float get_satiation();

                // Gets the max satiation value of the entity
                float get_max_satiation();

                // Gets the clock that reduces the satiation of the entity
                sf::Clock get_satiation_clock();

                // Satiate the entity
                void get_satiated(float satiation_value);

                // Make the entity hungry
                void get_hungry(float hungry_value);

                // Saves the entity
                // - The " location_path " must ends with the " Entities " directory
                // with no " / "
                void virtual save_to(std::string location_path);

                // Loads the entity
                // - The " location_path " must ends with the " . entity " extension
                void virtual load_from(std::string location_path);

            protected:
                std::string name = "entity"; // The name of the entity
                std::string id; // The id of the entity
                float life = 100; // The life of the entity
                float max_life = 100;
                bool moving = true; // If the entity is moving or not
                State state; // The state of the entity
                float velocity; // The velocity of the entity on the map (map's case unit)
                Type type = Entity; // The type of the entity
                Direction direction; // The direction of the entity
                CanMoveOn direction_allowed; // All directions that the entity can move
                sf::Clock clock_hurting; // The clock when the entity is hurting
                sf::Clock clock_healing; // The clock when the entity is healing
                sf::Clock clock_moving;  // The clock for the entity's movement
                AnimationState animation_state; // The animation state of the entity
                float satiation = 100;
                float max_satiation = 100;
                float satiation_diminution_lapse_time = 5;
                float satiation_diminution_lapse_time_original = 5;
                sf::Clock satiation_clock;
                Container::Object* inventory = new Container::Object(5); // The inventory of the entity
                Life_Bar::Object* life_bar; // The life bar of the entity
                Effect_manager* effect_manager = new Effect_manager(); // the effect manager of the entity
        };

        // All velocities for each entity
        const std::map<Type, float> Velocity{
            {Entity, 0.1},
            {Player, 0.1},
            {Zombie, 0.05}
        };
        
        //This list contains every Entity on the map
        extern std::list<Object*> container;

        // The scale of all entities on the map
        const sf::Vector2f ENTITY_SCALE = sf::Vector2f(2, 2);

        // Inits all elements needed for entities
        void init();
        
        // This function removes all entities
        void terminates();

        // Draws all entities with their item
        void draw_all_to(Map::Object* map);

        // Verifies if the entity is alive or not
        // - Return True if there is a dead entity
        // - Return False if not
        bool verify_entity();

        // Sets a temporary entity
        void set_temporary_entity(Object* entity);

        // Gets the temporary entity
        Object* get_temporary_entity();

        // Reset / removes the temporary entity
        void reset_temporary_entity();

        // Sets the nearest interactive objects of each entity
        void set_all_nearest_interactive_objects();

        // Sets the map for all entities
        void set_map_to(Map::Object* map);

        // Gets the map of all entities
        Map::Object* get_map();

        // Updates all entities
        void update_all();

        // Put all entities' movement variable to a specific value
        void set_entities_movement_to(bool movement_allowed);

        // Saves all entities
        // - The " location_path " must ends with the " Entities " directory
        // with no " / "
        // - The " inventories_path " must ends with the " Inventories " directory
        // with no " / "
        // - The " items_path " must ends with the " Items " directory
        // with no " / "
        // - The " effects_path " must ends with the " Effects " directory
        // with no " / "
        void save_all_to(std::string location_path, std::string inventories_path, std::string items_path, std::string effect_managers_path, std::string effects_path);

        // Loads all entities
        // - The " location_path " must ends with the " Entities " directory
        // with no " / "
        // - The " inventories_path " must ends with the " Inventories " directory
        // with no " / "
        // - The " items_path " must ends with the " Items " directory
        // with no " / "
        // - The " effects_path " must ends with the " Effects " directory
        // with no " / "
        void load_all_from(std::string location_path, std::string inventories_path, std::string items_path, std::string effect_managers_path, std::string effects_path);
    };
#endif