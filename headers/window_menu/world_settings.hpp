#ifndef __WORLD_SETTINGS__
    #define __WORLD_SETTINGS__

    #include "../main.hpp"

    namespace WorldSettingsWindow{
        extern bool window_resized;

        namespace ElementsNames{
            const std::string settings_rect = "main_rect";
            const std::string title = "title_txt";
            const std::string back_button = "back_button";
            const std::string save_button = "save_button";
            const std::string reset_button = "reset_button";
            const std::string terrain_generation_txt = "gen_txt";
            const std::string zone_update_txt = "update_txt";
            const std::string show_keys_txt = "keys_txt";
            const std::string terrain_generation_selector = "gen_select";
            const std::string zone_update_selector = "update_select";
        };

        struct Settings{
            int terrain_generation_size, zone_update_size;
            long long seed;
            bool show_keys = true;
        };

        Settings get_settings_from(std::string player_name);

        void Render();

        void Event();
    };

#endif