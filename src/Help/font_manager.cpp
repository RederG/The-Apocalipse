#include "../../headers/Help/font_manager.hpp"

namespace FontManager{

    std::map<Fonts, std::shared_ptr<sf::Font>> all_fonts; 

    void init_fonts(){
        Debugging::write("LOADING ALL FONTS.........", Debugging::get_state_of(Debugging::Description));
        sf::Font Arial;

        if(Arial.openFromFile("../res/Fonts/Arial_regular.ttf"))
            Debugging::write("Arial font loaded\n", Debugging::get_state_of(Debugging::Description));

        all_fonts[Fonts::Arial] = std::make_shared<sf::Font>(Arial);
    }

    void destroy_all(){
        all_fonts.clear();
        Debugging::write("FONTS DESTROYED", Debugging::get_state_of(Debugging::Description));
    }

    sf::Font* get_font(Fonts font_chosen){
        return &*all_fonts[font_chosen];
    }
}