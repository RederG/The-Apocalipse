#ifndef __SET_LANG__
    #define __SET_LANG__

    #include "../main.hpp"
    #include <map>
    #include <string>

    // All languages with its values
    const std::map<int, Main::Languages> lang_value{
        {0, Main::Languages::english},
        {1, Main::Languages::french},
        {2, Main::Languages::malagasy},
    };

    namespace SetLanguageWindow{
        void Render();
        
        void Event();
    };

#endif