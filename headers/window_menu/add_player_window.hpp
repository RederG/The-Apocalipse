#ifndef __ADD_P_MENU__
    #define __ADD_P_MENU__

    #include "../main.hpp"

    namespace AddPlayerWindow{
        extern bool size_event_detected;

        // For all drawing
        void Render();

        // For the add_player_menu's event
        void Event();
    };

#endif