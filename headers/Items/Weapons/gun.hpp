#ifndef __GUN__
    #define __GUN__

    #include "weapon.hpp"
    #include "cartridge.hpp"

    // All guns on the game
    namespace Gun{
        // The nature of gun
        enum Nature{
            pistolet, AK_47
        };

        // A gun
        class Object : public Weapon::Object{
            public:
                // The constructor
                Object(Nature type_of_gun);

                Object(Object* obj);

                Object(Item::Object& obj);

                // Gets the texture of the gun in relation with the fire mode
                sf::IntRect get_texture_on(Item::Direction direction) override;

                // When we use a gun (The entity parameter is useless)
                void use_item(Entity::Object &entity, bool special_use = false) override;

                // Add the gun on a specific container
                void add_to(Container::Object* container, int location) override;

                // Gets the extra info of a gun
                std::string get_extra_info() override;

                // Sets the position of the gun (When en entity handle it)
                sf::Vector2f set_position(Entity::Object &entity, sf::Sprite &sprite_to_draw) override;

                // Gets the scale of the gun
                sf::Vector2f get_scale() override;

                std::string get_file_name() override;

                /*
                Recharge the gun with its munition :
                - returns True if the gun is recharged correctly
                - returns False if not
                */ 
                bool recharge_with(Cartridge::Object* cartridge);

                /*
                Allows to the gun to fire
                */
                void fire();

                /*
                Verifies the fire mode clock if it should be disabled
                */
                void verify_fire_mode_clock();

                void save_to(std::string destination_path) override;
                void load_from(std::string item_file_path) override;

            protected:
                Gun::Nature gun_type;
                int ammo = 0;
                int max_ammo = 1;
                bool fire_mode = false;
                bool can_fire = true;
                sf::Clock can_fire_clock;
                sf::Clock fire_mode_clock;
                std::map<Item::Direction, sf::IntRect> fire_mode_texture_rect;

                // Sets the ammo of the gun
                void set_ammo();

                // Sets the ammo's number of the gun to the new_number_ammo
                void set_ammo(int new_number_ammo);
        };
    
        // All guns' time(seconds) pause after fire
        const std::map<Nature, float> time_pause_after_fire{
            {pistolet, 0.4},
            {AK_47, 0.1}
        };
        
        // All guns' name
        const std::map<Nature, std::map<Main::Languages, sf::String> > names{
            {pistolet, {
                {Main::english, "Pistolet"},
                {Main::french, "Pistolet"},
                {Main::malagasy, "Basy tsotra"},
            }},
            
            {AK_47, {
                {Main::english, "AK47"},
                {Main::french, "AK47"},
                {Main::malagasy, "AK47"},
            }}
        };

        // All guns' durability
        const std::map<Nature, int> durabilities{
            {pistolet, 100},
            {AK_47, 300}
        }; 

        // All guns' textures
        const std::map<Nature, std::map<Item::Direction, sf::IntRect>> textures{
            {pistolet, {  
                {Item::Direction::left, sf::IntRect ({32, 64}, {32, 32})},
                {Item::Direction::right, sf::IntRect({32, 0}, {32, 32})},
                {Item::Direction::up, sf::IntRect   ({32, 96}, {32, 32})},
                {Item::Direction::down, sf::IntRect ({32, 32}, {32, 32})}
            }},

            {AK_47, {   
                {Item::Direction::left, sf::IntRect ({64, 64}, {32, 32})},
                {Item::Direction::right, sf::IntRect({64, 0}, {32, 32})},
                {Item::Direction::up, sf::IntRect   ({64, 96}, {32, 32})},
                {Item::Direction::down, sf::IntRect ({64, 32}, {32, 32})}
            }}
        };

        // All guns' textures' name
        const std::map<Nature, std::string> textures_name{
            {pistolet, "pistolet"},
            {AK_47, "AK47"}  
        };

        // All guns' textures in fire mode
        const std::map<Item::Direction, sf::IntRect> textures_fire_mode{
            {Item::Direction::left, sf::IntRect({32,64}, {32,32})},      
            {Item::Direction::right, sf::IntRect({32,0}, {32,32})},      
            {Item::Direction::up, sf::IntRect({32,96}, {32,32})},      
            {Item::Direction::down, sf::IntRect({32,32}, {32,32})}
        };
    
        // All guns' ammo name
        const std::map<Nature, std::map<Main::Languages, sf::String>> ammo_name{
            {pistolet, {Item::Names::munition_pistolet}},
            {AK_47, {Item::Names::munition_AK_47}}
        };
    
        // All guns' max ammo
        const std::map<Nature, int> Max_ammo{
            {pistolet, 10},
            {AK_47, 100}
        };

        // All scales for each item (specific)
        const std::map<Nature, sf::Vector2f> Scales{
            {AK_47, sf::Vector2f(2,2)},
            {pistolet, sf::Vector2f(1.2, 1.2)}
        };

        // All localisation of the handling zone
        const std::map<Nature, std::map<Item::Direction, sf::Vector2f>> HandlingZone{
            {pistolet, {
                {Item::Direction::left, sf::Vector2f(26, 21)},
                {Item::Direction::right, sf::Vector2f(8, 22)},
                {Item::Direction::up, sf::Vector2f(16, 28)},
                {Item::Direction::down, sf::Vector2f(16, 11)}
            }},
            
            {AK_47, {
                {Item::Direction::left, sf::Vector2f(22, 18)},
                {Item::Direction::right, sf::Vector2f(10, 16)},
                {Item::Direction::up, sf::Vector2f(16, 22)},
                {Item::Direction::down, sf::Vector2f(16, 12)}
            }}
        };

        // All localisation of the handling zone
        const std::map<Nature, std::map<Item::Direction, sf::Vector2f>> CanonZone{
            {pistolet, {
                {Item::Direction::left, sf::Vector2f(8, 14)},
                {Item::Direction::right, sf::Vector2f(26, 14)},
                {Item::Direction::up, sf::Vector2f(16, 8)},
                {Item::Direction::down, sf::Vector2f(26, 16)}
            }},

            {AK_47, {
                {Item::Direction::left, sf::Vector2f(22, 18)},
                {Item::Direction::right, sf::Vector2f(4, 14)},
                {Item::Direction::up, sf::Vector2f(16, 6)},
                {Item::Direction::down, sf::Vector2f(16, 26)}
            }}
        };

        // List of all guns
        extern std::list<std::shared_ptr<Object>> all_guns;
        
        // Clears the list of gun
        void clear_list();

        // Verifis all guns if they are usable :
        // - If yes, they continue to exist
        // - if not, they will be remove from the list
        void verify_guns_usability();

        /*
        Verifies all fire mode clock's of the gun
        */
        void verify_all_fire_mode_clocks();

        // Sets a temporary gun
        void set_temporary_gun(Object* gun);

        // Gets the temporary gun
        Object* get_temporary_gun();

        // Deletes / resets the temporary gun
        void reset_temporary_gun();
    }


#endif