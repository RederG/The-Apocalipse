#ifndef __TEXTURE_MANAGER__
    #define __TEXTURE_MANAGER__
    
    #include "../main.hpp"

    // All textures for the game
    namespace AllTextures{

        enum KeysCoords{
            I, C, D, T, Arrows, Space, I_quit, C_quit, Arrows_inventory, P
        };

        enum EntityMode{
            player_handling_item, player, player_handling_weapon, player_handling_gun,
            zombie
        };

        // Initializes all textures
        void init();

        // Destroys all textures
        void destroy();

        // Gets a specific texture
        sf::Texture* get(std::string name);

        // Gets the texture of a structure
        sf::Texture* get_structure(std::pair<int, int> point);

        // Sets the texture of a structure from an image
        void set_structure(std::pair<int, int> point, sf::Image image);
    };

#endif