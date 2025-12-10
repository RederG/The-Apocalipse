#ifndef __PLAYER__
    #define __PLAYER__

    #include "entity.hpp"
    #include "../Items/item.hpp"
    #include "../InteractiveObjects/tomb.hpp"

    // The player class
    namespace Player{

        // The state of the player
        enum State{
            // When he is playing
            Playing,
            // When he is looking on his inventory
            Look_at_Inventory,
            // When he is interacting with another object
            Interacting_with_objects
        };
        
        // A player Object
        class Object : public Entity::Object {
            public:
                // The constructor of a player
                Object(std::string player_name);

                // This function allows the player to move
                void move() override;
                
                // Sets the state of the player
                void set_state(State new_state);

                // Gets the state of the player
                Player::State get_state();

                // Gets the sprite of the player
                sf::Sprite get_sprite(Map::Object* map, sf::Color color = sf::Color::White) override;

                // Locates the nearest tomb around the player
                Tomb::Object * get_nearest_tomb(int minimal_distance);

                // Add an item from another inventory
                void take_item_of(Tomb::Object &tomb);

                void fill_inventory() override;
                void update() override;
                void save_to(std::string location_path) override;
                void load_from(std::string location_path) override;

            private:
                State state; // The state of the player
        };

        // Contains every player on the game
        extern std::list<Object*> container;

        // This is the funcion that creates player
        void create_player(std::string player_name, Map::Object* map);

        // This function allows all player to move
        void move(int fps);

        // Sets the state of all players
        void set_state(State new_state);

        // Gets the state of all players
        void get_state();
    };

#endif