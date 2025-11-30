#include "../main.hpp"
#include <map>
#include <string>
#include "set_lang_window.hpp"

namespace SetLanguageWindow{
    sf::Font Arial;

    sf::Text Title(Arial);
    sf::Text Back(Arial);

    sf::RectangleShape Set_lang_rect(sf::Vector2f(720,480));

    sf::RectangleShape line;

    std::ofstream file;

    std::list<sf::Text> all_languages_txt;
    
    std::map<Main::Languages, std::list<sf::String>> all_languages{
        {Main::Languages::english, {L"English", L"French", L"Malagasy"}},
        {Main::Languages::french, {L"Anglais", L"Français", L"Malagasy"}},
        {Main::Languages::malagasy, {L"Englisy", L"Frantsay", L"Malagasy"}}
    };

    void Render(){
        Main::window()->clear(sf::Color(0,0,0));

        Main::window()->draw(*Main::background());
        Main::window()->draw(Set_lang_rect);
        Main::window()->draw(Title);
        Main::window()->draw(Back);
        for (sf::Text txt : all_languages_txt){
            if (txt.getGlobalBounds().contains({float(sf::Mouse::getPosition(*Main::window()).x), float(sf::Mouse::getPosition(*Main::window()).y)}))
                txt.setFillColor(sf::Color(100,100,255));
            else
                txt.setFillColor(sf::Color(255,255,255));
            Main::window()->draw(txt);
        }
        Main::window()->draw(line);

        Main::window()->display();
    }

    void Set_content_position(){
        int x = Main::window()->getSize().x/2 - Set_lang_rect.getSize().x/2;
        Set_lang_rect.setPosition(sf::Vector2f(x, 100));

        x = Main::window()->getSize().x/2 - Title.getGlobalBounds().size.x/2;
        Title.setPosition(sf::Vector2f(x, Set_lang_rect.getPosition().y+30));
        Back.setPosition(sf::Vector2f(Set_lang_rect.getPosition().x, Set_lang_rect.getPosition().y));

        int y = Title.getPosition().y + Title.getGlobalBounds().size.y + 30;
        line.setPosition(sf::Vector2f(Set_lang_rect.getGlobalBounds().position.x, y));
    }

    void Update_content(){
        if(Back.getGlobalBounds().contains({float(sf::Mouse::getPosition(*Main::window()).x), float(sf::Mouse::getPosition(*Main::window()).y)}))
            Back.setFillColor(sf::Color(100,100,100));
        else 
            Back.setFillColor(sf::Color(255,255,255));
    }

    void Input(){
        while(std::optional event = Main::window()->pollEvent()){
            if(Main::check_resize_events(event))
                Main::first_drawing(true);
            
            if(event->is<sf::Event::Closed>())
                Main::window()->close();

            if(event->is<sf::Event::MouseButtonPressed>()){
                if(Back.getGlobalBounds().contains({float(sf::Mouse::getPosition(*Main::window()).x), float(sf::Mouse::getPosition(*Main::window()).y)})){
                    Main::context(Main::Context::MainMenu);
                    Main::first_drawing(true);
                    Debugging::write("Button : BACK", Debugging::get_state_of(Debugging::Button));
                }
                
                for(const sf::Text txt : all_languages_txt){
                    if(txt.getGlobalBounds().contains({float(sf::Mouse::getPosition(*Main::window()).x), float(sf::Mouse::getPosition(*Main::window()).y)})){
                        int i = 0;
                        for (const std::string str : all_languages.at(Main::lang())){
                            if(txt.getString() == str){
                                Main::lang(lang_value.at(i));
                                file.open("../res/lang.txt");
                                file << static_cast<int>(lang_value.at(i));
                                file.close();
                                Main::first_drawing(true);
                                Debugging::write("LANGUAGE SETS TO " + str, Debugging::get_state_of(Debugging::Button));
                            }
                            else
                                i++;
                        }
                    }
                }
            }
        }
    }

    void Event(){
        Main::window()->setFramerateLimit(60);

        float Color_alpha;

        std::map<Main::Languages, sf::String> title_lang{
            {Main::Languages::english, L"CHOOSE YOUR LANGUAGE"},
            {Main::Languages::french, L"PRENEZ VOTRE LANGUAGE"},
            {Main::Languages::malagasy, L"SAFIDIO NY FITENINAO"}
        };

        std::map<Main::Languages, sf::String> back_lang{
            {Main::Languages::english, L"< BACK"},
            {Main::Languages::french, L"< RETOUR"},
            {Main::Languages::malagasy, L"< MIVERINA"}
        };

        Arial = *FontManager::get_font(FontManager::Arial);

        Title.setFont(Arial);
        Title.setCharacterSize(50);
        Title.setString(title_lang.at(Main::lang()));

        Back.setFont(Arial);
        Back.setCharacterSize(30);
        Back.setString(back_lang.at(Main::lang()));

        Set_lang_rect.setFillColor(sf::Color(0,0,0,100));

        Title.setFillColor(sf::Color(255,100,0));
        Back.setFillColor(sf::Color(255,255,255));

        line.setSize(sf::Vector2f(Set_lang_rect.getGlobalBounds().size.x, 4));
        line.setFillColor(sf::Color(0,0,0,150));

        Set_content_position();
        
        if(Main::first_drawing()){
            Debugging::write("----SETTING LANGUAGE-----\n", Debugging::get_state_of(Debugging::Description));

            all_languages_txt.clear();
            int x = 0, y = Title.getPosition().y + Title.getGlobalBounds().size.y + 30;
            for(const std::string txt : all_languages.at(Main::lang())){
                sf::Text text(Arial, txt, 30);
                x = Main::window()->getSize().x/2 - text.getGlobalBounds().size.x/2;
                text.setPosition(sf::Vector2f(x, y + 10));
                y += 50;
                text.setFillColor(sf::Color(255,255,255));
                all_languages_txt.push_back(text);
            }

            Main::first_drawing(false);
        }

        Input();
        Update_content();
        Render();
    }

};
