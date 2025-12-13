#include "../../headers/Map/map_manager.hpp"
#include "../../headers/Map/Structures/structure_manager.hpp"

namespace Map{

    Area::Area(){
        this->size = sf::Vector2i(16, 16);
        this->map_position = sf::Vector2i(0, 0);
        this->all_elements_g_values_for_pathfinding.clear();
        this->all_elements_visit_for_pathfinding.clear();    
    }

    Area::Area(Area* area){
        this->size = area->size;
        this->map_position = area->map_position;
        this->window_position = area->window_position;
        this->all_elements = area->all_elements;
        this->configured = area->configured;
        this->all_elements_g_values_for_pathfinding = area->all_elements_g_values_for_pathfinding;
        this->all_elements_visit_for_pathfinding = area->all_elements_visit_for_pathfinding;    
    }

    Area::Area(sf::Vector2i size, sf::Vector2i position, sf::Vector2f window_position){
        this->size = size;
        this->map_position = position;
        this->window_position = window_position;
        this->all_elements_g_values_for_pathfinding.clear();
        this->all_elements_visit_for_pathfinding.clear();    
    }

    sf::Vector2i Area::get_size(){
        return this->size;
    }

    sf::Vector2i Area::get_position(){
        return this->map_position;
    }

    sf::Vector2f Area::get_window_position(){
        return this->window_position;
    }

    MapElement::WorldContent Area::get_element_at(int x, int y, PerlinNoise::Object* noise){
        MapElement::WorldContent element = MapElement::WorldContent::nothing;

        sf::Vector2i element_position;
        element_position.x = this->map_position.x*this->size.x + x;
        element_position.y = this->map_position.y*this->size.y + y;

        sf::Color color = noise->get_color_at(element_position.x, element_position.y);

        element = MapElement::Help::get_element_at(color);

        return element;
    }

    MapElement::WorldContent Area::get_element_at(sf::Vector2i element_coord, PerlinNoise::Object* noise){
        return this->get_element_at(element_coord.x, element_coord.y, noise);
    }
    
    MapElement::WorldContent Area::get_element_at(unsigned int x, unsigned int y, sf::Image area_image, bool is_structure){
        if(!is_structure)
            return MapElement::Help::get_element_at(area_image.getPixel({x, y}));
        else
            return StructureManager::get_environment_at(x, y, area_image);
    }

    MapElement::WorldContent Area::get_element_at(int x, int y){
        return all_elements[x][y];
    }

    MapElement::WorldContent Area::get_element_at(sf::Vector2i element_coord){
        return this->get_element_at(element_coord.x, element_coord.y);
    }

    MapElement::Environment* Area::get_environment_at(int x, int y){
        return new MapElement::Environment(this->all_elements[x][y], {float(18*this->map_position.x + x), float(18*this->map_position.y + y)});
    }

    MapElement::Environment* Area::get_environment_at(sf::Vector2i element_coord){
        return this->get_environment_at(element_coord.x, element_coord.y);
    }

    std::vector<std::vector<MapElement::WorldContent>> Area::get_all_elements(){
        return this->all_elements;
    }

    std::map<std::pair<float, float>, bool> Area::get_all_elements_visit(){
        return this->all_elements_visit_for_pathfinding;
    }

    std::map<std::pair<float, float>, float*> Area::get_all_elements_g_values(){
        return this->all_elements_g_values_for_pathfinding;
    }

    bool Area::is_configurated(){
        return this->configured;
    }

    bool Area::has_structure(){
        return this->structure_exists;
    }

