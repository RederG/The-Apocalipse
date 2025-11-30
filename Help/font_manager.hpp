#ifndef __FONT_MANAGER__
    #define __FONT_MANAGER__

    #include "../main.hpp"

    // It manages all fonts from the game
    namespace FontManager
    {
        // All fonts for the game
        enum Fonts{
            Arial
        };
        
        // Initializes all fonts
        void init_fonts();

        // Destroys all fonts
        void destroy_all();

        // Gets a specifical font
        sf::Font* get_font(Fonts font_chosen);
    }
    
#endif