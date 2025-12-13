#include "../../headers/Map/map_element.hpp"
#include "../../headers/Help/the_apocalipse.hpp"
#include "../../headers/Map/Structures/structure_manager.hpp"

namespace MapElement{
    
    Object::Object(){
        this->map_position = sf::Vector2f(0, 0);
        this->element_type = Type::World;
    }

    sf::Vector2f Object::get_map_position(bool virtual_position){
        return this->map_position;
    }

    sf::Vector2f Object::get_window_position_on(Map::Object* map){
        sf::Vector2f area_position;
        sf::Vector2i i_area_position = map->get_area_position(this->map_position.x, this->map_position.y);
        area_position.x = i_area_position.x;
        area_position.y = i_area_position.y;

        sf::Vector2f element_position_on_area;
        element_position_on_area.x = this->map_position.x;
        element_position_on_area.y = this->map_position.y;
        element_position_on_area.x -= int(area_position.x)*map->get_area_size().x;
        element_position_on_area.y -= int(area_position.y)*map->get_area_size().y;

        Map::Area* area = map->get_area_at(area_position.x, area_position.y);

        std::vector<Map::Area *> working_areas = map->get_all_working_areas();
        if(std::find(working_areas.begin(), working_areas.end(), area) == working_areas.end())
            area = nullptr;
            
        sf::Vector2f element_window_position = sf::Vector2f(0, 0);
        if(area != nullptr)
            element_window_position = area->get_window_position();
        element_window_position.x += 32*map->get_scale().x*element_position_on_area.x;
        element_window_position.y += 32*map->get_scale().y*element_position_on_area.y;

        sf::Vector2f distance = map->get_distance_player_and_window_middle();
        element_window_position.x += distance.x;
        element_window_position.y += distance.y;

        return element_window_position;
    }

    sf::RectangleShape Object::get_collision_rect(Map::Object* map, sf::Color color){
        sf::RectangleShape rect;
        sf::Sprite element_sprite = this->get_sprite(map);
        rect.setSize(element_sprite.getGlobalBounds().size);
        rect.setPosition(element_sprite.getPosition());
        rect.setFillColor(color);
        return rect;
    }

    Type Object::get_element_type(){
        return this->element_type;
    }

    InteractiveObjects::Object* Object::get_nearest_interactive_object(){
        return this->nearest_object;
    }

    sf::Sprite Object::get_sprite(Map::Object* map, sf::Color color){
        return *new sf::Sprite(*new sf::Texture());
    }

    bool Object::collides_with(Object* element, Map::Object* map){
        sf::FloatRect element_hitbox = element->get_collision_rect(map).getGlobalBounds();
        sf::FloatRect this_hitbox = this->get_collision_rect(map).getGlobalBounds();
        if(this_hitbox.findIntersection(element_hitbox).has_value())
            return true;
        else
            return false;
    }

    void Object::draw_to(Map::Object* map, sf::Color color){
        if(map->get_window() != nullptr){
            map->get_window()->draw(this->get_sprite(map, color));
        }
    }

    void Object::move_on_map(float x, float y){
        this->map_position.x += x;
        this->map_position.y += y;
    }

    void Object::move_on_map(sf::Vector2f distance){
        this->move_on_map(distance.x, distance.y);
    }

    void Object::set_map_position_to(sf::Vector2f new_position){
        this->map_position = new_position;
    }

    void Object::set_map_position_to(float x, float y){
        this->set_map_position_to(sf::Vector2f(x, y));
    }

    void Object::set_nearest_interactive_object_on(Map::Object* map, float minimal_distance){
        this->nearest_object = Help::get_nearest_interactive_object_of(this, map, minimal_distance);
    }

    void Object::update(){}

    namespace Help{

        WorldContent get_element_at(sf::Color color){
            WorldContent element;
            if(color == sf::Color::Blue)
                element = soil_herb_1;
            else if(color == sf::Color::White)
                element = soil_rocks_1;
            else if(color == sf::Color::Black)
                element = walls;
            else if(color == sf::Color::Red)
                element = border_map;
            else if(color == sf::Color::Green)
                element = spawn_zombie_1;
            else if(color == sf::Color(0,0,0,0))
                element = nothing;
            return element;
        }

