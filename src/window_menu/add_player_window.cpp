#include "../../headers/main.hpp"
#include "../../headers/window_menu/add_player_window.hpp"

namespace AddPlayerWindow{

    // The writing_bar's clock for its animation
    sf::Clock writing_bar_clock;

    // All boolean values
    bool writing = false;
    bool valid = false;
    bool verif = true;
    bool bar_transparent = true;
    bool size_event_detected = false;

    float x_middle;
    float y_middle;

    // All unauthaurised characters
    std::vector<sf::Keyboard::Key> special_char = {
        sf::Keyboard::Key::Delete,
        sf::Keyboard::Key::Backspace,
        sf::Keyboard::Key::F1,
        sf::Keyboard::Key::F2,
        sf::Keyboard::Key::F3,
        sf::Keyboard::Key::F4,
        sf::Keyboard::Key::F5,
        sf::Keyboard::Key::F6,
        sf::Keyboard::Key::F7,
        sf::Keyboard::Key::F8,
        sf::Keyboard::Key::F9,
        sf::Keyboard::Key::F10,
        sf::Keyboard::Key::F11,
        sf::Keyboard::Key::F12,
        sf::Keyboard::Key::F13,
        sf::Keyboard::Key::F14,
        sf::Keyboard::Key::F15,
        sf::Keyboard::Key::Home,
        sf::Keyboard::Key::End,
        sf::Keyboard::Key::Enter,
        sf::Keyboard::Key::Escape,
        sf::Keyboard::Key::Insert,
        sf::Keyboard::Key::LAlt,
        sf::Keyboard::Key::LBracket,
        sf::Keyboard::Key::LControl,
        sf::Keyboard::Key::LShift,
        sf::Keyboard::Key::LSystem,
        sf::Keyboard::Key::Down,
        sf::Keyboard::Key::Up,
        sf::Keyboard::Key::PageUp,
        sf::Keyboard::Key::PageDown,
        sf::Keyboard::Key::Left,
        sf::Keyboard::Key::Right,
        sf::Keyboard::Key::RAlt,
        sf::Keyboard::Key::RBracket,
        sf::Keyboard::Key::RControl,
        sf::Keyboard::Key::RShift,
        sf::Keyboard::Key::RSystem,
        sf::Keyboard::Key::Menu,
        sf::Keyboard::Key::Pause,
        sf::Keyboard::Key::Period,
        sf::Keyboard::Key::Tab,
        sf::Keyboard::Key::Unknown,
    };

    // All numpads with its number equivalent
    std::map<int, char> numpads_to_char{
        {(int)sf::Keyboard::Key::Numpad0, '0'},
        {(int)sf::Keyboard::Key::Numpad1, '1'},
        {(int)sf::Keyboard::Key::Numpad2, '2'},
        {(int)sf::Keyboard::Key::Numpad3, '3'},
        {(int)sf::Keyboard::Key::Numpad4, '4'},
        {(int)sf::Keyboard::Key::Numpad5, '5'},
        {(int)sf::Keyboard::Key::Numpad6, '6'},
        {(int)sf::Keyboard::Key::Numpad7, '7'},
        {(int)sf::Keyboard::Key::Numpad8, '8'},
        {(int)sf::Keyboard::Key::Numpad9, '9'},
    };

    // All numbers keys with its number equivalent
    std::map<int, char> key_shift_version_to_char{
        {(int)sf::Keyboard::Key::Num0, '0'},
        {(int)sf::Keyboard::Key::Num1, '1'},
        {(int)sf::Keyboard::Key::Num2, '2'},
        {(int)sf::Keyboard::Key::Num3, '3'},
        {(int)sf::Keyboard::Key::Num4, '4'},
        {(int)sf::Keyboard::Key::Num5, '5'},
        {(int)sf::Keyboard::Key::Num6, '6'},
        {(int)sf::Keyboard::Key::Num7, '7'},
        {(int)sf::Keyboard::Key::Num8, '8'},
        {(int)sf::Keyboard::Key::Num9, '9'},
    };

    // All texts' langueges
    std::map<Main::Languages, sf::String> name_entering_lang{
        {Main::Languages::english, "Write your name here"},
        {Main::Languages::french, "Ecrivez votre nom ici"},
        {Main::Languages::malagasy, "Soraty eto ny anaranao"}
    };

    std::map<Main::Languages, sf::String> OK_lang{
        {Main::Languages::english, " OK "},
        {Main::Languages::french, " OK "},
        {Main::Languages::malagasy, " ENY "}
    };

    // For the new name/player/title
    std::string new_name = "";
    sf::Text new_name_txt(*new sf::Font());

