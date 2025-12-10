#ifndef __PLAY_MENU__
    #define __PLAY_MENU__

    #include "../main.hpp"

    // The chosen player
    extern std::string player_choosed;

    namespace PlayWindow{
        void Render();

        /*
        The function which controlles all the events
        */
        void Event();
    };

#endif

