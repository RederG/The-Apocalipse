#ifndef __WEAPON__
    #define __WEAPON__

    #include "../item.hpp"

    namespace Item{
        class Object;
    };

    // All weapons
    namespace Weapon {

        // The basic position of all weapons on their sprite sheet
        const std::map<Item::Direction, sf::IntRect> sprite_sheet_position{
            {Item::Direction::left, sf::IntRect({0,64}, {32,32})},
            {Item::Direction::right, sf::IntRect({0,0}, {32,32})},
            {Item::Direction::up, sf::IntRect({0,96}, {32,32})},
            {Item::Direction::down, sf::IntRect({0,32}, {32,32})}
        };

        // A weapon object
        class Object : public Item::Object{
            public:
                // the default constructor
                Object() = default;

                // The BIG constructor
                Object(string weapon_name, int weapon_attack, string weapon_texture_path, string weapon_texture_on_invnetory_path);
            protected:
                // Sets the durability of the weapon
                void set_durability();
        };
    };
    
#endif