    // All rectangles
    sf::RectangleShape writing_bar(sf::Vector2f(3, 35));
    sf::RectangleShape input(sf::Vector2f(340, 35));
    sf::RectangleShape add_player_menu_rect(sf::Vector2f(360, 150));
    sf::RectangleShape OK_rect;

    // All texts
    sf::Text Back(*new sf::Font());
    sf::Text OK(*new sf::Font());

    // For the writing_bar's animation
    void writing_bar_animation(){
        if(Main::clock_seconds(0.5, writing_bar_clock) == Main::GameTimer::Reached){
            if (bar_transparent == true)
                bar_transparent = false;
            else
                bar_transparent = true;
        }
        if (writing == true){
            if (bar_transparent == true)
                writing_bar.setFillColor(sf::Color::Transparent);
            else
                writing_bar.setFillColor(sf::Color::White);
        }
        else
            writing_bar.setFillColor(sf::Color::Transparent);
    }

    // If we want to save the new player
    void new_player(std::string &new_name){
        std::list<std::string> level_files_list;

        std::string path = "../Players/" + new_name + "/";
        std::filesystem::create_directory(path);

        std::fstream file(path + "data.save", std::ios::out | std::ios::binary);
        if(file.is_open()){
            bool first_play = true;
            file.seekp(sizeof(int)*3 + sizeof(bool), std::ios::beg);
            file.write(reinterpret_cast<char*>(&first_play), sizeof(bool));
            long long new_seed = Main::random_value();
            file.write(reinterpret_cast<char*>(&new_seed), sizeof(long long));
            file.close();
        }
        Debugging::write("\n" + path + "data.save FILE CREATED", Debugging::get_state_of(Debugging::File));
    }

    void Render(){
        Main::window()->draw(add_player_menu_rect);
        Main::window()->draw(Back);
        Main::window()->draw(input);
        Main::window()->draw(new_name_txt);
        Main::window()->draw(OK_rect);
        Main::window()->draw(OK);
        Main::window()->draw(writing_bar);

        Main::window()->display();
    }

    void Set_content_position(){
        x_middle = Main::window()->getSize().x/2 - add_player_menu_rect.getGlobalBounds().size.x/2;
        y_middle = Main::window()->getSize().y/2 - add_player_menu_rect.getGlobalBounds().size.y/2;
        add_player_menu_rect.setPosition({x_middle, y_middle});

        Back.setPosition({x_middle, y_middle});

        x_middle = Main::window()->getSize().x/2 - input.getGlobalBounds().size.x/2;
        input.setPosition(sf::Vector2f(x_middle, Back.getPosition().y+50));

        x_middle = Main::window()->getSize().x/2 - OK.getGlobalBounds().size.x/2;
        OK.setPosition(sf::Vector2f(x_middle, input.getPosition().y+50));

        OK_rect.setPosition(sf::Vector2f(OK.getGlobalBounds().position));
    }

