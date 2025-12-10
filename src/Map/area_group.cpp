#include "../../headers/Map/map_manager.hpp"

namespace Map{

    AreaGroup::AreaGroup(){
        this->area_group_size = sf::Vector2i(5, 5);
        this->position = sf::Vector2i(0, 0);
        this->area_size = sf::Vector2i(16, 16);
        this->position.x = 0;
        this->position.y = 0;

        float first_area_on_group_x = 0;
        float first_area_on_group_y = 0;
        float area_start_x = first_area_on_group_x;
        float area_start_y = first_area_on_group_y;

        float area_end_x = area_start_x;
        float area_end_y = area_start_y;
        area_end_x += area_group_size.x;
        area_end_y += area_group_size.y;

        this->accepted_area_position_x.first = area_start_x;
        this->accepted_area_position_x.second = area_end_x;

        this->accepted_area_position_y.first = area_start_y;
        this->accepted_area_position_y.second = area_end_y;
        this->init();
    }

    AreaGroup::AreaGroup(sf::Vector2i area_group_size, sf::Vector2i area_group_position, sf::Vector2i area_size){
        this->area_group_size = area_group_size;
        this->position = area_group_position;
        this->area_size = area_size;
        this->position.x = area_group_position.x;
        this->position.y = area_group_position.y;

        float first_area_on_group_x = area_group_position.x * this->area_group_size.x;
        float first_area_on_group_y = area_group_position.y * this->area_group_size.y;
        float area_start_x = first_area_on_group_x;
        float area_start_y = first_area_on_group_y;

        float area_end_x = area_start_x;
        float area_end_y = area_start_y;
        area_end_x += area_group_size.x;
        area_end_y += area_group_size.y;

        this->accepted_area_position_x.first = area_start_x;
        this->accepted_area_position_x.second = area_end_x;

        this->accepted_area_position_y.first = area_start_y;
        this->accepted_area_position_y.second = area_end_y;

        this->init();
    }

    AreaGroup::AreaGroup(sf::Vector2i area_group_size, sf::Vector2f area_start_position, sf::Vector2i area_size){
        this->area_group_size = area_group_size;
        this->position = sf::Vector2i(0,0);    
        this->area_size = area_size;

        sf::Vector2f area_group_position;
        area_group_position.x = area_start_position.x;
        area_group_position.y = area_start_position.y;
        area_group_position.x /= this->area_group_size.x;
        area_group_position.y /= this->area_group_size.y;
        if(area_group_position.x < float(int(area_group_position.x)))
            area_group_position.x = int(area_group_position.x) - 1;
        if(area_group_position.y < float(int(area_group_position.y)))
            area_group_position.y = int(area_group_position.y) - 1;
            
        sf::Vector2i true_area_group_position;
        true_area_group_position.x = area_group_position.x;
        true_area_group_position.y = area_group_position.y;

        float first_area_on_group_x = true_area_group_position.x * this->area_group_size.x;
        float first_area_on_group_y = true_area_group_position.y * this->area_group_size.y;
        float area_start_x = first_area_on_group_x;
        float area_start_y = first_area_on_group_y;

        float area_end_x = area_start_x;
        float area_end_y = area_start_y;
        area_end_x += area_group_size.x;
        area_end_y += area_group_size.y;

        this->accepted_area_position_x.first = area_start_x;
        this->accepted_area_position_x.second = area_end_x;

        this->accepted_area_position_y.first = area_start_y;
        this->accepted_area_position_y.second = area_end_y;

        this->position.x = true_area_group_position.x;
        this->position.y = true_area_group_position.y;
        
        this->init();
    }

    sf::Vector2i AreaGroup::get_map_position(){
        return this->position;
    }

    sf::Vector2i AreaGroup::get_size(){
        return this->area_group_size;
    }

    Area* AreaGroup::get_area_at(float x, float y){
        return this->get_area_at(sf::Vector2f(x, y));
    }

    Area* AreaGroup::get_area_at(sf::Vector2f area_position){
        Area* area = nullptr;
        if(this->is_mine(area_position)){
            if(this->all_areas.empty() == false){
                int index = Main::get_index(area_position.x, area_position.y, this->area_group_size.x, this->area_group_size.y);
                if(int(area_position.x) == 0 && area_position.x < 0)
                    index--;
                if(int(area_position.y) == 0 && area_position.y < 0)
                    index -= this->area_group_size.y;
                area = &*this->all_areas[index];
            }
        }
        return area;
    }

    Area* AreaGroup::add_area(float x, float y){
        Area* new_area = nullptr;
        if(this->is_mine(x, y)){
            Area* area_ptr = this->get_area_at(int(x), int(y));
            if(area_ptr != nullptr){
                new_area = &*area_ptr;
            }
            else{
                Area* new_area_ptr = new Area(this->area_size, sf::Vector2i(x, y));
                
                int index = Main::get_index(x, y, this->area_group_size.x, area_group_size.y);
                if(int(x) == 0 && x < 0)
                    index--;

                this->all_areas[index] = new_area_ptr;
                new_area = &*new_area_ptr;
            }
        }
        return new_area;
    }

    Area* AreaGroup::add_area(sf::Vector2f area_position){
        return this->add_area(area_position.x, area_position.y);
    }

    Area* AreaGroup::add_area(Area* area){
        Area* area_ptr = nullptr;
        if(this->is_mine(area->get_position().x, area->get_position().y)){
            int index = Main::get_index(area->get_position().x, area->get_position().y, this->area_group_size.x, this->area_group_size.y);
            this->all_areas[index] = area;
            area_ptr = area;
        }
        return area_ptr;
    }

    std::vector<Area*> AreaGroup::get_all_areas(){
        return this->all_areas;
    }

    void AreaGroup::clear(){
        if(this->all_areas.empty() == false){
            for(auto& area : this->all_areas){
                if(area != nullptr){
                    area->clear();
                    delete area;
                }
            }
        }
        this->all_areas.clear();
    }

    bool AreaGroup::is_mine(float x, float y){
        return this->is_mine(sf::Vector2f(x, y));
    }

    bool AreaGroup::is_mine(sf::Vector2f position){
        float x = position.x;
        float y = position.y;

        if((x >= this->accepted_area_position_x.first && x < this->accepted_area_position_x.second) && (y >= this->accepted_area_position_y.first && y < this->accepted_area_position_y.second))
            return true;
        else
            return false;
    }

    void AreaGroup::init(){
        this->all_areas.clear();
        for(int i = 0; i < this->area_group_size.x*this->area_group_size.y; i++)
            this->all_areas.push_back(nullptr);
    }

};