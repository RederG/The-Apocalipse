#include "pathfinding.hpp"
#include "../Entities/entity.hpp"
#include "cmath"

namespace PathFinding{
    A_star::A_star(){
        this->all_points_to_check.clear();
        this->all_values.clear();
        this->all_points.clear();
        this->all_old_points.clear();
        this->building_path = false;
        this->map_target = nullptr;
    }

    std::vector<std::pair<float, float>> A_star::get_adjacent_point_of(std::pair<float, float> point){
        std::vector<std::pair<float, float>> all_adjacent_points;

        sf::Vector2f point_middle = Main::get_vector2f_of(point);
        sf::Vector2f up = point_middle, down = point_middle, left = point_middle, right = point_middle;
        
        up.y -= 1;
        down.y += 1;
        right.x += 1;
        left.x -= 1;

        all_adjacent_points.push_back(Main::get_pair2f_of(up));
        all_adjacent_points.push_back(Main::get_pair2f_of(down));
        all_adjacent_points.push_back(Main::get_pair2f_of(right));
        all_adjacent_points.push_back(Main::get_pair2f_of(left));

        return all_adjacent_points;
    }

    std::vector<sf::Vector2f> A_star::get_path(){
        std::vector<sf::Vector2f> path;
        for(int i = 0; i < this->path.size(); i++)
            path.push_back(Main::get_vector2f_of(this->path[i]));
        return path;
    }

    float A_star::get_value_of(std::pair<float, float> point, std::pair<float, float> parent_point){
        float value = 0;

        sf::Vector2f point_to_check = Main::get_vector2f_of(point);
        sf::Vector2f parent_point_position = Main::get_vector2f_of(parent_point);
        sf::Vector2i area = this->map_target->get_area_position(point.first, point.second);
        std::pair<int, int> point_area = Main::get_pair2f_of(sf::Vector2f(area.x, area.y));

        area = this->map_target->get_area_position(parent_point.first, parent_point.second);
        std::pair<int, int> parent_point_area = Main::get_pair2f_of(sf::Vector2f(area.x, area.y));

        sf::Vector2f distance1;
        distance1.x = point_to_check.x - parent_point_position.x;
        distance1.y = point_to_check.y - parent_point_position.y;

        float g_value = 0;
        if(this->all_g_values[parent_point_area][parent_point] != nullptr)
            g_value = sqrt(distance1.x*distance1.x + distance1.y*distance1.y) + *this->all_g_values[parent_point_area][parent_point];

        if(this->all_g_values[point_area][point] == nullptr)
            this->all_g_values[point_area][point] = &g_value;

        else if(*this->all_g_values[point_area][point] > g_value)
            this->all_g_values[point_area][point] = &g_value;

        float g = *this->all_g_values[point_area][point];

        float h = 0;
        sf::Vector2f distance3;
        distance3.x = point_to_check.x - this->end_point.x;
        distance3.y = point_to_check.y - this->end_point.y;

        h = sqrt(distance3.x*distance3.x + distance3.y*distance3.y);

        value = g + h;
        return value;
    }

    bool A_star::found_end_position(){
        bool found = false;
        bool end_point_correct = this->is_correct(Main::get_pair2f_of(this->end_point));
        if(Main::get_vector2f_of(this->all_points_to_check[this->all_values.front()]) == this->end_point && end_point_correct){
            found = true;
        }
        else if(!end_point_correct){
            std::vector<std::pair<float, float>> end_point_adj_points = this->get_adjacent_point_of(Main::get_pair2f_of(this->end_point));
            for(auto adj_point : end_point_adj_points){
                if(adj_point == this->all_points_to_check[this->all_values.front()]){
                    found = true;
                    this->end_point = Main::get_vector2f_of(adj_point);
                    break;
                }
            }
        }
        return found;
    }

    bool A_star::found_path(){
        bool path_found = false;
        //6283872
        if(!this->path.empty() && !this->parent_points.empty()){
            if(this->parent_points.find(this->path.front()) != this->parent_points.end()){
                if(this->parent_points[this->path.front()] == Main::get_pair2f_of(this->start_point))
                    path_found = true;
            }
        }
        return path_found;
    }

