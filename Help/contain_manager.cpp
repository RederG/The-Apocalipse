#include "contain_manager.hpp"

namespace ContainManager{

    Button::Button(sf::Text *text, sf::RenderWindow *window, sf::RectangleShape* rect_target){
        this->text_target = text;
        this->window_target = window;
        this->text_rect_target = rect_target;
    }

    bool Button::contains(sf::Vector2i mouse_position){
        sf::Vector2f mouse({float(mouse_position.x), float(mouse_position.y)});
        if(this->text_target->getGlobalBounds().contains(mouse))
            return true;
        else return false;
    }

    sf::Text* Button::get_text(){
        return this->text_target;
    }

    sf::RectangleShape* Button::get_text_rect(){
        return &*this->text_rect_target;
    }

    void Button::draw(){
        if(this->text_rect_target != nullptr)
            this->window_target->draw(*this->text_rect_target);
        this->window_target->draw(*this->text_target);
    }

    GraphicalSelector::GraphicalSelector(int choices_number, sf::Vector2f size, sf::Vector2f position){
        this->current_choice = 0;
        this->choices_number = choices_number;
        this->size = size;
        this->position = position;
    }
    
    int GraphicalSelector::get_current_choice(){
        return this->current_choice;
    }

    sf::Vector2f GraphicalSelector::get_position(){
        return this->position;
    }

    sf::Vector2f GraphicalSelector::get_size(){
        return this->size;
    }

    void GraphicalSelector::set_choices_number_to(int new_choices_number){
        this->choices_number = new_choices_number;
    }

    bool GraphicalSelector::contains(sf::Vector2f position){
        if(position.x >= this->position.x && position.x <= (this->position.x + this->size.x) 
            && position.y >= this->position.y && position.y <= (this->position.y + this->size.y))
            return true;
        else
            return false;
    }
    
    void GraphicalSelector::draw_to(sf::RenderWindow& window){
        float distance = this->size.x/(this->choices_number - 1);

        sf::RectangleShape main_rect(this->size);
        main_rect.setPosition(this->position);
        main_rect.setFillColor(sf::Color(80,80,80));
        main_rect.setOutlineColor(sf::Color::Black);
        main_rect.setOutlineThickness(1);

        sf::RectangleShape secondary_rect({distance*this->current_choice, this->size.y});
        secondary_rect.setPosition(this->position);
        secondary_rect.setFillColor(sf::Color(100,100,100));

        sf::Vector2f choice_selected_position({this->position.x + distance*this->current_choice, this->position.y});
        sf::RectangleShape choice_selector_rect({10, this->size.y});
        choice_selector_rect.setPosition(choice_selected_position);
        choice_selector_rect.setFillColor(sf::Color(100,100,100));
        choice_selector_rect.setOutlineColor(sf::Color(150,150,150));
        choice_selector_rect.setOutlineThickness(4); 
        choice_selector_rect.setOrigin({choice_selector_rect.getGlobalBounds().size.x/2, 0});

        window.draw(main_rect);
        window.draw(secondary_rect);

        sf::Vector2f delimiter_position = this->position;
        for(int i = 0; i < this->choices_number; i++){
            sf::RectangleShape delimiter_rect({1, this->size.y});
            delimiter_rect.setPosition(delimiter_position);
            delimiter_rect.setFillColor(sf::Color::Black);

            window.draw(delimiter_rect);
            delimiter_position.x += distance;
        }

        window.draw(choice_selector_rect);
    }

    void GraphicalSelector::update_current_choice_from(sf::Vector2f position){
        if(this->contains(position)){
            float distance = this->size.x/(this->choices_number - 1);
            this->current_choice = round((position.x - this->position.x)/distance);
            if(this->current_choice < 0)
                this->current_choice = 0;
            else if(this->current_choice > this->choices_number)
                this->current_choice = this->choices_number;
        }
    }

    void GraphicalSelector::set_position_to(sf::Vector2f position){
        this->position = position;
    }

    void GraphicalSelector::set_current_choice_to(int new_current_choice){
        this->current_choice = new_current_choice;
    }

    Object::Object(sf::RenderWindow &window, std::vector<std::pair<std::string, Types>> *priority_drawing_list){
        this->window_target = &window;
        this->priority_drawing_list_target = &*priority_drawing_list;
        this->all_sprites.clear();
        this->all_rectangles.clear();
        this->all_texts.clear();
        this->all_texts_rectangle.clear();
    }

