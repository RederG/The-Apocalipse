#ifndef __TOMB__
    #define __TOMB__

    #include "../Others/container.hpp"
    #include "../Map/map_manager.hpp"
    #include "interactive_object.hpp"

    namespace Entity{
        class Object;
    };

    namespace Inventory{
        class Object;
    };

    namespace Container{
        class Object;
    };

    namespace MapElement{
        class Object;
    };

    // All about the tomb
    namespace Tomb{
        // The owner of the tomb
        struct Owner{
            std::string name;
            std::string id;
        };
    
        // The tomb object
        class Object : public InteractiveObjects::Object{
            public:
                // The constructor of a tomb
                Object();

                // Gets the sprite of the tomb
                sf::Sprite get_sprite(Map::Object* map, sf::Color color = sf::Color::White) override;

                // Gets the rect that is used for collision with other elements
                sf::RectangleShape get_collision_rect(Map::Object* map, sf::Color color = sf::Color::White) override;

                // Gets the tomb's position on the map
                sf::Vector2f get_map_position();

                // Gets the owner's name of the tomb
                std::string get_owner_name();

                // Gets the owner's id of the tomb
                std::string get_owner_id();

                // Gets the inventory of the tomb
                Container::Object* get_inventory();

                // Gets the name of the tomb
                std::string get_name();

                // Gets the id of the tomb
                std::string get_id();

                // Sets the position of the tomb on the map
                void set_map_position_to(sf::Vector2f new_position);

                // Sets the new owner's name
                void set_owner_name_to(std::string new_name);

                // Sets the new owner's id
                void set_owner_id_to(std::string new_id);

                // Sets the name of the tomb
                void set_name_to(std::string new_name);

                // Verifies the life time of the tomb (20 seconds)
                /*
                - Returns True if the tomb must disapear (life <= 0)
                - Returns False if not (life > 0)
                */
                bool verify_life_time();

                void save_to(std::string destination);

                void load_from(std::string tomb_file_path);

            private:
                Owner owner;
                std::string name;
                std::string id;
                sf::Clock clock_before_disapear;
                int life = 20;
                Container::Object* inventory = nullptr;
        };

        extern std::vector<Object*> container;

        // Sets the tomb of an entity (if the entity dies)
        void set_tomb_of(Entity::Object &entity);

        //Verifies all tomb's life time
        /*
        - Returns True if there is a tomb with his life <= 0
        - Returns False if not
        */
        bool verify_all_life_time();

        void save_all_to(std::string destination_path, std::string inventories_path, std::string items_path);
        void load_all_from(std::string tombs_files_path, std::string inventories_path, std::string items_path);
    };

#endif