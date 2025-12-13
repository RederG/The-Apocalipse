#include "../../headers/Map/map_manager.hpp"
#include "../../headers/Entities/entity.hpp"
#include "../../headers/InteractiveObjects/all_interactive_objects.hpp"
#include "../../headers/Entities/player.hpp"
#include "../../headers/Others/bullet.hpp"
#include "../../headers/window_menu/world_settings_window.hpp"
#include "../../headers/Help/thread_manager.hpp"

namespace Map{
    std::map<std::string, Object*> all_maps;

    Object::Object(sf::RenderWindow &window, PerlinNoise::Object &noise, sf::Vector2i area_size, sf::Vector2i area_group_size, sf::Vector2f scale, sf::Vector2i terrain_generation_size, sf::Vector2i updating_radius){
        this->window_target = &window;
        this->noise_target = &noise;
        this->scale = scale;
        this->area_size = area_size;
        this->area_group_size = area_group_size;
        this->terrain_generation_size = terrain_generation_size;
        this->event_updating_size = updating_radius;
        this->all_areas_group.clear();
    }

    Object::Object(sf::RenderWindow &window){
        this->window_target = &window;
    }

    sf::RenderWindow* Object::get_window(){
        return this->window_target;
    }

    PerlinNoise::Object* Object::get_noise(){
        return this->noise_target;
    }

    sf::Vector2f Object::get_scale(){
        return this->scale;
    }

    sf::Vector2f Object::get_player_pos_on_window(){
        sf::Vector2f player_pos, player_pos_window, player_pos_on_area;
        sf::Vector2i player_area;

        player_pos = this->get_player_map_pos();

        player_area = this->get_area_position(player_pos.x, player_pos.y);

        player_pos_on_area.x = player_pos.x - player_area.x*this->area_size.x;
        player_pos_on_area.y = player_pos.y - player_area.y*this->area_size.y;
        
        player_pos_window = this->get_area_at(player_area.x, player_area.y)->get_window_position();

        player_pos_window.x += player_pos_on_area.x*32*this->scale.x;
        player_pos_window.y += player_pos_on_area.y*32*this->scale.y;

        return player_pos_window;
    }

    sf::Vector2f Object::get_player_map_pos(){
        sf::Vector2f player_pos;
        for(auto entity : Entity::container){
            if(typeid(*entity) == typeid(Player::Object)){
                player_pos = entity->get_map_position();
                break;
            }
        }
        return player_pos;
    }

    sf::Vector2f Object::get_distance_player_and_window_middle(){
        sf::Vector2f distance;
        this->player_pos_window = this->get_player_pos_on_window();

        distance.x = (this->window_target->getSize().x/2 - this->player_pos_window.x);
        distance.y = (this->window_target->getSize().y/2 - this->player_pos_window.y);
        return distance;
    }

    sf::Vector2f Object::get_map_position_of(sf::Vector2f window_position){
        std::map<std::pair<float, float>, std::pair<int, int>> all_areas;
        for(auto area : this->all_working_areas){
            std::pair<float, float> area_window_position;
            area_window_position.first  = area->get_window_position().x;
            area_window_position.second = area->get_window_position().y;

            area_window_position.first  += this->get_distance_player_and_window_middle().x;
            area_window_position.second += this->get_distance_player_and_window_middle().y;

            std::pair<int, int> area_position;
            area_position.first  = area->get_position().x;
            area_position.second = area->get_position().y;

            all_areas[area_window_position] = area_position;
        }

        sf::Vector2f area_window_size;
        area_window_size.x = 32*this->area_size.x*this->scale.x;
        area_window_size.y = 32*this->area_size.y*this->scale.y;

        sf::Vector2f map_position = sf::Vector2f(0, 0);
        for(auto pair : all_areas){
            float x = pair.first.first;
            float y = pair.first.second;
            float x_max = x + area_window_size.x;
            float y_max = y + area_window_size.y;

            if(window_position.x >= x && window_position.x < x_max && window_position.y >= y && window_position.y < y_max){
                sf::Vector2f area_window_position;
                area_window_position.x = pair.first.first;
                area_window_position.y = pair.first.second;

                map_position.x = (window_position.x - area_window_position.x)/(32*this->scale.x);
                map_position.y = (window_position.y - area_window_position.y)/(32*this->scale.y);

                map_position.x += pair.second.first*this->area_size.x;
                map_position.y += pair.second.second*this->area_size.y;
                break;
            }
        }
        return map_position;
    }