    Object::Object(std::vector<std::pair<std::string, Types>> *priority_drawing_list){
        this->window_target = nullptr;
        this->priority_drawing_list_target = &*priority_drawing_list;
        this->all_sprites.clear();
        this->all_rectangles.clear();
        this->all_texts.clear();
        this->all_texts_rectangle.clear();
    }

    Object::Object(){
        this->window_target = nullptr;
        this->priority_drawing_list_target = nullptr;
        this->all_sprites.clear();
        this->all_rectangles.clear();
        this->all_texts.clear();
        this->all_texts_rectangle.clear();
    }

    void Object::set_window_to(sf::RenderWindow &window){
        this->window_target = &window;
    }

    void Object::set_priority_list_to(std::vector<std::pair<std::string, Types>> *priority_list){
        this->priority_drawing_list_target = &*priority_list;
    }

    void Object::add_sprite(std::string name){
        this->all_sprites[name] = std::make_shared<sf::Sprite>(*new sf::Sprite(*new sf::Texture()));
    }

    void Object::add_rect(std::string name){
        sf::RectangleShape new_rect;
        this->all_rectangles[name] = std::make_shared<sf::RectangleShape>(new_rect);
    }

    void Object::add_text(std::string name, std::string text, sf::Font *font, int size, bool has_rect){
        if(font != nullptr){
            sf::Text new_text(*font, text, size);
            this->all_texts[name] = std::make_shared<sf::Text>(new_text);
        }
        else if(font == nullptr){
            sf::Text new_text(*new sf::Font, text, size);
            this->all_texts[name] = std::make_shared<sf::Text>(new_text);
        }
        if(has_rect == true){
            sf::RectangleShape text_rect;
            this->all_texts_rectangle[name] = std::make_shared<sf::RectangleShape>(text_rect);
        }
    }

    void Object::add_text(std::string name, bool has_rect){
        sf::Text text(*new sf::Font());
        this->all_texts[name] = std::make_shared<sf::Text>(text);
        if(has_rect == true){
            sf::RectangleShape text_rect;
            this->all_texts_rectangle[name] = std::make_shared<sf::RectangleShape>(text_rect);
        }
    }

    void Object::add_button(std::string name, sf::Text *text){
        sf::RectangleShape* text_rect = nullptr;
        for(auto contain : this->all_texts){
            if(&*contain.second == &*text && this->all_texts_rectangle[contain.first] != nullptr){
                text_rect = &*this->all_texts_rectangle[contain.first];
                break;
            }
        }
        Button button(text, this->window_target, text_rect);
        this->all_buttons[name] = std::make_shared<Button>(button);
    }

    void Object::add_selector(std::string name, int choices_number, sf::Vector2f size){
        this->all_selectors[name] = std::make_shared<GraphicalSelector>(*new GraphicalSelector(choices_number, size, {0, 0}));
    }

    void Object::remove_sprite(std::string name){
        this->all_sprites.erase(name);
    }

    void Object::remove_rect(std::string name){
        this->all_rectangles.erase(name);
    }

    void Object::remove_text(std::string name){
        this->all_texts.erase(name);
    }

    void Object::remove_button(std::string name){
        this->all_buttons.erase(name);
    }

    void Object::remove_selector(std::string name){
        this->all_selectors.erase(name);
    }

    sf::Sprite* Object::get_sprite(std::string name){
        return &*this->all_sprites[name];
    }

    sf::RectangleShape* Object::get_rect(std::string name){
        return &*this->all_rectangles[name];
    }

    sf::Text* Object::get_text(std::string name){
        return &*this->all_texts[name];
    }

    Button* Object::get_button(std::string name){
        return &*this->all_buttons[name];
    }

    GraphicalSelector* Object::get_selector(std::string name){
        return &*this->all_selectors[name];
    }

    sf::RectangleShape* Object::get_text_rect(std::string name){
        return &*this->all_texts_rectangle[name];
    }

    void Object::configure_text(std::string name, sf::String text, sf::Font* font, int size, sf::Color color){
        this->all_texts[name]->setString(text);
        this->all_texts[name]->setFont(*font);
        this->all_texts[name]->setCharacterSize(size);
        this->all_texts[name]->setFillColor(color);
    }

    void Object::configure_rect(std::string name, sf::Vector2f size, sf::Color color, sf::Color outline_color, float outline_size){
        this->all_rectangles[name]->setSize(size);
        this->all_rectangles[name]->setFillColor(color);
        this->all_rectangles[name]->setOutlineColor(outline_color);
        this->all_rectangles[name]->setOutlineThickness(outline_size);
    }

