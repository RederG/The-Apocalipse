#include "../../headers/main.hpp"
#include "../../headers/window_menu/play_window.hpp"
#include "../../headers/window_menu/add_player_window.hpp"

std::string player_choosed;

namespace PlayWindow{
    // This is the 'y position' for the first player 
    const int player_init_pos = 200;

    const std::map<Main::Languages, sf::String> back_lang{
        {Main::Languages::english, "< BACK"},
        {Main::Languages::french, "< RETOUR"},
        {Main::Languages::malagasy, "< MIVERINA"}
    };

    const std::map<Main::Languages, sf::String> title_lang{
        {Main::Languages::english, "CHOOSE A PLAYER"},
        {Main::Languages::french, "PRENEZ UN JOUEUR"},
        {Main::Languages::malagasy, "MISAFIDIANA MPILALAO IRAY"}
    };

    const std::map<Main::Languages, sf::String> add_player_lang{
        {Main::Languages::english, " Add a player "},
        {Main::Languages::french, " Ajouter un joueur "},
        {Main::Languages::malagasy, " Ampina mpilalao iray "}
    };

    // The mouse
    sf::FloatRect mouse_rect;

    std::list<std::string> players_list;

    std::vector<std::pair<std::string, ContainManager::Types>> all_elements{
        {"play_menu_rect", ContainManager::Rect},
        {"title", ContainManager::Text},
        {"back", ContainManager::Buttons},
        {"add_player_rect", ContainManager::Rect},
        {"add_player", ContainManager::Text},
        {"line_down", ContainManager::Rect},
        {"line_up", ContainManager::Rect},
    };

    void get_players(){
        std::string players_list_path = "../Players";
        if(!std::filesystem::exists(players_list_path))
            std::filesystem::create_directory(players_list_path);
        players_list.clear();
        for (auto path : std::filesystem::directory_iterator(players_list_path)){
            std::string player_name = path.path().string();
            std::string player_real_name;

            for (int i = players_list_path.size()+1; i < player_name.size(); i++){
                player_real_name += player_name.at(i);
            }

            players_list.push_back(player_real_name);
        }
    }

    void draw_players(){
        float Y = player_init_pos;
        sf::Font Arial = *FontManager::get_font(FontManager::Arial);

        for(std::string name : players_list){
            
            sf::Text Player(Arial, name, 30);
            float X = Main::window()->getSize().x/2 - Player.getGlobalBounds().size.x/2;
            Player.setPosition({X, Y});

            if (Player.getGlobalBounds().contains(mouse_rect.position) || name == player_choosed)
                Player.setFillColor(sf::Color(100,100,100));
            else
                Player.setFillColor(sf::Color::White);

            Main::window()->draw(Player);

            Y += 50;
        }
    }

    void one_player_pushed(float Y, sf::FloatRect mouse_rect){
        sf::Font Arial;
        if(!Arial.openFromFile("../res/Fonts/Arial_regular.ttf"))
            return;
        for (std::string name : players_list){
            
            sf::Text Player(Arial, name, 30);
            float X = Main::window()->getSize().x/2 - Player.getGlobalBounds().size.x/2;
            Player.setPosition({X, Y});

            Y += 50;

            if (Player.getGlobalBounds().contains(mouse_rect.position)){
                player_choosed = Player.getString();
                Debugging::write(player_choosed + " selected", Debugging::get_state_of(Debugging::Description));
                break;
            }
            else{
                player_choosed = "";
                Debugging::write("Nobody has been selected", Debugging::get_state_of(Debugging::Description));
            }
        }
    }

    void Render(){
        Main::window()->clear();

        Main::window()->draw(*Main::background());
        Main::context_content()->draw_all();
        draw_players();

        Main::window()->display();
    }

    void Set_content_position(){
        float x; 
        x = Main::window()->getSize().x/2 - Main::context_content()->get_rect("play_menu_rect")->getGlobalBounds().size.x/2;
        Main::context_content()->get_rect("play_menu_rect")->setPosition({x, 100});
        Main::context_content()->get_text("back")->setPosition({Main::context_content()->get_rect("play_menu_rect")->getPosition().x, Main::context_content()->get_rect("play_menu_rect")->getPosition().y});
                
        x = Main::window()->getSize().x/2 - Main::context_content()->get_text("title")->getGlobalBounds().size.x/2;
        Main::context_content()->get_text("title")->setPosition({x, Main::context_content()->get_text("back")->getPosition().y+30});
        Main::context_content()->get_rect("line_up")->setPosition({Main::context_content()->get_rect("play_menu_rect")->getPosition().x, Main::context_content()->get_text("title")->getPosition().y+60});
        Main::context_content()->get_rect("line_down")->setPosition({Main::context_content()->get_rect("play_menu_rect")->getPosition().x, Main::context_content()->get_rect("play_menu_rect")->getSize().y+Main::context_content()->get_rect("play_menu_rect")->getPosition().y-60});

        x = Main::window()->getSize().x/2 - Main::context_content()->get_text("add_player")->getGlobalBounds().size.x/2;
        Main::context_content()->get_text("add_player")->setPosition({x, Main::context_content()->get_rect("line_down")->getPosition().y+5});

        Main::context_content()->get_rect("add_player_rect")->setPosition(sf::Vector2f(Main::context_content()->get_text("add_player")->getGlobalBounds().position));    
    }