    sf::Vector2i Object::get_area_size(){
        return this->area_size;
    }

    sf::Vector2i Object::get_terrain_generation_size(){
        return this->terrain_generation_size;
    }

    Area* Object::get_player_area(){
        sf::Vector2f player_pos = this->get_player_map_pos();
        sf::Vector2i player_area_position = this->get_area_position(player_pos.x, player_pos.y);
        return this->get_area_at(player_area_position.x, player_area_position.y);
    }

    Area* Object::get_area_at(float area_x, float area_y){
        Area* area = nullptr;

        AreaGroup* area_group = nullptr;
        for(int i = 0; i < this->all_areas_group.size(); i++){
            AreaGroup* area_group_ptr = &*this->all_areas_group[i];
            if(area_group_ptr != nullptr && area_group_ptr->is_mine(area_x, area_y)){
                area_group = &*this->all_areas_group[i];
                break;
            }
        }
        if(area_group == nullptr){
            area_group = this->new_area_group_that_contains(sf::Vector2f(area_x, area_y));
            area = &*area_group->add_area(area_x, area_y);
        }
        else if(area_group != nullptr){
            area = area_group->get_area_at(area_x, area_y);
            if(area == nullptr)
                area = area_group->add_area(area_x, area_y);
        }
        return area;
    }

    Area* Object::get_area_at(sf::Vector2i area_map_position){
        return this->get_area_at(area_map_position.x, area_map_position.y);
    }

    Area* Object::get_area_of(MapElement::Object* element){
        Area* area = nullptr;

        sf::Vector2f element_position = element->get_map_position();
        sf::Vector2i element_area_position = this->get_area_position(element_position.x, element_position.y);

        area = this->get_area_at(element_area_position);

        return area;
    }

    sf::Vector2i Object::get_spawn_position(int seed){
        sf::Vector2i spawn;

        Area* area = this->get_area_at(get_spawn_area(seed));
        if(area != nullptr && !area->is_configurated())
            area->configure_with(this->noise_target);
        std::vector<sf::Vector2i> all_correct_spawn;
        for(int x = 0; x < area->get_all_elements().size(); x++){
            for(int y = 0; y < area->get_all_elements()[x].size(); y++){
                MapElement::Environment envi(area->get_all_elements()[x][y], {0, 0});
                if(envi.can_move_on())
                    all_correct_spawn.push_back({18*area->get_position().x + x, 18*area->get_position().y + y});
            }
        }
        if(!all_correct_spawn.empty())
            spawn = all_correct_spawn[Main::random_value() % all_correct_spawn.size()];
        return spawn;
    }

    sf::Vector2i Object::get_area_position(float x, float y){
        sf::Vector2f area_position;
        area_position.x = x;
        area_position.y = y;
        area_position.x /= this->area_size.x;
        area_position.y /= this->area_size.y;
        if(area_position.x < float(int(area_position.x)))
            area_position.x = int(area_position.x) -1;
        if(area_position.y < float(int(area_position.y)))
            area_position.y = int(area_position.y) -1;
            
        sf::Vector2i true_area_position;
        true_area_position.x = area_position.x;
        true_area_position.y = area_position.y;
        return true_area_position;
    }

    Area* Object::new_area(int x, int y){
        Area* new_area = nullptr;

        sf::Vector2i area_group_position = this->get_area_group_position(x, y);

        if(this->get_area_at(x, y) == nullptr){
            AreaGroup* area_group = this->get_area_group(area_group_position);
            if(area_group != nullptr){
                new_area = &*area_group->add_area(x, y);
            }
            else{
                area_group = this->new_area_group(area_group_position);
                new_area = &*this->get_area_group(area_group_position)->add_area(x, y);
            }
        }
        return new_area;
    }

    Area* Object::new_area(sf::Vector2i map_position){
        return this->new_area(map_position.x, map_position.y);
    }

    Area* Object::new_area(Area* new_area){
        sf::Vector2i area_position = new_area->get_position();
        sf::Vector2i area_group_position = this->get_area_group_position(area_position.x, area_position.y);
        if(this->get_area_group(area_group_position) == nullptr)
            this->new_area_group(area_group_position);
        return this->get_area_group(area_group_position)->add_area(new_area);
    }

    MapElement::Environment* Object::get_environment_at(sf::Vector2f map_position){
        return this->get_environment_at(map_position.x, map_position.y);
    }

