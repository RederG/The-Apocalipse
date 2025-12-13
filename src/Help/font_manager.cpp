#include "../../headers/Help/font_manager.hpp"

namespace FontManager{

    std::map<Fonts, sf::Font*> all_fonts; 

    void init_fonts(){
        Debugging::write("LOADING ALL FONTS.........", Debugging::get_state_of(Debugging::Description));
        all_fonts[Fonts::Arial] = new sf::Font();
        
        if(all_fonts[Fonts::Arial]->openFromFile("../res/Fonts/Arial_regular.ttf"))
            Debugging::write("Arial font loaded\n", Debugging::get_state_of(Debugging::Description));

    }

    void destroy_all(){
        for(auto& font : all_fonts){
            delete font.second;
            font.second = nullptr;
        }
        all_fonts.clear();
        Debugging::write("FONTS DESTROYED", Debugging::get_state_of(Debugging::Description));
    }

    sf::Font* get_font(Fonts font_chosen){
        return &*all_fonts[font_chosen];
    }
}