    void Input(){
        while(std::optional event = Main::window()->pollEvent()){
            // =============== THE EVENTS =============== //
            if(Main::check_resize_events(event)){
                Main::first_drawing(true);
                size_event_detected = true;
                Main::context(Main::Context::PlayMenu);
            }

            // if the user wants to close the window
            if(event->is<sf::Event::Closed>()){
                Main::window()->close();
            }

            // if the user pressed a mouse's button
            if(event->is<sf::Event::MouseButtonPressed>()){

                // If the user push the 'Back button'
                if(Back.getGlobalBounds().contains({float(sf::Mouse::getPosition(*Main::window()).x), float(sf::Mouse::getPosition(*Main::window()).y)})){
                    Main::context(Main::Context::PlayMenu);
                    Main::first_drawing(true);
                    new_name.clear();
                    Debugging::write("Button : BACK\n", Debugging::get_state_of(Debugging::Button));
                }

                // If the user push on/out the 'input button'
                if(input.getGlobalBounds().contains({float(sf::Mouse::getPosition(*Main::window()).x), float(sf::Mouse::getPosition(*Main::window()).y)})){
                    writing = true; 
                    Debugging::write("WRITNG : TRUE\n", Debugging::get_state_of(Debugging::Description));
                }
                
                else{
                    writing = false;
                    Debugging::write("WRITNG : FALSE", Debugging::get_state_of(Debugging::Description));
                }

                // if the user push the 'OK button'
                if(OK_rect.getGlobalBounds().contains({float(sf::Mouse::getPosition(*Main::window()).x), float(sf::Mouse::getPosition(*Main::window()).y)}) && valid == true){
                    Debugging::write("Button : OK\n", Debugging::get_state_of(Debugging::Button));
                    new_player(new_name);
                    Debugging::write("ADDING NEW PLAYER : " + new_name + " TERMINATED\n", Debugging::get_state_of(Debugging::Description));
                    new_name.clear();
                    Main::first_drawing(true);
                    writing = false;
                    Main::context(Main::Context::PlayMenu);
                }
            }

            if(auto* key = event->getIf<sf::Event::KeyPressed>()){
                // If the user can write something
                if (writing == true){
                    // if he pushes one character
                    if(std::find(special_char.begin(), special_char.end(), key->code) == special_char.end()){
                        std::string character;

                        if((key->code != sf::Keyboard::Key::Space && numpads_to_char.find(int(key->code)) == numpads_to_char.end())
                            || (key_shift_version_to_char.find(int(key->code)) != key_shift_version_to_char.end() && !key->shift))
                            character = sf::Keyboard::getDescription(key->scancode);
                        if(key->code == sf::Keyboard::Key::Space)
                            character = " ";
                        if(numpads_to_char.find(int(key->code)) != numpads_to_char.end())
                            character = numpads_to_char[(int)key->code];
                        if(key_shift_version_to_char.find(int(key->code)) != key_shift_version_to_char.end() && key->shift)
                            character = key_shift_version_to_char[(int)key->code];

                        if(key->shift)
                            character[0] = std::toupper(character[0]);
                        else
                            character[0] = std::tolower(character[0]);

                        if(new_name.size() < 15)
                            new_name.insert(new_name.size(), character);
                        Debugging::write("TEXT ENTERED => NEW NAME : " + new_name, Debugging::get_state_of(Debugging::Description));
                    }
                
                    // if he want to delete a/all character(s)
                    if(key->code == sf::Keyboard::Key::Backspace && !new_name.empty()){
                        std::string new_txt = "";
                        for (int i = 0; i < new_name.size()-1; i++){
                            new_txt += new_name.at(i); 
                        }
                        new_name = new_txt;
                        Debugging::write("REMOVING LETTER => NEW NAME : " + new_txt, Debugging::get_state_of(Debugging::Description));
                    }

                    // verification if the new name is already on the game
                    if(verif == true){
                        std::list<std::string> players_name_list_file;
                        Main::get_files_in("../Players/", players_name_list_file);
                        if(players_name_list_file.empty() == true && new_name.empty() == false)
                            valid = true;
                        else if(!new_name.empty()){
                            if(std::find(players_name_list_file.begin(), players_name_list_file.end(), new_name) == players_name_list_file.end())
                                valid = true;
                            else    
                                valid = false;
                        }
                    }

                    // if he wants to save the name(the new name is valid)
                    if(key->code == sf::Keyboard::Key::Enter && valid){
                        new_player(new_name);
                        Debugging::write("ADDING NEW PLAYER : " + new_name + " TERMINATED", Debugging::get_state_of(Debugging::Description));
                        new_name.clear();
                        Main::first_drawing(true);
                        valid = false;
                        writing = false;
                        Main::context(Main::Context::PlayMenu);
                    }

                    // if the new name is empty
                    if(new_name.empty()){
                        new_name_txt.setString(name_entering_lang.at(Main::lang()));
                        new_name_txt.setFillColor(sf::Color(100,100,100));
                        valid = false;
                    }

                    // if the new name is not empty
                    else{
                        new_name_txt.setString(new_name);
                        x_middle = Main::window()->getSize().x/2 + new_name_txt.getGlobalBounds().size.x/2+5;
                        writing_bar.setPosition(sf::Vector2f(x_middle, input.getGlobalBounds().position.y));
                        if (valid == true)
                            new_name_txt.setFillColor(sf::Color(255,255,255));
                        else
                            new_name_txt.setFillColor(sf::Color(255,50,50));
                    }
                }

                // if he wants to save the name(the new name is valid)
                else if(key->code == sf::Keyboard::Key::Enter && valid){
                    Debugging::write("ADDING NEW PLAYER : " + new_name, Debugging::get_state_of(Debugging::Description));
                    new_player(new_name);
                    new_name.clear();
                    Main::first_drawing(true);
                    writing = false;
                    valid = false;
                    Main::context(Main::Context::PlayMenu);
                }
            }
        }
    }