    MapElement::Environment* Object::get_environment_at(sf::Vector2i map_position){
        return this->get_environment_at(map_position.x, map_position.y);
    }

    MapElement::Environment* Object::get_environment_at(float x, float y){
        MapElement::Environment* environment = nullptr;

        sf::Vector2i area_position, element_position_on_area;
        area_position = this->get_area_position(x, y);

        Area* area = this->get_area_at(area_position);
        if(area != nullptr && area->is_configurated()){
            element_position_on_area = sf::Vector2i(int(x), int(y));
            element_position_on_area.x -= area->get_position().x*area->get_size().x;
            element_position_on_area.y -= area->get_position().y*area->get_size().y;

            environment = area->get_environment_at(element_position_on_area);
        }
        return environment;
    }

    std::vector<Area*> Object::get_all_working_areas(){
        return this->all_working_areas;
    }

    std::vector<Area*> Object::get_all_updating_areas(){
        return this->all_updating_areas;
    }

    std::vector<std::pair<int, int>> Object::get_all_areas_position(){
        std::vector<std::pair<int, int>> all_areas_position;
        for(int i = 0; i < this->all_working_areas.size(); i++){
            std::pair<int, int> position = {this->all_working_areas[i]->get_position().x, this->all_working_areas[i]->get_position().y};
            all_areas_position.push_back(position);
        }
        return all_areas_position;
    }

    AreaGroup* Object::get_area_group(int x, int y){
        AreaGroup* area_group = nullptr;
        for(auto area_group_ptr : this->all_areas_group){
            if(area_group_ptr->get_map_position().x == x && area_group_ptr->get_map_position().y == y){
                area_group = &*area_group_ptr;
                break;
            }
        }
        return area_group;
    }

    AreaGroup* Object::get_area_group(sf::Vector2i area_group_position){
        return this->get_area_group(area_group_position.x, area_group_position.y);
    }

    AreaGroup* Object::new_area_group(int x, int y){
        AreaGroup* area_group = nullptr;
        if(this->get_area_group(x, y) == nullptr){
            AreaGroup* area_group_ptr = new AreaGroup(this->area_group_size, sf::Vector2i(x, y), this->area_size);
            this->all_areas_group.push_back(area_group_ptr);
            area_group = &*area_group_ptr;
        }
        return area_group;
    }

    AreaGroup* Object::new_area_group(sf::Vector2i area_group_position){
        return this->new_area_group(area_group_position.x, area_group_position.y);
    }

    AreaGroup* Object::new_area_group_that_contains(sf::Vector2f area_position){
        AreaGroup* area_group = nullptr;
        for(int i = 0; i < this->all_areas_group.size(); i++){
            AreaGroup* area_group_ptr = &*this->all_areas_group[i];
            if(area_group_ptr != nullptr && area_group_ptr->is_mine(area_position)){
                area_group = &*this->all_areas_group[i];
                break;
            }
        }
        if(area_group == nullptr){
            AreaGroup* new_area_group = new AreaGroup(this->area_group_size, area_position, this->area_size);
            this->all_areas_group.push_back(new_area_group);
            area_group = &*new_area_group;
        }
        return area_group;
    }

    sf::Vector2i Object::get_area_group_position(float area_x, float area_y){
        sf::Vector2i area_group_position;
        area_group_position.x = area_x/this->area_group_size.x;
        area_group_position.y = area_y/this->area_group_size.y;

        sf::Vector2f area_group_position_float;
        area_group_position_float.x = area_x;
        area_group_position_float.y = area_y;
        area_group_position_float.x /= this->area_group_size.x;
        area_group_position_float.y /= this->area_group_size.y;

        if(area_group_position_float.x < 0)
            area_group_position.x -= 1;
        if(area_group_position_float.y < 0)
            area_group_position.y -= 1;

        return area_group_position;
    }

