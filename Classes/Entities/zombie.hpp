#ifndef __ZOMBIE__
    #define __ZOMBIE__

    #include "entity.hpp"

    // All things for the zombie class
    namespace Zombie{

        // The zombie object (a class)
        class Object : public Entity::Object{
            using Entity::Object::Object;

            public:
                // This is the constructor of a zombie
                Object();

                // Draws the zombie on the map
                sf::Sprite get_sprite(Map::Object* map, sf::Color color = sf::Color::White) override;

                // Moves the zombie
                void move() override;

                // This function determines where is the player
                void locates_player();

                // Put the zombie on a random position
                void random_pos(Map::Object* map);

                // Fills the zombie's inventory
                void fill_inventory() override;

                // Allows to the zombie to  attack
                virtual void attack();

                // Destroys every object on the zombie
                void destroy() override;

                // Updates the zombie
                void update() override;

                void save_to(std::string location_path) override;
                void load_from(std::string location_path) override;
                
            protected:
                PathFinding::A_star* A_star_path_finder = new PathFinding::A_star();
                std::vector<sf::Vector2f> path_to_something;

                int attack_value = 0;
                sf::Clock attack_timer;
        };
        
        //This list contains every Zombies on the map
        extern std::list< std::shared_ptr<Object> > container;

        /*
        This function creates multiple zombies.
        It puts all zombies in a random position on the window 
        */
        void create_multiple(unsigned int number_of_zombies, Map::Object* map);

        // Updates all zombies
        void everyone_search_player();

        // Verifies if the zombies are searching
        bool are_searching();
    };

#endif