    void Update_content(){
        mouse_rect = sf::FloatRect(sf::Vector2f(sf::Mouse::getPosition(*Main::window())), sf::Vector2f(1,1));

        if(Main::context_content()->get_button("back")->contains({int(mouse_rect.position.x), int(mouse_rect.position.y)}))
            Main::context_content()->get_text("back")->setFillColor(sf::Color(100,100,100));
        else 
            Main::context_content()->get_text("back")->setFillColor(sf::Color(255,255,255));

        if(Main::context_content()->get_button("add_player")->contains({int(mouse_rect.position.x), int(mouse_rect.position.y)}))
            Main::context_content()->get_text("add_player")->setFillColor(sf::Color(255,100,100));
        else 
            Main::context_content()->get_text("add_player")->setFillColor(sf::Color(100,50,255));

        if(player_choosed.empty() == false){
            Main::first_drawing(true);
            Main::context(Main::Context::PlayerData);
        }
        
        if(AddPlayerWindow::size_event_detected){
            Main::first_drawing(true);
            Main::context(Main::Context::AddPlayer);
            Main::context_content()->get_text("add_player")->setFillColor(sf::Color(255,100,100));
        }
    }

    void Input(){
        while(std::optional event = Main::window()->pollEvent()){
            Main::check_resize_events(event);

            // If the user wants to close the window
            if(event->is<sf::Event::Closed>()){
                players_list.clear();
                Main::window()->close();
            }
            
            // If the user pushes a mouse's button
            if(event->is<sf::Event::MouseButtonPressed>()){
                if(Main::context_content()->get_button("back")->contains({int(mouse_rect.position.x), int(mouse_rect.position.y)})){
                    Main::context(Main::Context::MainMenu);
                    Main::first_drawing(true);
                    Debugging::write("Button : BACK\n", Debugging::get_state_of(Debugging::Button));
                }
                
                if(Main::context_content()->get_button("add_player")->contains({int(mouse_rect.position.x), int(mouse_rect.position.y)})){
                    Main::context(Main::Context::AddPlayer);
                    Main::first_drawing(true);
                    Debugging::write("Button : ADD A PLAYER\n", Debugging::get_state_of(Debugging::Button));
                }
                one_player_pushed(player_init_pos, mouse_rect);
            }
        }
    }

    void Event(){
        // The FPS
        Main::window()->setFramerateLimit(60);

        Main::context_content()->clear();
        
        // All texts

        Main::context_content()->add_text("title");
        Main::context_content()->add_text("back");
        Main::context_content()->add_text("add_player");

        // All rectagles

        Main::context_content()->add_rect("play_menu_rect");
        Main::context_content()->add_rect("line_up");
        Main::context_content()->add_rect("line_down");
        Main::context_content()->add_rect("add_player_rect");

        Main::context_content()->configure_rect("play_menu_rect", sf::Vector2f(720, 480), sf::Color(0,0,0,100), sf::Color(0,0,0,100), 1);
        Main::context_content()->configure_rect("line_up", sf::Vector2f(720, 2), sf::Color(255,255,255), sf::Color(255,255,255), 1);
        Main::context_content()->configure_rect("line_down", sf::Vector2f(720, 2), sf::Color(255,255,255), sf::Color(255,255,255), 1);

        Main::context_content()->add_button("back", Main::context_content()->get_text("back"));
        Main::context_content()->add_button("add_player", Main::context_content()->get_text("add_player"));

        Main::context_content()->get_text("back")->setString(back_lang.at(Main::lang()));
        Main::context_content()->get_text("back")->setCharacterSize(30); Main::context_content()->get_text("back")->setFont(*FontManager::get_font(FontManager::Arial));

        Main::context_content()->get_text("title")->setString(title_lang.at(Main::lang())); 
        Main::context_content()->get_text("title")->setCharacterSize(30); 
        Main::context_content()->get_text("title")->setFont(*FontManager::get_font(FontManager::Arial));
        
        Main::context_content()->get_text("add_player")->setString(add_player_lang.at(Main::lang())); 
        Main::context_content()->get_text("add_player")->setCharacterSize(30); 
        Main::context_content()->get_text("add_player")->setFont(*FontManager::get_font(FontManager::Arial));
        
        Main::context_content()->get_rect("add_player_rect")->setSize(sf::Vector2f(Main::context_content()->get_text("add_player")->getGlobalBounds().size));
        Main::context_content()->get_rect("add_player_rect")->setOutlineThickness(2);

        // Their color
        Main::context_content()->get_rect("play_menu_rect")->setFillColor(sf::Color(0,0,0,100));
        Main::context_content()->get_rect("line_up")->setFillColor(sf::Color(255,255,255));
        Main::context_content()->get_rect("line_down")->setFillColor(sf::Color(255,255,255));
        Main::context_content()->get_text("back")->setFillColor(sf::Color(255,255,255));
        Main::context_content()->get_text("title")->setFillColor(sf::Color(255,200,100));
        Main::context_content()->get_text("add_player")->setFillColor(sf::Color(100,50,255));
        Main::context_content()->get_rect("add_player_rect")->setFillColor(sf::Color(255,200,0));
        Main::context_content()->get_rect("add_player_rect")->setOutlineColor(sf::Color(255,200,0));

        Set_content_position();
        
        Main::context_content()->set_priority_list_to(&all_elements);

        if(player_choosed.empty() == false){
            Main::first_drawing(true);
            Main::context(Main::Context::PlayerData);
        }
        
        if(AddPlayerWindow::size_event_detected){
            Main::first_drawing(true);
            Main::context(Main::Context::AddPlayer);
            Main::context_content()->get_text("add_player")->setFillColor(sf::Color(255,100,100));
        }

        // The first drawing
        if(Main::first_drawing()){
            Debugging::write("----PLAY MENU----\n", Debugging::get_state_of(Debugging::Description));
            get_players();
            Main::first_drawing(false);
            Render();
        }
        
        Input();
        Update_content();
        Render();
    }
};
