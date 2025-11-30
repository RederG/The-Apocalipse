#ifndef __MAIN_MENU__
    #define __MAIN_MENU__

    #include "../main.hpp"
    #include <map>
    #include <string>

    namespace MainMenuWindow{
        void RenderOnThread();

        // The main_menu's events detector and its reactions
        void Event();
    }

#endif