    void Area::draw_to(Object* map){
        sf::Vector2f window_size, environment_pos_on_window, environment_pos;

        window_size.x = map->get_window()->getSize().x;
        window_size.y = map->get_window()->getSize().y;
        sf::Vector2f player_pos = map->get_player_map_pos();

        if(player_pos.x < float(int(player_pos.x)))
            player_pos.x = int(player_pos.x) - 1;
        if(player_pos.y < float(int(player_pos.y)))
            player_pos.y = int(player_pos.y) - 1;

        sf::Vector2f area_window_pos = this->get_window_position();
        area_window_pos.x += map->get_distance_player_and_window_middle().x;
        area_window_pos.y += map->get_distance_player_and_window_middle().y;

        sf::Color color = PerlinNoise::get(StructureManager::NOISE_NAME_STRUCT_SPAWN)->get_color_at(this->get_position().x, this->get_position().y);

        bool area_can_be_draw = false;
        if((area_window_pos.x > -32*map->get_scale().x*map->get_area_size().x && area_window_pos.x < window_size.x) && 
                (area_window_pos.y > -32*map->get_scale().y*map->get_area_size().y && area_window_pos.y < window_size.y))
            area_can_be_draw = true;
        if(area_can_be_draw){
            for(int y = 0; y < this->size.y && this->configured && !this->structure_exists; y++){
                for(int x = 0; x < this->size.x && this->configured && !this->structure_exists; x++){
                    MapElement::Environment environment(this->all_elements[x][y], {float(18*this->map_position.x + x), float(18*this->map_position.y + y)});
                    environment_pos_on_window = environment.get_window_position_on(map);
                    environment_pos = environment.get_map_position();
                    
                    if(-32*map->get_scale().x < environment_pos_on_window.x && environment_pos_on_window.x < window_size.x
                        && -32*map->get_scale().y < environment_pos_on_window.y && environment_pos_on_window.y < window_size.y
                        && environment.get_environment_element() != MapElement::WorldContent::nothing){

                        if(environment_pos.x == (int)player_pos.x && environment_pos.y == (int)player_pos.y)
                            environment.draw_to(map, sf::Color::Green);
                        else
                            environment.draw_to(map);
                    }
                }
            }
        }
    }

    void Area::configure_with(PerlinNoise::Object* noise){
        this->all_elements.clear();
        for(int x = 0; x < this->size.x; x++){
            this->all_elements.push_back(*new std::vector<MapElement::WorldContent>());
            for(int y = 0; y < this->size.y; y++)
                this->all_elements[x].push_back(MapElement::WorldContent::nothing);
        }

        for(int y = 0; y < this->size.y; y++){
            for(int x = 0; x < this->size.x; x++){
                sf::Vector2f element_map_position;
                element_map_position.x = this->map_position.x*this->size.x + x;
                element_map_position.y = this->map_position.y*this->size.x + y;

                if(element_map_position.x > 0 && this->map_position.x < 0)
                    element_map_position.x = -element_map_position.x;
                if(element_map_position.y > 0 && this->map_position.y < 0)
                    element_map_position.y = -element_map_position.y;

                MapElement::WorldContent element = this->get_element_at(x, y, noise);
                element = MapElement::Help::get_element_alternative(MapElement::WorldContent(element), PerlinNoise::get(Map::NAME_ALEATORY_NOISE)->get_color_at(element_map_position.x, element_map_position.y).r%3);

                this->all_elements[x][y] = element;

                this->all_elements_visit_for_pathfinding[Main::get_pair2f_of(element_map_position)] = false;
                this->all_elements_g_values_for_pathfinding[Main::get_pair2f_of(element_map_position)] = nullptr;
            }
        }
        this->configured = true;
    }

    void Area::set_window_position_to(sf::Vector2f new_position){
        this->window_position = new_position;
    }

    void Area::clear(){
        for(std::vector<MapElement::WorldContent> vector : this->all_elements)
            vector.clear();
        this->all_elements.clear();
        this->all_elements_visit_for_pathfinding.clear();
        for(auto g_value : this->all_elements_g_values_for_pathfinding){
            if(g_value.second != nullptr){
                delete g_value.second;
                g_value.second = nullptr;
            }
        }
        this->all_elements_g_values_for_pathfinding.clear();
    }

