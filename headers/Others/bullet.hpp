#ifndef __BULLET__
    #define __BULLET__

    #include "../main.hpp"
    #include "../Items/all_items.hpp"

    namespace Gun{
        enum Nature;
    };
    using namespace Weapon;

    // All about the bullets
    namespace Bullet{

        struct Gun_used{
            std::string gun_name = "", gun_id = "";
            sf::Vector2f gun_pos = sf::Vector2f(0,0);    
        };

        // A bullet
        class Object : public MapElement::Object{
            public:
                // The default constructor
                Object(Gun::Nature type_of_gun);

                // The copier
                Object(Object* bullet);

                // Sets the position of the bullet
                // - Puts the bullet on the middle of the gun
                void set_position(sf::Sprite &gun_sprite);
                
                // Gets the sprite of the bullet
                sf::Sprite get_sprite(Map::Object* map, sf::Color color = sf::Color::White) override;

                // Gets the life of the bullet(a countdown)
                sf::Clock get_living_clock();

                // Gets the direction of the bullet
                Gun::Nature get_gun_nature();

                // Gets the position of the bullet
                sf::Vector2f get_position();

                // Gets the velocity of the bullet
                sf::Vector2f get_velocity();

                // Gets the name and the id of the gun 
                // used by the bullet
                Gun_used get_gun_used();

                // Gets the damage of the bullet
                int get_attack();

            private:
                Gun::Nature gun_type; // The nature/type of the gun
                Gun_used gun_used; // The gun that is used by the bullet
                float velocity_x, velocity_y; // The velocity of the bullet
                sf::Vector2f position; // The position of the bullet
                float rotation; // The position of the bullet
                sf::Clock living_clock; // The life of the bullet(a countdown)
                int attack;
        };

        // All bullets' textures
        const std::map<Gun::Nature, sf::IntRect> bullet_textures{
            {Gun::Nature::pistolet, {sf::IntRect ({0, 0}, {32, 32})}},
            {Gun::Nature::AK_47, {sf::IntRect ({32, 0}, {32, 32})}}
        };

        // All bullets velocity
        const std::map<Gun::Nature, float> velocity{
            {Gun::Nature::pistolet, 0.2},
            {Gun::Nature::AK_47, 0.4}
        };
        
        // All bullets' attack
        const std::map<Gun::Nature, int> all_attacks{
            {Gun::Nature::pistolet, 20},
            {Gun::Nature::AK_47, 30}
        };
        
        // Verifies if one of the bullets must disapear
        void verify_bullets_lives();

        // Creates properly a bullet
        Object* new_bullet(Gun::Nature type_of_gun);

        // Clears the list of bullet
        void clear_bullet_list();

        // Draws all bullets to the window
        void draw_all_bullets_to(Map::Object* map);

        // Moves all bullets on the screen
        void move_all(int fps);

        // Verifies if there is a collision with an entity
        void verify_collision(); // shooter

    };

#endif