    bool A_star::can_find_path(){
        bool can_find = false;
        if(this->map_target != nullptr){
            sf::Vector2i area_position = this->map_target->get_area_position(this->start_point.x, this->start_point.y);
            Map::Area* start_area = this->map_target->get_area_at(area_position);
            bool player_has_changed_area = this->map_target->is_player_changed_area();
            
            if(start_area != nullptr && this->map_target->is_for_render(start_area)){
                bool map_updated = this->map_target->is_updated();
                if(this->all_points.empty() && map_updated){
                    this->create_map_of(this->map_target);
                    this->reset();
                    can_find = true;
                }
                else if(!map_updated){
                    this->reset();
                    this->all_points.clear();
                    can_find = false;
                }
                else if(map_updated && !this->all_points.empty())
                    can_find = true;
            }
            if(player_has_changed_area && this->map_target->is_for_render(start_area)){
                this->create_map_of(this->map_target);
                this->reset();
                can_find = true;
            }
            if(this->all_values.empty()){
                this->reset();
                can_find = false;
            }
        }
        else{
            this->reset();
            can_find = false;
        }

        return can_find;
    }

    bool A_star::is_correct(std::pair<float, float> point){
        bool correct = true;
        sf::Vector2f point_to_check = Main::get_vector2f_of(point);
        MapElement::Environment* environment = nullptr;
        if(this->map_target != nullptr)
            environment = this->map_target->get_environment_at(point_to_check);
        if(environment != nullptr && environment->can_move_on())
            correct = true;
        else
            correct = false;
        return correct;
    }

    bool A_star::is_parent_point_of(std::pair<float, float> point, std::pair<float, float> parent_point){
        std::vector<std::pair<float, float>> adj_points = this->get_adjacent_point_of(point);
        bool correct = false;
        for(int i = 0; i < adj_points.size(); i++){
            if(adj_points[i] == parent_point){
                correct = true;
                break;
            }
        }
        return correct;
    }

    void A_star::set_up(sf::Vector2f start_position, sf::Vector2f end_position, Map::Object* map){
        if(start_position.x < float(int(start_position.x)))
            start_position.x = int(start_position.x) - 1;
        if(start_position.y < float(int(start_position.y)))
            start_position.y = int(start_position.y) - 1;
        
        if(end_position.x < float(int(end_position.x)))
            end_position.x = int(end_position.x) - 1;
        if(end_position.y < float(int(end_position.y)))
            end_position.y = int(end_position.y) - 1;
        
        sf::Vector2f start = Main::get_vector2f_of(Main::get_pair2f_of(start_position));
        sf::Vector2f end = Main::get_vector2f_of(Main::get_pair2f_of(end_position));
        
        this->map_target = &*map;

        bool end_position_correct = this->is_correct(Main::get_pair2f_of(end));

        if((start != this->start_point || end != this->end_point) && end_position_correct){
            if(this->all_points.empty() == false){
                this->reset();
                this->all_points = this->all_old_points;
            }
            else{
                this->reset();
            }
        }
        else if(start != this->start_point && !end_position_correct){
            if(this->all_points.empty() == false){
                this->reset();
                this->all_points = this->all_old_points;
            }
            else{
                this->reset();
            }
        }

        this->start_point = start;
        if(end_position_correct)
            this->end_point = end;
        else
            this->true_end_point = end;
    }

