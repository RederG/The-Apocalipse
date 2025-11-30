#ifndef __MAIN__
    #define __MAIN__

    #include <SFML/Graphics.hpp>
    #include <iostream>
    #include "Classes/Others/Collision.hpp"
    #include <fstream>
    #include <filesystem>
    #include <list>
    #include <vector>
    #include <stack>
    #include <map>
    #include <unordered_map>
    #include <algorithm>
    #include <random>
    #include <thread>
    #include <mutex>
    #include "Help/font_manager.hpp"
    #include "Help/debugging.hpp"
    #include "Help/contain_manager.hpp"

    namespace  Main{
        // All different languages
        enum Languages{
            english,
            french,
            malagasy
        };

        // All Game's menu 
        enum Context{
            MainMenu,
            SetLangMenu,
            PlayMenu,
            AddPlayer,
            PlayerData,
            WorldSettings,
            TheGame,
            GamePauseMenu,
            GameOver
        };

        // For the clocks if we reached
        // the time or not
        enum GameTimer : bool{
            Reached = true,
            NotReached = false
        };

        // The max FPS
        const int MAX_FPS = 1200;

        // The name of the game
        const std::string game_name = "THE APOCALIPSE";

        // Gets the main window
        sf::RenderWindow* window();

        // Gets tha background
        sf::Sprite* background();

        // Gets the main language
        Languages lang();

        // Gets the main context
        Context context();

        // Gets the main content manager
        ContainManager::Object* context_content();

        // Gets the first drawing
        bool first_drawing();

        // Sets the main language
        void lang(Languages new_lang);

        // Sets the main context
        void context(Context new_context);

        // Sets the first drawing
        void first_drawing(bool activation);

        // The clock for seconds
        GameTimer clock_seconds(float seconds, sf::Clock &clock_to_compare, bool reset_timer = true);

        // The clock for MilliSeconds
        GameTimer clock_millisec(int milliSec, sf::Clock &clock_to_compare, bool reset_timer = true);

        // the Clock for MicroSeconds
        GameTimer clock_microsec(int microSec, sf::Clock &clock_to_compare, bool reset_timer = true);

        /* 
        Get all the files in one directory
        Note : The 'directory_path' must ends with a " / " 
        */
        void get_files_in(std::string directory_path, std::list<std::string> &to_file_list);

        /* 
        Get all the files'path in one directory
        Note : The 'directory_path' must ends with a " / " 
        */
        void get_files_path_in(std::string directory_path, std::list<std::string> &file_path_list);

        /*
        This function transform Seconds into minutes
        */
        void sec_conversion(int seconds, std::string &result);

        /*
        This function calculate the FPS (Frame Per Seconds)
        */
        void get_FPS(sf::Clock &FPS_clock, sf::Clock &FPS_clock_update, float &the_FPS);

        // Gets the image portion of a texture
        sf::Image get_image_from(sf::IntRect rect, sf::Texture texture);

        // Gets the seed based on x, y and the given seed
        long long get_seed(long long x, long long y, long long seed);

        // Gets the index of a point in an area base size
        long long get_index(long long x, long long y, long long size_x, long long size_y);

        // Returns a random value
        long long random_value(int min = 0, long long max = 4294967295);

        // Gets the absolute value of a number
        float absolute(float number);

        // Checks all resize events
        bool check_resize_events(std::optional<sf::Event> event);

        // Converts a std::pair to a sf::Vector2f
        sf::Vector2f get_vector2f_of(std::pair<float, float> point);

        // Converts a std::pair to a sf::Vector2f
        sf::Vector2i get_vector2i_of(std::pair<float, float> point);

        // Converts a std::pair to a sf::Vector2f
        sf::Vector2i get_vector2i_of(sf::Vector2f point);

        // Converts a sf::Vector2f to a std::pair<float, float>
        std::pair<float, float> get_pair2f_of(sf::Vector2f point);

        // Converts a sf::Vector2f to a std::pair<int, int>
        std::pair<int, int> get_pair2i_of(sf::Vector2f point);

        // Converts a sf::Vector2i to a std::pair<int, int>
        std::pair<int, int> get_pair2i_of(sf::Vector2i point);

        // Gets the delta time of the game
        float delta_time();
    };

#endif