    void Object::update(){
        sf::Vector2f player_pos;
        sf::Vector2i player_area;

        player_pos = this->get_player_map_pos();

        player_area = this->get_area_position(player_pos.x, player_pos.y);

        sf::Vector2i area_rect_size;
        area_rect_size.x = 32*this->scale.x*this->area_size.x;
        area_rect_size.y = 32*this->scale.y*this->area_size.y;

        sf::Vector2f player_area_window_position;
        player_area_window_position.x = area_rect_size.x;
        player_area_window_position.y = area_rect_size.y;

        // Add all area in generation
        if(this->is_player_changed_area()){
            this->old_player_area = player_area;
            this->update_areas();
            
            Map::Area* area = this->get_player_area();
            
            if(!area->is_configurated())
                area->configure_with(this->noise_target);

            StructureManager::set_all_points_to(this->get_all_areas_position());
        }

        int x_window = 0;
        int y_window = 0;
        
        for(auto area : this->all_working_areas){
            sf::Vector2f window_position;
            window_position.x = x_window*area_rect_size.x;
            window_position.y = y_window*area_rect_size.y;
        
            area->set_window_position_to(window_position);

            x_window++;
            if(area->get_position().x >= (player_area.x + this->terrain_generation_size.x)){
                x_window = 0;
                y_window++;
            }

            if(!area->is_configurated()){
                area->configure_with(this->noise_target);
                break;
            }
            else if(StructureManager::is_structure(Main::get_pair2i_of(area->get_position()))){
                Structure::Instance& structure = StructureManager::get_structure(Main::get_pair2i_of(area->get_position()));
                if(!structure.contains_walls)
                    Structure::set_walls_of(structure);
                if(structure.contain.roof == nullptr)
                    Structure::set_roof_of(structure);
            }
        }
    }

    void Object::set_elements_drawing_order(){
        if(this->sorting_elements){
            this->sorted_elements_to_draw.clear();
            for(auto entity : Entity::container)
                this->sorted_elements_to_draw.push_back(&*entity);
            for(auto tomb : Tomb::container)
                this->sorted_elements_to_draw.push_back(&*tomb);
            std::vector<Structure::Instance> all_structs = StructureManager::get_all();
            for(auto structure : all_structs){
                if(std::find(this->all_struct_contain_to_draw.begin(), this->all_struct_contain_to_draw.end(), structure.position) != this->all_struct_contain_to_draw.end()){
                    for(auto wall : structure.contain.walls)
                        this->sorted_elements_to_draw.push_back(wall);
                }
            }

            MapElement::Help::sort_by_positions(this->sorted_elements_to_draw);
            for(auto structure : all_structs){
                if(std::find(this->all_struct_roof_to_draw.begin(), this->all_struct_roof_to_draw.end(), structure.position) != this->all_struct_roof_to_draw.end() &&
                    std::find(this->all_struct_contain_to_draw.begin(), this->all_struct_contain_to_draw.end(), structure.position) == this->all_struct_contain_to_draw.end()){
                    if(structure.contain.roof != nullptr)
                        this->sorted_elements_to_draw.push_back(structure.contain.roof);
                }
            }
            this->elements_to_draw = this->sorted_elements_to_draw;

            this->sorting_elements = false;
        }
    }

    void Object::draw(){
        for(auto area : this->all_working_areas)
            area->draw_to(this);
        
        if(!this->sorting_elements)
            for(int i = 0; i < this->sorted_elements_to_draw.size() && !this->sorting_elements; i++)
                this->sorted_elements_to_draw[i]->draw_to(this);
        if(this->sorting_elements)
            for(int i = 0; i < this->elements_to_draw.size(); i++)
                this->elements_to_draw[i]->draw_to(this);
              
        this->sorting_elements = true;
        Bullet::draw_all_bullets_to(this);
    }

    void Object::update_areas(){
        sf::Vector2f player_pos;
        sf::Vector2i player_area;

        player_pos = this->get_player_map_pos();

        player_area = this->get_area_position(player_pos.x, player_pos.y);

        sf::Vector2i area_rect_size;
        area_rect_size.x = 32*this->scale.x*this->area_size.x;
        area_rect_size.y = 32*this->scale.y*this->area_size.y;

        sf::Vector2i gen = this->terrain_generation_size;
        std::vector<Area*> working_areas;

        working_areas.clear();
        this->all_working_areas.clear();
        this->all_struct_roof_to_draw.clear();
        for(float y = player_area.y - gen.y, y_window = 0; y <= player_area.y + gen.y; y++, y_window++){
            for(float x = player_area.x - gen.x, x_window = 0; x <= player_area.x + gen.x; x++, x_window++){
                Area* area = this->get_area_at(x, y);

                sf::Vector2f window_position;
                window_position.x = x_window*area_rect_size.x;
                window_position.y = y_window*area_rect_size.y;

                area->set_window_position_to(window_position);
                working_areas.push_back(area);

                this->all_struct_roof_to_draw.push_back({int(x), int(y)});
            }
        }
        this->all_working_areas = working_areas;
        
        gen = this->event_updating_size;
        this->all_struct_contain_to_draw.clear();
        this->all_struct_contain_to_draw.push_back(player_area);
        bool skip = true;
        for(float y = player_area.y - gen.y, y_window = 0; y <= player_area.y + gen.y; y++, y_window++){
            for(float x = player_area.x - gen.x, x_window = 0; x <= player_area.x + gen.x; x++, x_window++){
                Area* area = this->get_area_at(x, y);
                this->all_updating_areas.push_back(area);
                if(!skip)
                    this->all_struct_contain_to_draw.push_back(area->get_position());
                skip = !skip;
            }
        }
    }