    void Object::configure_text_rect(std::string name, sf::Color color, sf::Color outline_color, float outline_size){
        this->all_texts_rectangle[name]->setFillColor(color);
        this->all_texts_rectangle[name]->setOutlineColor(outline_color);
        this->all_texts_rectangle[name]->setOutlineThickness(outline_size);
    }

    void Object::updade_text_rect(std::string name){
        sf::Vector2f text_size = this->all_texts[name]->getGlobalBounds().size;
        sf::Vector2f text_pos = this->all_texts[name]->getGlobalBounds().position;
        if(this->all_texts_rectangle[name] != nullptr){
            this->all_texts_rectangle[name]->setSize(text_size);
            this->all_texts_rectangle[name]->setPosition(text_pos);
        }
    }

    void Object::set_position(sf::Text* Text, bool middle_x, bool middle_y){
        float x = 0, y = 0;
        if(middle_x == true){
            x = this->window_target->getSize().x/2 - Text->getGlobalBounds().size.x/2;
        }
        else{
            x = Text->getGlobalBounds().position.x;
        }
        if(middle_y == true){
            y = this->window_target->getSize().y/2 - Text->getGlobalBounds().size.y/2;
        }
        else{
            y = Text->getGlobalBounds().position.y;
        }
        for(auto text : this->all_texts){
            if(&*text.second == &*Text && this->all_texts_rectangle[text.first] != nullptr){
                this->all_texts_rectangle[text.first]->setPosition({x, y});
                break;
            }
        }
        Text->setPosition({x, y});
    }

    void Object::set_position(GraphicalSelector* selector, bool middle_x, bool middle_y){
        float x = 0, y = 0;
        if(middle_x == true){
            x = this->window_target->getSize().x/2 - selector->get_size().x/2;
        }
        else{
            x = selector->get_position().x;
        }
        if(middle_y == true){
            y = this->window_target->getSize().y/2 - selector->get_size().y/2;
        }
        else{
            y = selector->get_position().y;
        }
        for(auto text : this->all_selectors){
            if(&*text.second == &*selector && this->all_selectors[text.first] != nullptr){
                this->all_selectors[text.first]->set_position_to({x, y});
                break;
            }
        }
    }

    void Object::set_position(sf::Sprite* sprite, bool middle_x, bool middle_y){
        float x = 0, y = 0;
        if(middle_x == true){
            x = this->window_target->getSize().x/2 - sprite->getGlobalBounds().size.x/2;
        }
        else x = sprite->getGlobalBounds().position.x;
        if(middle_y == true){
            y = this->window_target->getSize().y/2 - sprite->getGlobalBounds().size.y/2;
        }
        else y = sprite->getGlobalBounds().position.y;
        sprite->setPosition({x, y});
    }

    void Object::set_position(sf::RectangleShape* rect, bool middle_x, bool middle_y){
        float x = 0, y = 0;
        if(middle_x == true){
            x = this->window_target->getSize().x/2 - rect->getGlobalBounds().size.x/2;
        }
        else x = rect->getGlobalBounds().position.x;
        if(middle_y == true){
            y = this->window_target->getSize().y/2 - rect->getGlobalBounds().size.y/2;
        }
        else y = rect->getGlobalBounds().position.y;
        rect->setPosition({x, y});
    }

    void Object::draw_all(){
        if(this->priority_drawing_list_target != nullptr && this->priority_drawing_list_target->empty() == false 
                                                         && this->window_target != nullptr){
            for(auto contain : *this->priority_drawing_list_target){
                if(contain.second == Types::Sprite){
                    if(this->all_sprites[contain.first] != nullptr)
                        this->window_target->draw(*this->all_sprites[contain.first]);
                }
                else if(contain.second == Types::Text){
                    if(this->all_texts[contain.first] != nullptr){
                        if(this->all_texts_rectangle[contain.first] != nullptr)
                            this->window_target->draw(*this->all_texts_rectangle[contain.first]);
                        this->window_target->draw(*this->all_texts[contain.first]);
                    }
                }
                else if(contain.second == Types::Rect){
                    if(this->all_rectangles[contain.first] != nullptr)
                        this->window_target->draw(*this->all_rectangles[contain.first]);
                }
                else if(contain.second == Types::Buttons){
                    if(this->all_buttons[contain.first] != nullptr)
                        this->all_buttons[contain.first]->draw();
                }
                else if(contain.second == Types::Selector)
                    this->all_selectors[contain.first]->draw_to(*this->window_target);
            }
        }
    }

    void Object::clear(){
        this->all_texts.clear();
        this->all_sprites.clear();
        this->all_rectangles.clear();
        this->all_buttons.clear();
        this->all_texts_rectangle.clear();
        this->all_selectors.clear();
    }

}