    void Area::save_to(std::string save_file_location){
        std::fstream save_file;
        if(!std::filesystem::exists(save_file_location))
            std::filesystem::create_directory((save_file_location));

        save_file_location = save_file_location;
        std::string area_file_name = std::to_string(this->map_position.x) + "_" + std::to_string(this->map_position.y);
        save_file_location += area_file_name + ".area";

        if(!std::filesystem::exists(save_file_location)){
            save_file.open(save_file_location, std::ios::out);
            save_file.close();
        }
        save_file.open(save_file_location, std::ios::out | std::ios::binary);
        if(save_file.is_open()){
            int area_position_x = this->map_position.x;
            int area_position_y = this->map_position.y;
            save_file.write(reinterpret_cast<char*>(&area_position_x), sizeof(int));
            save_file.write(reinterpret_cast<char*>(&area_position_y), sizeof(int));

            int area_size_x = this->size.x;
            int area_size_y = this->size.y;
            save_file.write(reinterpret_cast<char*>(&area_size_x), sizeof(int));
            save_file.write(reinterpret_cast<char*>(&area_size_y), sizeof(int));

            float area_window_pos_x = this->window_position.x;
            float area_window_pos_y = this->window_position.y;
            save_file.write(reinterpret_cast<char*>(&area_window_pos_x), sizeof(float));
            save_file.write(reinterpret_cast<char*>(&area_window_pos_y), sizeof(float));
            
            for(int y = 0; y < this->size.y; y++){
                for(int x = 0; x < this->size.x; x++)
                    if(!this->all_elements.empty() && !this->all_elements[x].empty())
                        save_file.write(reinterpret_cast<char*>(&this->all_elements[x][y]), sizeof(MapElement::WorldContent));
            }
            
            save_file.close();
        }
    }

    void Area::load_from(std::string load_location){
        std::fstream file;
        file.open(load_location, std::ios::in | std::ios::binary);

        if(file.is_open()){
            int area_pos_x = 0;
            int area_pos_y = 0;
            file.read(reinterpret_cast<char*>(&area_pos_x), sizeof(int));
            file.read(reinterpret_cast<char*>(&area_pos_y), sizeof(int));
            sf::Vector2i area_position(area_pos_x, area_pos_y);
            this->map_position = area_position;

            int area_size_x = 0;
            int area_size_y = 0;
            file.read(reinterpret_cast<char*>(&area_size_x), sizeof(int));
            file.read(reinterpret_cast<char*>(&area_size_y), sizeof(int));
            sf::Vector2i area_size(area_size_x, area_size_y);
            this->size = area_size;

            float area_window_pos_x = 0;
            float area_window_pos_y = 0;
            file.read(reinterpret_cast<char*>(&area_window_pos_x), sizeof(float));
            file.read(reinterpret_cast<char*>(&area_window_pos_y), sizeof(float));
            sf::Vector2f area_window_pos(area_window_pos_x, area_window_pos_y);
            this->window_position = area_window_pos;

            this->all_elements_g_values_for_pathfinding.clear();
            this->all_elements_visit_for_pathfinding.clear();
            this->all_elements.clear();
            for(int x = 0; x < this->size.x; x++){
                this->all_elements.push_back(*new std::vector<MapElement::WorldContent>());
                for(int y = 0; y < this->size.y; y++)
                    this->all_elements[x].push_back(MapElement::WorldContent::nothing);
            }
            
            for(int y = 0; y < this->size.y; y++){
                for(int x = 0; x < this->size.x; x++){
                    MapElement::WorldContent element;
                    file.read(reinterpret_cast<char*>(&element), sizeof(MapElement::WorldContent));
                    this->all_elements[x][y] = element;

                    sf::Vector2f element_map_position(18*this->map_position.x + x, 18*this->map_position.y + y);

                    this->all_elements_g_values_for_pathfinding[Main::get_pair2f_of(element_map_position)] = nullptr;
                    this->all_elements_visit_for_pathfinding[Main::get_pair2f_of(element_map_position)] = false; 
                }
            }
            this->configured = true;
        }
    }

}