    Area* Object::configure_area(int x, int y, sf::Vector2f window_position){
        Area* area_ptr = this->get_area_at(x, y);

        sf::Vector2i area_group_position;
        area_group_position.x = x/this->area_group_size.x;
        area_group_position.y = y/this->area_group_size.y;

        if(area_ptr == nullptr){
            if(this->get_area_group(area_group_position) == nullptr){
                this->new_area_group(area_group_position);
            }
            area_ptr = this->get_area_group(area_group_position)->add_area(x, y);
        }
        area_ptr->set_window_position_to(window_position);
        return area_ptr;
    }

    Area* Object::configure_area(sf::Vector2i map_position, sf::Vector2f window_position){
        return this->configure_area(map_position.x, map_position.y, window_position);
    }

    void Object::reduce_scale(){
        this->scale.x /= 2;
        this->scale.y /= 2;
        //if(this->scale.x < 1)
        //    this->scale.x = 1;
        //if(this->scale.y < 1)
        //    this->scale.y = 1;
    }

    void Object::increase_scale(){
        this->scale.x *= 2;
        this->scale.y *= 2;
        //if(this->scale.x > 4)
        //    this->scale.x = 4;
        //if(this->scale.y > 4)
        //    this->scale.y = 4;
    }

    void Object::clear(){
        this->elements_to_draw.clear();
        this->sorted_elements_to_draw.clear();
        for(auto& working_area : this->all_working_areas)
            working_area = nullptr;
        this->all_working_areas.clear();
        for(auto& working_area : this->all_updating_areas)
            working_area = nullptr;
        this->all_updating_areas.clear();
        for(auto& area_group : this->all_areas_group){
            area_group->clear();
            delete area_group;
        }
        this->all_areas_group.clear();
        this->all_struct_contain_to_draw.clear();
        this->all_struct_roof_to_draw.clear();
    }

    void Object::save_to(std::string save_file_location){
        std::fstream save_file;

        std::string map_name = "";
        for(auto map : all_maps){
            if(&*map.second == &*this){
                map_name = map.first;
                break;
            }
        }
        if(!std::filesystem::exists(save_file_location + "World"))
            std::filesystem::create_directory(save_file_location + "World");
        if(!std::filesystem::exists(save_file_location + "World/" + map_name))
            std::filesystem::create_directory(save_file_location + "World/" + map_name);
        if(!std::filesystem::exists(save_file_location + "World/" + map_name + "/" + map_name + ".settings")){
            save_file.open(save_file_location + "World/" + map_name + "/" + map_name + ".settings", std::ios::out);
            save_file.close();
        }
        save_file.open(save_file_location + "World/" + map_name + "/"+ map_name + ".settings", std::ios_base::binary | std::ios_base::out);
        if(save_file.is_open()){
            float scale_x = this->scale.x;
            float scale_y = this->scale.y;
            save_file.write(reinterpret_cast<char*>(&scale_x), sizeof(float));
            save_file.write(reinterpret_cast<char*>(&scale_y), sizeof(float));
            
            save_file.close();
        }
        for(auto area_group : this->all_areas_group){
            for(auto area : area_group->get_all_areas()){
                if(area != nullptr)
                    area->save_to(save_file_location + "World/" + map_name + "/Areas/");
            }
        }
    }

