#ifndef __PLAYER_DATA_MENU__
    #define __PLAYER_DATA_MENU__

    #include "../main.hpp"
    #include "play_window.hpp"

    namespace PlayerDataWindow{
        struct Data{
            int score = 0, best_score = 0, time = 0;
            bool dead, first_play;
            long long world_seed;
        };

        Data get_player_data(std::string player);

        void Render();

        void Event();
    }

#endif