    void Update_content(){
        // Setting the title 
        new_name_txt.setString(name_entering_lang.at(Main::lang()));
        x_middle = Main::window()->getSize().x/2 - new_name_txt.getGlobalBounds().size.x/2;
        new_name_txt.setPosition({x_middle, input.getPosition().y});

        // setting all things' color
        add_player_menu_rect.setFillColor(sf::Color(100,100,100));
        Back.setFillColor(sf::Color(255,255,255));
        input.setFillColor(sf::Color(0,0,0));
        input.setOutlineColor(sf::Color(255,255,255,0));
        OK.setFillColor(sf::Color(0,0,0));

        new_name_txt.setFillColor(sf::Color(100,100,100));
        writing_bar.setPosition(sf::Vector2f(Main::window()->getSize().x/2, input.getGlobalBounds().position.y));

        // setting all colors
        add_player_menu_rect.setFillColor(sf::Color(100,100,100));

        // new_name's color and writing_bar's position
        if (new_name.empty() == true){
            new_name_txt.setString(name_entering_lang.at(Main::lang()));
            new_name_txt.setFillColor(sf::Color(100,100,100));
            writing_bar.setPosition({float(Main::window()->getSize().x/2), float(input.getGlobalBounds().position.y)});
            x_middle = Main::window()->getSize().x/2 - new_name_txt.getGlobalBounds().size.x/2;
            new_name_txt.setPosition(sf::Vector2f(x_middle, input.getGlobalBounds().position.y));
            valid = false;
        }
        else {
            new_name_txt.setString(new_name);
            if (valid == true)
                new_name_txt.setFillColor(sf::Color(255,255,255));
            else
                new_name_txt.setFillColor(sf::Color(255,50,50));
            x_middle = Main::window()->getSize().x/2 + new_name_txt.getGlobalBounds().size.x/2 + 5;
            writing_bar.setPosition(sf::Vector2f(x_middle, input.getGlobalBounds().position.y));
            x_middle = Main::window()->getSize().x/2 - new_name_txt.getGlobalBounds().size.x/2;
            new_name_txt.setPosition(sf::Vector2f(x_middle, input.getGlobalBounds().position.y));
        }

        // Back's color
        if (Back.getGlobalBounds().contains({float(sf::Mouse::getPosition(*Main::window()).x), float(sf::Mouse::getPosition(*Main::window()).y)}))
            Back.setFillColor(sf::Color::Black);
        else
            Back.setFillColor(sf::Color(255,255,255));
        
        // OK's color
        if (OK.getGlobalBounds().contains({float(sf::Mouse::getPosition(*Main::window()).x), float(sf::Mouse::getPosition(*Main::window()).y)}) && valid == true)
            OK.setFillColor(sf::Color(0,0,0));
        else if (!OK.getGlobalBounds().contains({float(sf::Mouse::getPosition(*Main::window()).x), float(sf::Mouse::getPosition(*Main::window()).y)}) && valid == true)
            OK.setFillColor(sf::Color(sf::Color(255,255,0)));
        else if (!OK.getGlobalBounds().contains({float(sf::Mouse::getPosition(*Main::window()).x), float(sf::Mouse::getPosition(*Main::window()).y)}) && valid == false)
            OK.setFillColor(sf::Color(0,0,0));
        else
            OK.setFillColor(sf::Color(0,0,0));

        // Input's color
        if (writing == true)
            input.setOutlineColor(sf::Color::White);
        else
            input.setOutlineColor(sf::Color::Transparent);

        // the writing_bar's animation
        writing_bar_animation();
    }

    // for the add_player_menu's event
    void Event(){
        size_event_detected = false;
        Main::window()->setFramerateLimit(60);
        
        // the mouse and its rectangle
        sf::FloatRect mouse_rect(sf::Vector2f(sf::Mouse::getPosition(*Main::window())), sf::Vector2f(1,1));
        
        // the main font
        sf::Font Arial = *FontManager::get_font(FontManager::Arial);

        // Setting all texts parameters
        Back.setString(" < ");
        Back.setCharacterSize(40);
        Back.setFont(Arial);

        OK.setString(OK_lang.at(Main::lang()));
        OK.setCharacterSize(30);
        OK.setFont(Arial);

        OK_rect.setSize(sf::Vector2f(OK.getGlobalBounds().size));

        new_name_txt.setCharacterSize(30);
        new_name_txt.setFont(Arial);
        
        // for the outline parameters and OK_rect's color
        input.setOutlineThickness(-3);
        OK_rect.setOutlineThickness(3);

        OK_rect.setFillColor(sf::Color(50,50,255));
        OK_rect.setOutlineColor(sf::Color(50,50,255));

        Set_content_position();

        // It's for the first drawing
        if (Main::first_drawing()){
            Debugging::write("----ADDING PLAYER----\n", Debugging::get_state_of(Debugging::Description));
            Main::first_drawing(false);
        }

        Input();
        Update_content();
        Render();
    }
};