    bool Object::load_from(std::string load_location, std::string map_name){
        if(!std::filesystem::exists(load_location + "World/" + map_name + "/" + map_name + ".settings"))
            return false;
        else{
            std::fstream file;
            file.open(load_location + "World/" + map_name + "/" + map_name + ".settings", std::ios::in | std::ios::binary);
            if(file.is_open()){
                this->noise_target = PerlinNoise::get(map_name);

                file.read(reinterpret_cast<char*>(&this->scale.x), sizeof(float));
                file.read(reinterpret_cast<char*>(&this->scale.y), sizeof(float));

                this->area_size = {18, 18};
                this->area_group_size = {10, 10};
                
                file.close();
            }
            std::list<std::string> all_stored_areas;
            Main::get_files_in(load_location + "World/" + map_name + "/Areas/", all_stored_areas);
            for(std::string area_file_name : all_stored_areas){
                Area* area_to_load = new Area(sf::Vector2i(0, 0), sf::Vector2i(0, 0));
                area_to_load->load_from(load_location + "World/" + map_name + "/Areas/" + area_file_name);
                this->new_area(area_to_load);
            }
            WorldSettingsWindow::Settings settings = WorldSettingsWindow::get_settings_from(load_location);
            this->terrain_generation_size = {settings.terrain_generation_size, settings.terrain_generation_size};
            this->event_updating_size = {settings.zone_update_size, settings.zone_update_size};
            
            return true;
        }
    }

    bool Object::is_updated(){
        bool updated = true;
        if(this->all_working_areas.empty() == false){
            for(int i = 0; i < this->all_working_areas.size(); i++){
                Area* area = &*this->all_working_areas[i];
                if(!area->is_configurated()){
                    updated = false;
                    break;
                }
            }
        }
        else{
            updated = false;
        }
        return updated;
    }

    bool Object::is_working(Area* area){
        if(std::find(this->all_working_areas.begin(), this->all_working_areas.end(), area) != this->all_working_areas.end())
            return true;
        else
            return false;
    }

    bool Object::is_for_render(Area* area){
        if(std::find(this->all_updating_areas.begin(), this->all_updating_areas.end(), area) != this->all_updating_areas.end())
            return true;
        else
            return false;
    }

    bool Object::is_player_changed_area(){
        sf::Vector2f player_position = this->get_player_map_pos();
        sf::Vector2i player_area = this->get_area_position(player_position.x, player_position.y);

        if(player_area.x != this->old_player_area.x || player_area.y != this->old_player_area.y)
            return true;
        else
            return false;
    }

    bool can_move_on(MapElement::WorldContent element){
        bool can_move = true;
        switch(element){
            case MapElement::WorldContent::walls:
                can_move = false;
                break;
        }
        return can_move;
    }

    void create(std::string map_name, sf::RenderWindow &window, PerlinNoise::Object* noise, sf::Vector2i area_size, sf::Vector2i area_group_size, sf::Vector2f scale, sf::Vector2i terrain_generation_size, sf::Vector2i update_radius){
        if(!exists(map_name))
            all_maps[map_name] = new Object(window, *noise, area_size, area_group_size, scale, terrain_generation_size, update_radius);
    }

    void remove(std::string map_name){
        if(exists(map_name)){
            all_maps[map_name]->clear();
            delete all_maps.at(map_name);
            all_maps.erase(map_name);
        }
    }

    bool exists(std::string map_name){
        bool exists = false;
        if(all_maps.empty() == false){
            for(auto map : all_maps){
                if(map.first == map_name){
                    exists = true;
                    break;
                }
            }
        }
        return exists;
    }

    Object* get(std::string map_name){
        if(exists(map_name))
            return &*all_maps[map_name];
        else
            return nullptr;
    }

    sf::Vector2i get_spawn_area(int seed){
        sf::Vector2i area_position;
        area_position.x = (seed & 255) % 100;
        area_position.y = (seed & (255 << 2)) % 100;

        return area_position;
    }

    void clear(){
        Debugging::write("Clearing map initialised");
        if(all_maps.empty() == false){
            for(auto& map : all_maps){
                map.second->clear();
                delete map.second;
            }
        }
        all_maps.clear();
        Debugging::write("Clearing map terminated");
    }

    void save_all(std::string save_file_location){
        for(auto map : all_maps){
            map.second->save_to(save_file_location);
        }
    }

    bool load_from(std::string file_path, std::string map_name, sf::RenderWindow &window){
        if(exists(map_name))
            return false;
        else{
            if(std::filesystem::exists(file_path + "World/" + map_name + "/"+ map_name + ".settings")){
                Map::Object* map = new Map::Object(window);
                map->load_from(file_path, map_name);
                all_maps[map_name] = map;
                return true;
            }
            else
                return false;
        }
    }

}