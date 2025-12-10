#ifndef __ITEM__
    #define __ITEM__

    #include "../main.hpp"
    #include "../Entities/entity.hpp"
    #include "../Help/texture_manager.hpp"
    #include <stack>

    using namespace std;

    namespace Entity{
        class Object;
    };

    namespace Container{
        class Object;
    };

    namespace Map{
        class Object;
    };

    // All Items
    namespace Item{

        // All directions that the item can go
        enum Direction{
            up = 96, down = 32,
            right = 0, left = 64
        };

        // The type of the item
        enum Type{
            simple,
            food,
            weapon,
            guns,
            bullet,
            cartridge,
            effect
        };
        
        // All items' name
        namespace Names{
            
            const std::map<Main::Languages, sf::String> simple{
                {Main::english, "Item"},
                {Main::french, "Item"},
                {Main::malagasy, "Zavatra"}
            };

            const std::map<Main::Languages, sf::String> knife{
                {Main::english, "Knife"},
                {Main::french, "Couteau"},
                {Main::malagasy, "Antsy"}
            };

            const std::map<Main::Languages, sf::String> munition_pistolet{
                {Main::english, "Pistolet's cartridge"},
                {Main::french, "Cartouches de pistolet"},
                {Main::malagasy, "Fitoaran'ny balan'ny basy tsotra"}
            };
        
            const std::map<Main::Languages, sf::String> munition_AK_47{
                {Main::english, "AK47's cartridge"},
                {Main::french, "Cartouche d'un AK47"},
                {Main::malagasy, "Fitoaran'ny balan'ny AK47"}
            };

        };

        // All states of the item
        enum Usability{
            useless, usable
        };

        // All items' extra information
        namespace Extra_info{

            const std::map<Main::Languages, std::string> simple{
                {Main::english, "No information"},
                {Main::french, "Pas d'information"},
                {Main::malagasy, "Tsy misy mampiavaka"},
            };

            const std::map<Main::Languages, std::string> weapon{
                {Main::english, " damages"},
                {Main::french, " degats"},
                {Main::malagasy, " hamafisana"}
            };

            const std::map<Main::Languages, std::string> gun{
                {Main::english, " bullet(s)"},
                {Main::french, " balle(s)"},
                {Main::malagasy, " bala"}
            };

            const std::map<Main::Languages, std::string> munition{
                {Main::english, " munition(s)"},
                {Main::french, " munition(s)"},
                {Main::malagasy, " bala"}
            };

            const std::map<Main::Languages, std::string> munition_number{
                {Main::english, " cartridge(s)"},
                {Main::french, " cartouches(s)"},
                {Main::malagasy, " fitoeran'ny bala"}
            };
        }

        // The object of the item class
        class Object{
            public:
                // The constructor of a Item
                Object();

                // The copier of a Item
                Object(Object *item);

                // Draws the sprite of the Item
                // when an entity handle an item
                void draw_to(sf::RenderWindow &window, Entity::Object &entity);

                // Draws the sprite of the Item
                // when an entity handle an item
                sf::Sprite draw_to(Entity::Object* entity, Map::Object* map, bool draw = true);

                // Draws the attack rect of the item
                virtual void draw_attack_rect(sf::RenderWindow &window);

                // Sets the position of teh item in relation with the direction of the entity
                virtual sf::Vector2f set_position(Entity::Object &entity, sf::Sprite &sprite_to_draw);

                // Adds the item on a specific container in a specific location
                void virtual add_to(Container::Object* container, int location);

                // Adds the item on a specific container
                bool virtual add_to_container(Container::Object* container, bool aleatory = false);

                // Sets the new number of the item
                void set_number_to(int new_number);

                // Gets the identifier of the Item
                string get_id();

                // Gets the name of the Item
                sf::String get_name();

                // Gets the type of the item
                Type get_type();

                // Gets the texture_rect of the item in relation with its direction
                virtual sf::IntRect get_texture_on(Direction direction);

                // Gets the name of the texture of the item
                std::string get_texture_name();

                // Gets the direction of the item
                Direction get_direction();

                // Gets the number of the item
                // - If false : get the number of the item
                // - If True : get the special get of the item
                virtual unsigned int get_number(bool special = false);

                // Gets the max number of the item
                // - If false : get the max number of the item
                // - If True : get the max special get of the item
                virtual unsigned int get_max_number(bool special = false);

                // Gets the clock that checks if we can use the item
                sf::Clock* get_using_timelapse_clock();

                // Gets the name that is used on saving files
                std::string virtual get_file_name();

                // Uses the item ( each has its specificity ) 
                virtual void use_item(Entity::Object &entity, bool special_use = false);

                // Increases the number of the item
                void add(unsigned number_plus);

                // Decreases the number of the item
                void reduce(unsigned number_minus);

                /*
                Verify if the item is stackable or not :
                - Returns True if the item is stackable
                - Returns False if not
                */
                bool is_stackable();

                bool operator==(Object& item);

                // Gets the extra information of the item
                virtual std::string get_extra_info();

                // Gets the state of the item (if it is usable or not)
                Usability get_usability();

                // Reduces the durability
                void reduce_durability_to(int number);

                // Verifies if the item is usable or not
                bool is_usable();
                
                // Gets the durability of the item
                unsigned int get_durability();

                // Gets the maximum durability of the item
                unsigned int get_max_durability();

                // Gets the scale of the item
                // when an entity handle it
                virtual sf::Vector2f get_scale();

                // Gets the name of the item that this item needs
                std::string get_dependance();

                void virtual save_to(std::string destination_path);
                void virtual load_from(std::string file_name_path);

            protected:
                std::map<Main::Languages, sf::String> item_name; // The name of the item
                string id; // The Item's identifier
                int attack; // The attack of the item
                Direction direction; // The Item's direction
                Type type; // The type of the item
                std::map<Direction, sf::IntRect> item_texture;
                std::string texture_name; // the name of the texture of the item on the texture manager
                unsigned int number = 0; // The number of the item
                unsigned int max_number = 0; // The max number of the item
                bool stackable = false; // The stackability of the item
                std::string extra_info; // The extra information of the item
                int durability = 1; // The durability of the item
                unsigned int max_durability = 1; // The maximum durability of the item
                Usability usability = usable; // the usability of the item
                std::map<Main::Languages, sf::String> item_dependance = Item::Names::simple; // The name of the item that this item needs
                std::map<Direction, sf::Vector2f> HandleZone{
                    {right, sf::Vector2f(0, 0)},
                    {left, sf::Vector2f(0, 0)},
                    {up, sf::Vector2f(0, 0)},
                    {down, sf::Vector2f(0, 0)}
                }; // The location of the handle zone of an item
                sf::Clock using_timelapse; // The clock to check if we can use the item
        };

        // All scales for each thing (when an entity handle it)
        // - The bullet is an exception :
        // The entities can not handle it but the guns can summon it 
        const std::map<Item::Type, sf::Vector2f> Scales{
            {Item::Type::simple, sf::Vector2f(2,2)},
            {Item::Type::weapon, sf::Vector2f(1,1)},
            {Item::Type::guns, sf::Vector2f(2,2)},
            {Item::Type::bullet, sf::Vector2f(1,1)},
            {Item::Type::cartridge, sf::Vector2f(1,1)},
            {Item::Type::effect, sf::Vector2f(1,1)},
            {Item::Type::food, sf::Vector2f(1,1)}
        };

        // Inits all things for all items
        void init();

        // Terminates all things for all items
        void terminates();
    };

    #include "../Others/container.hpp"

#endif