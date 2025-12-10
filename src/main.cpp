#include "../headers/main.hpp"
#include "../headers/window_menu/main_menu_window.hpp"
#include "../headers/window_menu/set_lang_window.hpp"
#include "../headers/window_menu/play_window.hpp"
#include "../headers/window_menu/add_player_window.hpp"
#include "../headers/window_menu/player_data_window.hpp"
#include "../headers/window_menu/game_window.hpp"
#include "../headers/window_menu/game_pause_window.hpp"
#include "../headers/window_menu/game_over_window.hpp"
#include "../headers/window_menu/world_settings.hpp"

bool main_first_drawing = true;
sf::Vector2f old_window_size;

// The main window
sf::RenderWindow main_window;

sf::Sprite* main_background = nullptr;

// The app's menu
Main::Context main_context = Main::Context::MainMenu;

Main::Languages main_lang;

ContainManager::Object main_content_manager;

sf::Clock app_clock;

float app_delta_time = 0;

namespace Main{
    sf::RenderWindow* window(){
        return &main_window;
    }

    sf::Sprite* background(){
        return main_background;
    }

    bool first_drawing(){
        return main_first_drawing;
    }

    Languages lang(){
        return main_lang;
    }

    Context context(){
        return main_context;
    }

    ContainManager::Object* context_content(){
        return &main_content_manager;
    }

    void lang(Languages new_lang){
        main_lang = new_lang;
    }

    void context(Context new_context){
        main_context = new_context;
    }

    void first_drawing(bool activation){
        main_first_drawing = activation;
    }

    GameTimer clock_seconds(float seconds, sf::Clock &clock_to_compare, bool reset_timer){
        if (clock_to_compare.getElapsedTime().asSeconds() >= seconds){
            if(reset_timer == true){
                clock_to_compare.restart();
            }
            return Reached;
        }
        else
            return NotReached;
    }

    GameTimer clock_millisec(int milliSec, sf::Clock &clock_to_compare, bool reset_timer){
        if (clock_to_compare.getElapsedTime().asMilliseconds() >= milliSec){
            if(reset_timer == true){
                clock_to_compare.restart();
            }
            return Reached;
        }
        else
            return NotReached;
    }

    GameTimer clock_microsec(int microSec, sf::Clock &clock_to_compare, bool reset_timer){
        if (clock_to_compare.getElapsedTime().asMicroseconds() >= microSec){
            if(reset_timer == true){
                clock_to_compare.restart();
            }
            return Reached;
        }
        else
            return NotReached;
    }

    void get_files_in(std::string directory_path, std::list<std::string> &to_file_list){
        for (auto file_path : std::filesystem::directory_iterator(directory_path)){
            std::string file;
            std::string file_true_path = file_path.path().string();
            for(int i = directory_path.size(); i < file_true_path.size(); i++){
                char character = file_true_path[i];
                char* char_to_insert = &character;
                file.insert(file.size(), char_to_insert);
            }
            to_file_list.push_back(file);
        }
    }

    void get_files_path_in(std::string directory_path, std::list<std::string> &file_path_list){
        for (auto file_path : std::filesystem::directory_iterator(directory_path))
            file_path_list.push_back(file_path.path().string());
    }

    void sec_conversion(int seconds, std::string &result){
        int sec = 0, min = 0, calcul = 1;
        while (calcul > 0){
            calcul = seconds - 60;
            if (calcul > 0){
                min += 1;
                seconds -= 60;
            }
            else{
                if (seconds > 0)
                    sec = seconds;
                else
                    sec = 0;

                seconds = 0;
            }
        }
        result = std::to_string(min) + ":" + std::to_string(sec);
    }

    void get_FPS(sf::Clock &FPS_clock, sf::Clock &FPS_clock_update, float &the_FPS){
        float FPS = 1 / FPS_clock.getElapsedTime().asSeconds();
        FPS_clock.restart();
        if (clock_millisec(1500, FPS_clock_update) == Reached){
            if (FPS > MAX_FPS)
                FPS = MAX_FPS;
            the_FPS = FPS;
        }
    }

