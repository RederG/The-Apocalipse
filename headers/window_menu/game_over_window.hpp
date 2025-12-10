#ifndef __GAME_OVER__
    #define __GAME_OVER__
    
    #include "game_window.hpp"

    namespace GameOverWindow{
        // Save the progress of the player
        void save_progress(bool dead = true); 

        void Render();

        void Event();
    }
#endif