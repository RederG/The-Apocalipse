#ifndef __CARTRIDGE__
    #define __CARTRIDGE__

    #include "../item.hpp"

    // All bullets
    namespace Cartridge{

        // All types of cartridge
        enum Type{
            pistolet, AK_47
        };

        // All cartridges' ammos' damages
        const std::map<Type, int> damage{
            {pistolet, 20},
            {AK_47, 30}
        };

        // All cartridges' names
        namespace Names{
            const std::map<Main::Languages, sf::String> cartridge_pistolet{
                {Main::english, "Pistolet's cartridge"},
                {Main::french, "Cartouches de pistolet"},
                {Main::malagasy, "Fitoeran'ny balan'ny basy tsotra"}
            };
        
            const std::map<Main::Languages, sf::String> cartridge_AK47{
                {Main::english, "AK47's cartridge"},
                {Main::french, "Cartouche d'un AK47"},
                {Main::malagasy, "Fitoeran'ny balan'ny AK47"}
            };
        }

        // All cartridge' textures
        const std::map<Type, std::map<Item::Direction, sf::IntRect>> textures{
            {pistolet, {  
                            {Item::Direction::left, sf::IntRect({0, 0}, {32, 32})},
                            {Item::Direction::right, sf::IntRect({0, 0}, {32, 32})},
                            {Item::Direction::up, sf::IntRect({0, 0}, {32, 32})},
                            {Item::Direction::down, sf::IntRect({0, 0}, {32, 32})}
                        }
            },
            
            {AK_47, {   
                        {Item::Direction::left, sf::IntRect({32, 0}, {32, 32})},
                        {Item::Direction::right, sf::IntRect({32, 0}, {32, 32})},
                        {Item::Direction::up, sf::IntRect({32, 0}, {32, 32})},
                        {Item::Direction::down, sf::IntRect({32, 0}, {32, 32})}
                    }
            }
        };
        
        // All munitions' name
        const std::map<Type, std::map<Main::Languages, sf::String>> names{
            {pistolet, Names::cartridge_pistolet},
            {AK_47, Names::cartridge_AK47}
        };

        // All munitions' max numbers
        const std::map<Type, unsigned int> max_number{
            {pistolet, 10},
            {AK_47, 20}
        };

        // All munitions' max ammo
        const std::map<Type, unsigned int> max_ammo{
            {pistolet, 10},
            {AK_47, 30}
        };

        // A munition
        class Object : public Item::Object{
            using Item::Object::add_to;
            public:
                // Creates a munition
                Object(Type type_chosen);
                Object(Object& obj);
                Object(Object* ptr);

                // Gets the type of the bullet
                Type get_cartridge_type();

                // Returns the extra info of the munition
                std::string get_extra_info() override;

                // Gets the number of the ammo
                int get_number_ammo();

                // Recharge the gun associated to the munition
                void use_item(Entity::Object &entity, bool speacial_use = false) override;

                // Change the number of ammo to the next
                void next_ammo();

                // Add the bullet on a specific container
                void add_to(Container::Object* container, int location) override;

                // - If false : gets the number of item
                // - If true : gets the number of ammo on the munition
                unsigned int get_number(bool special = false) override;

                // - If false : gets the max number of item
                // - If true : gets the max number of ammo on the munition
                unsigned int get_max_number(bool special = false) override;

                // Gets the maximum value of the ammo
                int get_max_ammo();

                std::string get_file_name() override;

                // Adds the munition on a specific container
                bool add_to_container(Container::Object* container, bool aleatory = false) override;

                void save_to(std::string destination_path) override;
                void load_from(std::string item_file_path) override;
                
            private:
                Type cartridge_type; // The type of the munition
                int max_ammo = 1;

                std::stack<int> all_ammo;
                
                // Sets aleatory the number of munitions
                void set_number();

                // Sets the number of the ammo on the munition
                void set_number_ammo();

                // Sets the position of the munition 
                // when an entity handle it
                sf::Vector2f set_position(Entity::Object &entity, sf::Sprite &sprite_to_draw) override;
        };

        // Sets a temporary munition
        void set_temporary_cartridge(Object* cartridge);

        // Gets the temporary munition
        Object* get_temporary_cartridge();

        // Resets the temporary munition
        void reset_temporary_cartridge();

        //Creates correctly a new munition
        Object* create_new(Type type_chosen);

        // Gets a specific munition on the list 
        //(All the munitions on the list are correctly created by
        //the "create_new" function)
        std::shared_ptr<Object> get_cartridge(std::string name, std::string id);

        // Clears the list of munitions
        void clear();
    }


#endif