        WorldContent get_element_alternative(WorldContent element, char i){
            WorldContent final_element = element;
            switch (element){
                case soil_rocks_1:
                    switch (i){
                        case 1:
                            final_element = soil_rocks_2;
                            break;
                        case 2:
                            final_element = soil_rocks_3;
                            break;
                        default:
                            break;
                    }
                    break;
                case soil_herb_1:
                    switch (i){
                        case 1:
                            final_element = soil_herb_2;
                            break;
                        case 2:
                            final_element = soil_herb_3;
                            break;
                        default:
                            break;
                    }
                    break;
                case spawn_zombie_1:
                    switch (i){
                        case 1:
                            final_element = WorldContent::spawn_zombie_2;
                            break;
                        case 2:
                            final_element = WorldContent::spawn_zombie_3;
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
            return final_element;
        }

        InteractiveObjects::Object* get_nearest_interactive_object_of(MapElement::Object* element, Map::Object* map, float minimal_distance){
            sf::Vector2i element_area = map->get_area_position(element->get_map_position().x, element->get_map_position().y);
            InteractiveObjects::Object* nearest_obj = nullptr;
            float nearest_distance = minimal_distance;

            for(int y = element_area.y - 1; y <= element_area.y + 1; y++){
                for(int x = element_area.x - 1; x <= element_area.x + 1; x++){
                    if(StructureManager::is_structure({x, y}) && StructureManager::search_finished()){
                        Structure::Instance& structure = StructureManager::get_structure(sf::Vector2i({x, y}));
                        for(int i = 0; i < structure.contain.walls.size() && StructureManager::search_finished() && structure.contains_walls; i++){
                            InteractiveObjects::Object* wall = structure.contain.walls[i];
                            if(wall->get_interaction_type() != InteractiveObjects::Type::nothing){
                                sf::Vector2f distance = wall->get_map_position();
                                distance.x -= int(element->get_map_position().x);
                                distance.y -= int(element->get_map_position().y);

                                float real_distance = sqrt(distance.x*distance.x + distance.y*distance.y);
                                if(real_distance <= nearest_distance){
                                    nearest_distance = real_distance;
                                    nearest_obj = wall;
                                }
                            }
                        }
                    }
                }
            }
            for(auto tomb : Tomb::container){
                sf::Vector2f distance = tomb->get_map_position();
                distance.x -= element->get_map_position().x;
                distance.y -= element->get_map_position().y;

                float real_distance = sqrt(distance.x*distance.x + distance.y*distance.y);
                if(real_distance < nearest_distance){
                    nearest_distance = real_distance;
                    nearest_obj = &*tomb;
                }
            }
            return nearest_obj;
        }

        sf::Color get_color_of(WorldContent element){
            sf::Color color;
            switch (element){
                case walls:
                    color = sf::Color::Black;
                    break;

                case soil_herb_1:
                    color = sf::Color::Blue;
                    break;

                case soil_rocks_1:
                    color = sf::Color::White;
                    break;

                case nothing:
                    color = sf::Color(0,0,0,0);
                    break;

                case border_map:
                    color = sf::Color::Red;
                    break;

                case spawn_zombie_1:
                    color = sf::Color::Green;
                    break;
                
                default:
                    break;
            }
            return color;
        }

        void sort_by_positions(std::vector<Object*>& element_list){
            for(int i = 0; i < element_list.size() && TheApocalipse::is_running(); i++){
                for(int j = 0; j < element_list.size() && TheApocalipse::is_running(); j++){
                    int corrrect_position = 0;
                    Object* obj = element_list[j];
                    for(int k = 0; k < element_list.size() && TheApocalipse::is_running(); k++){
                        if(obj->get_map_position(true).y >= element_list[k]->get_map_position(true).y)
                            corrrect_position = k;
                    }
                    element_list.erase(element_list.begin() + j);
                    element_list.insert(element_list.begin() + corrrect_position, obj);
                }
            }
        }
    
    }
}