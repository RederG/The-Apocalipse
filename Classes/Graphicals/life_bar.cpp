#include "life_bar.hpp"

namespace Life_Bar{
    bool pause_drawing = false;

    std::list< std::shared_ptr<Object> > container;

    Object::Object(float max_life){
        life_rect_green.setSize(sf::Vector2f(96, 10));
        life_rect_red.setSize(sf::Vector2f(96, 10));
        life_rect_green.setFillColor(sf::Color::Green);
        life_rect_red.setFillColor(sf::Color::Red);
        this->max_life = max_life;
    }

    void Object::draw_to(Entity::Object* entity, Map::Object* map){

        if(pause_drawing == false){
            this->life_rect_green.setFillColor(sf::Color::Green);
            this->life_rect_red.setFillColor(sf::Color::Red);

            if(Main::clock_seconds(1, this->appearing_clock) == Main::Reached){
                this->showing_time++;
            }
            
            if(this->showing_time >= 3){
                this->life_rect_green.setFillColor(sf::Color::Transparent);
                this->life_rect_red.setFillColor(sf::Color::Transparent);
            }
        }

        if(this->life_now <= 0)
            this->life_now = 1;

        const int PIXEL_X = 90;
        unsigned int x = (int(this->life_now)*PIXEL_X) / int(this->max_life);
        this->life_rect_green.setSize(sf::Vector2f(x*map->get_scale().x, 10*map->get_scale().y));
        this->life_rect_red.setSize(sf::Vector2f(PIXEL_X*map->get_scale().x, 10*map->get_scale().y));

        sf::Sprite entity_sprite = entity->get_sprite(map);    

        sf::Vector2f entity_pos = entity->get_window_position_on(map);

        sf::Vector2f entity_default_position = entity_pos;
        entity_default_position.x -= entity_sprite.getOrigin().x*entity_sprite.getScale().x;
        entity_default_position.y -= entity_sprite.getOrigin().y*entity_sprite.getScale().y;

        sf::Vector2f life_bar_position = entity_default_position;
        life_bar_position.x += entity_sprite.getGlobalBounds().size.x/2;
        life_bar_position.x -= this->life_rect_red.getSize().x/2;
        life_bar_position.y -= this->life_rect_red.getSize().y + 10*map->get_scale().y;
        
        this->life_rect_red.setPosition(life_bar_position);
        this->life_rect_green.setPosition(life_bar_position);

        if(map->get_window() != nullptr){
            map->get_window()->draw(this->life_rect_red);
            map->get_window()->draw(this->life_rect_green);
        }
    }

    void Object::set_new_life_to(int new_life){
        life_now = new_life;
    }

    void Object::reset_timer(){
        this->showing_time = 0;
    }

    void set_pause_drawing_to(bool state){
        pause_drawing = state;
    }

}