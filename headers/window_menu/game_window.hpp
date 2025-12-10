#ifndef __GAME__
    #define __GAME__

    #include "../Help/the_apocalipse.hpp"
    #include "play_window.hpp"
    #include "player_data_window.hpp"

    namespace GameWindow{
        extern int time;

        extern int zombie_killed;

        extern sf::Clock fps_clock;
        extern float fps;
        extern sf::Clock FPS_update;

        extern sf::Vector2f player_position;

        // Draws all of the elements of the game
        void draw(bool pause_animation = false);

        void Render();

        void UpdateOnThread();

        void Event();
    }

#endif