    sf::Image get_image_from(sf::IntRect rect, sf::Texture texture){
        sf::Image img;
        img.resize({(unsigned int)(rect.size.x), (unsigned int)(rect.size.y)}, sf::Color::White);
        if(texture.getSize().x >= rect.size.x && texture.getSize().y >= rect.size.y){
            for(unsigned int y_texture = rect.position.y, y = 0; y_texture < rect.position.y + rect.size.y, y < rect.size.y; y_texture++, y++){
                for(unsigned int x_texture = rect.position.x, x = 0; x_texture < rect.position.x + rect.size.x, x < rect.size.x; x_texture++, x++)
                    img.setPixel({x, y}, texture.copyToImage().getPixel({x_texture, y_texture}));
            }
        }
        return img;
    }

    long long get_seed(long long x, long long y, long long seed){
        long long seed_x = seed + x;
        long long seed_y = seed*y;
        long long new_seed = (seed_x - seed_y);
        return new_seed;
    }

    long long get_index(long long x, long long y, long long size_x, long long size_y){
        return (abs(x) % abs(size_x)) + (abs(y) % abs(size_y))*abs(size_y);
    }

    float absolute(float number){
        if(number < 0)
            number = -number;
        return number;
    }

    long long random_value(int min, long long max){
        std::random_device random_device;
        std::uniform_int_distribution<long long> int_distribution(min, max);
        return int_distribution(random_device);
    }

    bool check_resize_events(std::optional<sf::Event> event){
        bool resize_event_detected = false;
        if(event && event->is<sf::Event::Resized>()){
            sf::View view(sf::Vector2f(Main::window()->getSize().x/2, Main::window()->getSize().y/2), sf::Vector2f(Main::window()->getSize().x, Main::window()->getSize().y));
            Main::window()->setView(view);
            resize_event_detected = true;
        }
        //if(event.type == sf::Event::KeyPressed){
        //    if(event.text.unicode == sf::Keyboard::F10){
        //        old_window_size.x = window.getSize().x;
        //        old_window_size.y = window.getSize().y;
        //        if(full_screen == false){
        //            window.create(sf::VideoMode::getFullscreenModes()[0], game_name, sf::Style::Fullscreen);
        //            full_screen = true;
        //            resize_event_detected = true;
        //        }
        //    }
        //    if(event.text.unicode == sf::Keyboard::Escape && full_screen == true){
        //        window.create(sf::VideoMode(old_window_size.x, old_window_size.y), game_name, sf::Style::Default);
        //        sf::View view(sf::Vector2f(window.getSize().x/2, window.getSize().y/2), sf::Vector2f(window.getSize().x, window.getSize().y));
        //        window.setView(view);
        //        resize_event_detected = true;
        //        full_screen = false;
        //    }
        //}
        return resize_event_detected;
    }

    std::pair<float, float> get_pair2f_of(sf::Vector2f point){
        std::pair<float, float> point_converted;
        point_converted.first = int(point.x);
        point_converted.second= int(point.y);

        return point_converted;
    }

    sf::Vector2f get_vector2f_of(std::pair<float, float> point){
        sf::Vector2f point_converted;
        point_converted.x = int(point.first);
        point_converted.y = int(point.second);

        return point_converted;
    }

    sf::Vector2i get_vector2i_of(std::pair<float, float> point){
        sf::Vector2i point_converted;
        point_converted.x = int(point.first);
        point_converted.y = int(point.second);

        return point_converted;
    }

    sf::Vector2i get_vector2i_of(sf::Vector2f point){
        sf::Vector2i point_converted;
        point_converted.x = int(point.x);
        point_converted.y = int(point.y);

        return point_converted;
    }

    std::pair<int, int> get_pair2i_of(sf::Vector2f point){
        return std::pair<int, int>(int(point.x), int(point.y));
    }

    std::pair<int, int> get_pair2i_of(sf::Vector2i point){
        return std::pair<int, int>(point.x, point.y);
    }

    float delta_time(){
        return app_delta_time;
    }

}

