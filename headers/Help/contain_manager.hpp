#ifndef __CONTAIN_MANAGER__
    #define __CONTAIN_MANAGER__

    #include <SFML/Graphics.hpp>
    #include <vector>
    #include <map>
    #include <string>
    #include <cmath>

    // Controller of the things realated to the window 
    namespace ContainManager{
        // Some sfml types
        enum Types{
            Sprite, Texture, Text, Rect, Buttons, Switch, Selector
        };
    
        // The button
        class Button{
            public:
                Button(sf::Text *text, sf::RenderWindow *window, sf::RectangleShape* rect_target = nullptr);
                bool contains(sf::Vector2i mouse_position);
                sf::Text* get_text();
                sf::RectangleShape* get_text_rect();
                void draw();
            private:
                sf::Text *text_target;
                sf::RectangleShape *text_rect_target = nullptr;
                sf::RenderWindow *window_target;
        };

        // The selector
        class GraphicalSelector{
            public:
                GraphicalSelector(int choices_number, sf::Vector2f size, sf::Vector2f position);
                int get_current_choice();
                sf::Vector2f get_size();
                sf::Vector2f get_position();
                void set_choices_number_to(int new_choices_number);
                void set_position_to(sf::Vector2f position);
                void set_current_choice_to(int new_current_choice);
                void draw_to(sf::RenderWindow& window);
                bool contains(sf::Vector2f position);
                void update_current_choice_from(sf::Vector2f position);
            private:
                int current_choice, choices_number;
                sf::Vector2f size, position;
        };

        // The contain manager's object
        class Object{
            public:
                Object(sf::RenderWindow &window, std::vector<std::pair<std::string, Types>> *priority_drawing_list);
                Object(std::vector<std::pair<std::string, Types>> *priority_drawing_list);
                Object();
                void set_window_to(sf::RenderWindow &window);
                void set_priority_list_to(std::vector<std::pair<std::string, Types>> *priority_list);
                void add_sprite(std::string name);
                void add_text(std::string name, bool has_rect);
                void add_text(std::string name, std::string text = "text", sf::Font *font = nullptr, int size = 1, bool has_rect = false);
                void add_rect(std::string name);
                void add_button(std::string name, sf::Text *text);
                void add_selector(std::string name, int choices_number, sf::Vector2f size);
                void remove_sprite(std::string name);
                void remove_text(std::string name);
                void remove_rect(std::string name);
                void remove_button(std::string name);
                void remove_selector(std::string name);
                sf::Sprite* get_sprite(std::string name);
                sf::Text* get_text(std::string name);
                sf::RectangleShape* get_text_rect(std::string name);
                sf::RectangleShape* get_rect(std::string name);
                Button* get_button(std::string name);
                GraphicalSelector* get_selector(std::string name);
                void configure_text(std::string name, sf::String text, sf::Font* font, int size, sf::Color color = sf::Color::White); 
                void configure_text_rect(std::string name, sf::Color color, sf::Color outline_color, float outline_size);
                void configure_rect(std::string name, sf::Vector2f size, sf::Color color, sf::Color outline_color, float outline_size);
                void updade_text_rect(std::string name);
                void set_position(sf::Text *Text, bool middle_x = false, bool middle_y = false);
                void set_position(GraphicalSelector *selector, bool middle_x = false, bool middle_y = false);
                void set_position(sf::Sprite *sprite, bool middle_x = false, bool middle_y = false);
                void set_position(sf::RectangleShape *rect, bool middle_x = false, bool middle_y = false);
                void draw_all();
                void clear();

            private:
                sf::RenderWindow *window_target;
                std::vector<std::pair<std::string, Types>> *priority_drawing_list_target;
                std::map<std::string, std::shared_ptr<sf::Sprite>> all_sprites;
                std::map<std::string, std::shared_ptr<sf::Text>> all_texts;
                std::map<std::string, std::shared_ptr<sf::RectangleShape>> all_texts_rectangle;
                std::map<std::string, std::shared_ptr<sf::RectangleShape>> all_rectangles;
                std::map<std::string, std::shared_ptr<Button>> all_buttons;
                std::map<std::string, std::shared_ptr<GraphicalSelector>> all_selectors;
                sf::Sprite* sprite;
        };
    };
    
#endif