    void A_star::create_map_of(Map::Object* map){
        if(map->is_updated()){

            if(!this->all_old_points.empty())
                this->all_points = this->all_old_points;
            else
                this->all_points.clear();
            if(!this->all_old_g_values.empty())
                this->all_g_values = this->all_old_g_values;
            else
                this->all_g_values.clear();

            for(auto area : map->get_rendering_areas()){
                sf::Vector2f area_position;
                area_position.x = area->get_position().x;
                area_position.y = area->get_position().y;

                if(this->all_points.find(Main::get_pair2f_of(area_position)) == this->all_points.end())
                    this->all_points[Main::get_pair2f_of(area_position)] = area->get_all_elements_visit();
                if(this->all_g_values.find(Main::get_pair2f_of(area_position)) == this->all_g_values.end())
                    this->all_g_values[Main::get_pair2f_of(area_position)] = area->get_all_elements_g_values();
            }

            sf::Vector2i area = map->get_area_position(this->start_point.x, this->start_point.y);
            float g_value = 0;
            this->all_g_values[Main::get_pair2f_of(sf::Vector2f(area.x, area.y))][Main::get_pair2f_of(this->start_point)] = &g_value;

            for(auto entity : Entity::container){
                sf::Vector2f entity_pos = Main::get_vector2f_of(Main::get_pair2f_of(entity->get_map_position()));
                if(entity_pos != this->end_point && entity_pos != this->start_point){
                    std::pair<float, float> entity_position = Main::get_pair2f_of(entity->get_map_position());
                    sf::Vector2i area = this->map_target->get_area_position(entity_position.first, entity_position.second);
                    std::pair<int, int> entity_area(area.x, area.y);
                    this->all_points[entity_area][entity_position] = true;
                }
            }
            this->all_old_points = this->all_points;
            this->all_old_g_values = this->all_g_values;
        }
    }

    void A_star::reset(){
        this->all_points_to_check.clear();
        this->all_points_to_check[0] = Main::get_pair2f_of(this->start_point);

        this->all_values.clear();
        this->all_values.push_back(0);

        this->parent_points.clear();
        this->path.clear();

        this->all_g_values = this->all_old_g_values;
        this->all_points = this->all_old_points;

        this->building_path = false;
    }

    void A_star::find_path(){
        if(!this->found_path()){
            bool end_position_found = this->found_end_position();
            if(this->can_find_path() && !end_position_found)
                this->search();
            if(end_position_found)
                this->create_path();
        }
    }

    void A_star::search(){
        std::pair<float, float> point_to_check = this->all_points_to_check[this->all_values.front()];
        sf::Vector2i area = this->map_target->get_area_position(point_to_check.first, point_to_check.second);
        std::pair<int, int> point_to_check_area(area.x, area.y);

        if(this->all_points[point_to_check_area][point_to_check] == false){
            this->all_points[point_to_check_area][point_to_check] = true;
            
            std::vector<std::pair<float, float>> all_adjacent_points = this->get_adjacent_point_of(point_to_check);
            for(std::pair<float, float>& adjacent_point : all_adjacent_points){
                area = this->map_target->get_area_position(adjacent_point.first, adjacent_point.second);
                std::pair<int, int> adj_point_area(area.x, area.y);

                if(this->is_correct(adjacent_point) && this->all_points[adj_point_area][adjacent_point] == false){
                    float value = this->get_value_of(adjacent_point, point_to_check);
                    this->all_values.push_back(value);
                    this->all_points_to_check[value] = adjacent_point;
                    this->all_points[adj_point_area][adjacent_point] = false;
                    this->parent_points[adjacent_point] = point_to_check;
                }
            }
        }

        this->all_values.erase(this->all_values.begin());
        this->all_values.sort();
    }

    void A_star::create_path(){
        if(!this->found_path()){
            if(this->building_path == false){
                this->path.clear();
                std::vector<std::pair<float, float>> old_path = this->path;
                this->path.clear();
                this->path.push_back(Main::get_pair2f_of(this->end_point));
                for(int i = 0; i < old_path.size(); i++)
                    this->path.push_back(old_path[i]);
                building_path = true;
            }
            std::pair<float, float> parent_point = this->parent_points[this->path.front()];
            if(this->is_parent_point_of(this->path.front(), parent_point)){
                std::vector<std::pair<float, float>> old_path = this->path;
                this->path.clear();
                this->path.push_back(parent_point);
                for(int i = 0; i < old_path.size(); i++)
                    this->path.push_back(old_path[i]);
                building_path = true;
            }
        }
    }

    void A_star::clear_path(){
        this->path.clear();
    }

};