void language_setting(){
    bool error = false;

    // Language configuration
    std::ifstream file_read("../res/language.save", std::ios::in | std::ios::binary);
    if (file_read.is_open()){
        int file_content_lang;
        file_read.read(reinterpret_cast<char*>(&file_content_lang), sizeof(int));
        for (const auto [key, value] : lang_value){
            if (file_content_lang != key) {
                Main::lang(Main::Languages::english);
                error = true;
            }
            else if (file_content_lang == key){
                Main::lang(value);
                error = false;
                Debugging::write("Loading the language completed", Debugging::get_state_of(Debugging::Description));
                break;
            }
        }
        file_read.close();
    } 

    if (error == true){
        Debugging::write("ERROR : LOADING THE LANGUAGE FAILED", Debugging::get_state_of(Debugging::Description));
        Debugging::write("LANGUAGE SETS TO ENGLISH", Debugging::get_state_of(Debugging::Description));
        std::ofstream file_write("../res/language.save", std::ios::out | std::ios::binary);
        if (file_write.is_open()){
            int lang_value = 0;
            file_write.write(reinterpret_cast<char*>(&lang_value), sizeof(int));
        } 
        file_write.close();
        Debugging::write("ERROR CORRECTION COMPLETED\n", Debugging::get_state_of(Debugging::Description));
    }
}

void write_work_of_today(){
    std::vector<std::string> all_works = {
        "- Let's make structures' content and roofs !!!'",
    };
    Debugging::write("WORK OF TODAY :");
    for(int i = 0; i < all_works.size(); i++)
        Debugging::write(all_works[i] + "\n");
}

int main(){
    Debugging::clear();
    Debugging::enable_all();
    Debugging::write("----APP CONFIGURATION----\n", Debugging::get_state_of(Debugging::Description));

    Debugging::write("Setting the window...\n", Debugging::get_state_of(Debugging::Description));
    Main::window()->create(sf::VideoMode(sf::Vector2u(1280, 720)), Main::game_name, sf::Style::Default);
    Main::window()->setFramerateLimit(60);
    Debugging::write("Window created", Debugging::get_state_of(Debugging::Description));

    language_setting();

    // clearing the window
    Main::window()->clear(sf::Color(0,0,0));
    Main::window()->display();

    // Starting all fonts
    FontManager::init_fonts();

    // background configuration

    // The main background
    sf::Texture bg_image;
    if(!bg_image.loadFromFile("../res/images/World/main_background.jpg"))
        Debugging::write("Background texture not loaded");
    sf::Sprite background(bg_image); // The background sprite
    main_background = &background;
    Debugging::write("Background loaded", Debugging::get_state_of(Debugging::Description));

    Debugging::write("----APP CONFIGURATION TERMINATED----\n", Debugging::get_state_of(Debugging::Description));
    Debugging::write("Launching application.....", Debugging::get_state_of(Debugging::Description));

    if(Main::window()->isOpen() == true)
        Debugging::write("Application launched !\n", Debugging::get_state_of(Debugging::Description));
    
    write_work_of_today();

    ThreadManager::init();
    
    Main::context_content()->set_window_to(*Main::window());

    // while the window is open
    while (Main::window()->isOpen()){
        float previous_time = app_clock.getElapsedTime().asSeconds();

        // if we are in the main_menu
        if(Main::context() == Main::Context::MainMenu)
            MainMenuWindow::Event();

        // if we are in the language_setting's menu
        else if(Main::context() == Main::Context::SetLangMenu)
            SetLanguageWindow::Event();

        // if we are in the Play_menu
        else if(Main::context() == Main::Context::PlayMenu)
            PlayWindow::Event();

        // if we are in the adding_player's menu
        else if(Main::context() == Main::Context::AddPlayer)
            AddPlayerWindow::Event();

        // if we are in the player's data menu (We took a player)
        else if(Main::context() == Main::Context::PlayerData)
            PlayerDataWindow::Event();

        // if we are in the world's settings' menu (We took a player)
        else if(Main::context() == Main::Context::WorldSettings)
            WorldSettingsWindow::Event();

        // if we are in the player's data menu (We took a player)
        else if(Main::context() == Main::Context::TheGame)
            GameWindow::Event();

        // if we are in the player's data menu (We took a player)
        else if(Main::context() == Main::Context::GamePauseMenu)
            GamePauseWindow::Event();

        // if we are in the game over menu (We took a player)
        else if(Main::context() == Main::Context::GameOver)
            GameOverWindow::Event();

        Main::get_FPS(GameWindow::fps_clock, GameWindow::FPS_update, GameWindow::fps);
        app_delta_time = app_clock.getElapsedTime().asSeconds() - previous_time;
    }
    
    Debugging::write("CLOSING WINDOW", Debugging::get_state_of(Debugging::Description));
    ThreadManager::terminates_all();
    ThreadManager::kill_all();
    FontManager::destroy_all();
    Debugging::write("APP TERMINATED", Debugging::get_state_of(Debugging::Description));

    return 0;
}

