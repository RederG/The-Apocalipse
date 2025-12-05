#include "structure_manager.hpp"
#include "../../../Help/thread_manager.hpp"
#include "../../../Help/the_apocalipse.hpp"

namespace StructureManager{
    enum SearchState{
        Stopped, Searching_spawn_point, Building_structs, Verifying_structs, Adding_new_structs
    };

    SearchState search_state = Searching_spawn_point;

    std::vector<Structure::Instance> all_structures;
    std::vector<Structure::Instance> all_new_structures;
    std::vector<std::pair<int, int>> all_structures_position;
    std::vector<std::pair<int, int>> all_new_structures_position;
    std::vector<std::pair<int, int>> all_points;
    std::vector<std::pair<int, int>> all_spawn_points;
    std::vector<std::pair<int, int>> all_visited_point;
    std::vector<std::pair<int, int>> all_struct_position_to_check;
    std::vector<std::pair<int, int>> perimeter_points;
    std::vector<std::pair<int, int>> walls_seen_points;

    std::map<std::pair<int, int>, std::pair<int, int>> parent_points_position;

    std::pair<int, int> struct_spawn_point;

    bool search_terminated = false;
    bool actived = false;
    bool paused = false;

    namespace Counter{
        int point = 0, struct_to_be_valid = 0, new_struct = 0;
    };

    bool is_structure(std::pair<int, int> point){
        if(std::find(all_structures_position.begin(), all_structures_position.end(), point) != all_structures_position.end())
            return true;
        return false;
    }

    bool is_actived(){
        return actived;
    }

    bool is_walls_seen(std::pair<int, int> point){
        if(std::find(walls_seen_points.begin(), walls_seen_points.end(), point) != walls_seen_points.end())
            return true;
        return false;
    }

    bool is_new_structure(std::pair<int, int> point){
        if(std::find(all_new_structures_position.begin(), all_new_structures_position.end(), point) != all_new_structures_position.end())
            return true;
        return false;
    }

    bool search_finished(){
        return search_terminated;
    }

    bool is_spawn_point(std::pair<int, int> point){
        if(std::find(all_spawn_points.begin(), all_spawn_points.end(), point) != all_spawn_points.end())
            return true;
        return false;
    }

    bool make_link(Structure::Instance& structure, Structure::Instance parent_structure){
        bool link_created = false;
        if(structure.position.x < parent_structure.position.x && parent_structure.sides.left.position != Structure::EntryPosition::none){
            structure.sides.right.position = parent_structure.sides.left.position;
            structure.sides.right.big_entry = parent_structure.sides.left.big_entry;
            link_created = true;
        }
        else if(structure.position.x > parent_structure.position.x && parent_structure.sides.right.position != Structure::EntryPosition::none){
            structure.sides.left.position = parent_structure.sides.right.position;
            structure.sides.left.big_entry = parent_structure.sides.right.big_entry;
            link_created = true;
        }
        if(structure.position.y < parent_structure.position.y && parent_structure.sides.up.position != Structure::EntryPosition::none){
            structure.sides.down.position = parent_structure.sides.up.position;
            structure.sides.down.big_entry = parent_structure.sides.up.big_entry;
            link_created = true;
        }
        else if(structure.position.y > parent_structure.position.y && parent_structure.sides.down.position != Structure::EntryPosition::none){
            structure.sides.up.position = parent_structure.sides.down.position;
            structure.sides.up.big_entry = parent_structure.sides.down.big_entry;
            link_created = true;
        }
        return link_created;
    }
    
    bool is_visited(std::pair<int, int> point){
        if(std::find(all_visited_point.begin(), all_visited_point.end(), point) != all_visited_point.end())
            return true;
        else
            return false;
    }

    bool is_perimeter(std::pair<int, int> point){
        if(std::find(perimeter_points.begin(), perimeter_points.end(), point) != perimeter_points.end())
            return true;
        else
            return false;
    }

    MapElement::WorldContent get_environment_at(unsigned int x, unsigned int y, sf::Image image){
        MapElement::WorldContent environment = MapElement::WorldContent::nothing;
        sf::Vector2u img_size = image.getSize();
        if(img_size.x > x && img_size.y > y){
            sf::Color pixel = image.getPixel({x, y});
            if(pixel == sf::Color::Black || pixel == sf::Color::Blue)
                environment = MapElement::WorldContent::walls;
            if(pixel == sf::Color(100, 100, 100))
                environment = MapElement::WorldContent::nothing;
        }
        return environment;
    }

    Structure::Instance& get_structure(std::pair<int, int> position, bool is_new_structure){
        int struct_location = 0;
        for(int i = 0; i < all_structures.size() && !is_new_structure; i++){
            if(Main::get_pair2i_of(all_structures[i].position) == position){
                struct_location = i;
                break;
            }
        }
        for(int i = 0; i < all_new_structures.size() && is_new_structure; i++){
            if(Main::get_pair2i_of(all_new_structures[i].position) == position){
                struct_location = i;
                break;
            }
        }
        if(!is_new_structure)
            return all_structures[struct_location];
        else
            return all_new_structures[struct_location];
    }

    Structure::Instance& get_structure(sf::Vector2i position, bool is_new_structure){
        return get_structure(Main::get_pair2i_of(position), is_new_structure);
    }

    std::vector<Structure::Instance> get_all(){
        return all_structures;
    }

    sf::Color get_color_of(NoiseParts noise_part){
        sf::Color color = sf::Color::White;
        switch(noise_part){
            case NoiseParts::biome_house:
                color = sf::Color(100, 100, 100);
                break;
            case NoiseParts::struct_spawn_points:
                color = sf::Color(255, 0, 0);
                break;
            case NoiseParts::empty:
                color = sf::Color::White;
            default:
                break;
        }
        return color;
    }

    NoiseParts get_noise_part_of(sf::Color color){
        if(color == get_color_of(NoiseParts::biome_house))
            return NoiseParts::biome_house;
        else if(color == get_color_of(NoiseParts::struct_spawn_points))
            return NoiseParts::struct_spawn_points;
        else
            return NoiseParts::empty;
    }

    std::vector<std::pair<int, int>> get_adjacent_points_of(std::pair<int, int> point){
        std::pair<int, int> up = point, down = point, left = point, right = point;
        up.second -= 1;
        down.second += 1;
        left.first -= 1;
        right.first += 1;
        return std::vector<std::pair<int, int>>{up, down, right, left};
    }
    
    std::vector<std::pair<int, int>> get_adjacent_points_of(std::pair<int, int> point, Structure::Sides sides){
        std::pair<int, int> up = point, down = point, left = point, right = point;
        up.second -= 1;
        down.second += 1;
        left.first -= 1;
        right.first += 1;

        std::vector<std::pair<int, int>> adj_points;

        if(sides.up.position != Structure::EntryPosition::none)
            adj_points.push_back(up);
        if(sides.down.position != Structure::EntryPosition::none)
            adj_points.push_back(down);
        if(sides.left.position != Structure::EntryPosition::none)
            adj_points.push_back(left);
        if(sides.right.position != Structure::EntryPosition::none)
            adj_points.push_back(right);
        return adj_points;
    }

    std::vector<Structure::Instance> get_adjacent_struct_of(Structure::Instance structure, bool is_new){
        std::vector<Structure::Instance> all_adj_struct;
        std::vector<std::pair<int, int>> adj_points = get_adjacent_points_of({structure.position.x, structure.position.y});
        for(int i = 0; i < adj_points.size(); i++){
            if(!is_new && is_structure(adj_points[i]))
                all_adj_struct.push_back(get_structure(adj_points[i], !is_new));
            else if(is_new && is_new_structure(adj_points[i]))
                all_adj_struct.push_back(get_structure(adj_points[i], is_new));
        }
        return all_adj_struct;
    }

    void set_up(long long default_seed){
        all_structures.clear();
        all_structures_position.clear();
        all_points.clear();
        all_spawn_points.clear();
        all_visited_point.clear();

        if(PerlinNoise::exists(NOISE_NAME_STRUCT_SPAWN))
            PerlinNoise::remove(NOISE_NAME_STRUCT_SPAWN);
        PerlinNoise::new_noise(NOISE_NAME_STRUCT_SPAWN, 1, 1, 3, Main::get_seed(3332, 55201, default_seed));

        if(PerlinNoise::exists(NOISE_NAME_BIOME))
            PerlinNoise::remove(NOISE_NAME_BIOME);
        PerlinNoise::new_noise(NOISE_NAME_BIOME, 1, 1, 8, Main::get_seed(4512105, 895127, default_seed));

        if(PerlinNoise::exists(NOISE_NAME_ENTRY_POS_UP))
            PerlinNoise::remove(NOISE_NAME_ENTRY_POS_UP);
        PerlinNoise::new_noise(NOISE_NAME_ENTRY_POS_UP, 1, 1, 8, Main::get_seed(215, 21206, default_seed));

        if(PerlinNoise::exists(NOISE_NAME_ENTRY_POS_DOWN))
            PerlinNoise::remove(NOISE_NAME_ENTRY_POS_DOWN);
        PerlinNoise::new_noise(NOISE_NAME_ENTRY_POS_DOWN, 1, 1, 8, Main::get_seed(159845, 30325, default_seed));

        if(PerlinNoise::exists(NOISE_NAME_ENTRY_POS_LEFT))
            PerlinNoise::remove(NOISE_NAME_ENTRY_POS_LEFT);
        PerlinNoise::new_noise(NOISE_NAME_ENTRY_POS_LEFT, 1, 1, 8, Main::get_seed(444441, 510268, default_seed));

        if(PerlinNoise::exists(NOISE_NAME_ENTRY_POS_RIGHT))
            PerlinNoise::remove(NOISE_NAME_ENTRY_POS_RIGHT);
        PerlinNoise::new_noise(NOISE_NAME_ENTRY_POS_RIGHT, 1, 1, 8, Main::get_seed(7154562, 996214, default_seed));

        PerlinNoise::get(NOISE_NAME_BIOME)->configure_color(PerlinNoise::Interval(100, 255), get_color_of(NoiseParts::biome_house));
        PerlinNoise::get(NOISE_NAME_BIOME)->configure_color(PerlinNoise::Interval(0, 100), get_color_of(NoiseParts::empty));

        PerlinNoise::get(NOISE_NAME_STRUCT_SPAWN)->configure_color(PerlinNoise::Interval(0, 85), get_color_of(NoiseParts::empty));
        PerlinNoise::get(NOISE_NAME_STRUCT_SPAWN)->configure_color(PerlinNoise::Interval(85, 95), get_color_of(NoiseParts::struct_spawn_points));
        PerlinNoise::get(NOISE_NAME_STRUCT_SPAWN)->configure_color(PerlinNoise::Interval(95, 105), get_color_of(NoiseParts::empty));
        PerlinNoise::get(NOISE_NAME_STRUCT_SPAWN)->configure_color(PerlinNoise::Interval(105, 110), get_color_of(NoiseParts::struct_spawn_points));
        PerlinNoise::get(NOISE_NAME_STRUCT_SPAWN)->configure_color(PerlinNoise::Interval(110, 170), get_color_of(NoiseParts::empty));
        PerlinNoise::get(NOISE_NAME_STRUCT_SPAWN)->configure_color(PerlinNoise::Interval(170, 255), get_color_of(NoiseParts::struct_spawn_points));

        Counter::point = 0;
        search_terminated = false;
    }

    void get_all_spawn_points_from(std::pair<int, int> point){
        all_spawn_points.clear();
        all_spawn_points.push_back(point);

        std::vector<std::pair<int, int>> points_to_verify{point};
        while(!points_to_verify.empty() && TheApocalipse::is_running()){
            std::pair<int, int> front_point = points_to_verify.front();
            std::vector<std::pair<int, int>> adj_points = get_adjacent_points_of(front_point); 
            for(std::pair<int, int> adj_point : adj_points){
                sf::Color color = get_color_of(NoiseParts::empty);
                if(PerlinNoise::get(NOISE_NAME_STRUCT_SPAWN) != nullptr)
                    color = PerlinNoise::get(NOISE_NAME_STRUCT_SPAWN)->get_color_at(adj_point.first, adj_point.second);
                if(get_noise_part_of(color) == NoiseParts::struct_spawn_points && !is_spawn_point(adj_point)){
                    all_spawn_points.push_back(adj_point);
                    points_to_verify.push_back(adj_point);
                }
            }
            points_to_verify.erase(points_to_verify.begin());
        }
    }

    void set_perimeter_from(std::vector<std::vector<std::pair<int, int>>> sorted_points){
        perimeter_points.clear();
        for(int i = 0; i < sorted_points.size(); i++){
            for(int j = 0; j < sorted_points[i].size(); j++){
                if(i == 0 || i == sorted_points.size() - 1 || j == 0 || j == sorted_points[i].size() - 1)
                    perimeter_points.push_back(sorted_points[i][j]);

                std::pair<int, int> down_point = {sorted_points[i][j].first, sorted_points[i][j].second + 1};
                std::pair<int, int> up_point = {sorted_points[i][j].first, sorted_points[i][j].second - 1};
                if((i + 1) < sorted_points.size() && (i - 1) >= 0){
                    if(std::find(sorted_points[i + 1].begin(), sorted_points[i + 1].end(), down_point) == sorted_points[i + 1].end())
                        perimeter_points.push_back(sorted_points[i][j]);
                    if(std::find(sorted_points[i - 1].begin(), sorted_points[i - 1].end(), up_point) == sorted_points[i - 1].end())
                        perimeter_points.push_back(sorted_points[i][j]);
                }
            }
        }
    }

    void set_walls_seen_points(){
        for(int i = 0; i < perimeter_points.size(); i++){
            std::pair<int, int> perimeter = perimeter_points[i];
            if(!is_new_structure({perimeter.first, perimeter.second + 1}))
                walls_seen_points.push_back(perimeter);
        }
    }

    void sort_points(std::vector<std::pair<int, int>>& points){
        std::vector<std::pair<int, int>> first_sort_vector = points;
        // Sorting with the Y coodinates
        for(int i = 0; i < first_sort_vector.size(); i++){
            for(int j = 0; j < first_sort_vector.size(); j++){
                std::pair<int, int> element = first_sort_vector[j];
                int correct_position = 0;
                for(int k = 1; k < first_sort_vector.size(); k++){
                    if(element.second >= first_sort_vector[k].second)
                        correct_position = k;
                }
                first_sort_vector.erase(first_sort_vector.begin() + j);
                first_sort_vector.insert(first_sort_vector.begin() + correct_position, element);
            }
        }

        std::vector<std::vector<std::pair<int, int>>> second_sort_vector;
        std::vector<std::pair<int, int>> temporary_vector;
        int y = 0;
        // Preparing sorting with the X coordinates
        for(int i = 0; i < first_sort_vector.size(); i++){
            if(y != first_sort_vector[i].second && !temporary_vector.empty()){
                second_sort_vector.push_back(temporary_vector);
                temporary_vector.clear();
            }
            y = first_sort_vector[i].second;
            temporary_vector.push_back(first_sort_vector[i]);
        }
        second_sort_vector.push_back(temporary_vector);

        // Sorting with the X coordinates
        for(int i = 0; i < second_sort_vector.size(); i++){
            for(int j = 0; j < second_sort_vector[i].size(); j++){
                for(int k = 0; k < second_sort_vector[i].size(); k++){
                    std::pair<int, int> element = second_sort_vector[i][k];
                    int correct_position = 0;
                    for(int l = 1; l < second_sort_vector[i].size(); l++){
                        if(element.first >= second_sort_vector[i][l].first)
                            correct_position = l;
                    }
                    second_sort_vector[i].erase(second_sort_vector[i].begin() + k);
                    second_sort_vector[i].insert(second_sort_vector[i].begin() + correct_position, element);
                }
            }
        }

        // Setting the sorted spawn points vector
        points.clear();
        set_perimeter_from(second_sort_vector);
        for(int i = 0; i < second_sort_vector.size(); i++){
            for(int j = 0; j < second_sort_vector[i].size(); j++)
                points.push_back(second_sort_vector[i][j]);
        }
    }

    Structure::Instance build_struct_at(std::pair<int, int> position, Structure::Instance parent_structure){
        Structure::Instance structure;
        Structure::Sides sides;
        sides.up.position = Structure::EntryPosition(int(PerlinNoise::get(NOISE_NAME_ENTRY_POS_UP)->get_color_at(position.first, position.second).r%5));
        sides.down.position = Structure::EntryPosition(int(PerlinNoise::get(NOISE_NAME_ENTRY_POS_DOWN)->get_color_at(position.first, position.second).r%5));
        sides.left.position = Structure::EntryPosition(int(PerlinNoise::get(NOISE_NAME_ENTRY_POS_LEFT)->get_color_at(position.first, position.second).r%5));
        sides.right.position = Structure::EntryPosition(int(PerlinNoise::get(NOISE_NAME_ENTRY_POS_RIGHT)->get_color_at(position.first, position.second).r%5));
        
        if(is_perimeter(position)){
            if(sides.up.position == Structure::EntryPosition::full)
                sides.up.position = Structure::EntryPosition::middle;
            if(sides.down.position == Structure::EntryPosition::full)
                sides.down.position = Structure::EntryPosition::middle;
            if(sides.left.position == Structure::EntryPosition::full)
                sides.left.position = Structure::EntryPosition::middle;
            if(sides.right.position == Structure::EntryPosition::full)
                sides.right.position = Structure::EntryPosition::middle;
        }

        sides.up.big_entry = bool(PerlinNoise::get(NOISE_NAME_ENTRY_POS_UP)->get_color_at(position.first, position.second).r%2);
        sides.down.big_entry = bool(PerlinNoise::get(NOISE_NAME_ENTRY_POS_DOWN)->get_color_at(position.first, position.second).r%2);
        sides.left.big_entry = bool(PerlinNoise::get(NOISE_NAME_ENTRY_POS_LEFT)->get_color_at(position.first, position.second).r%2);
        sides.right.big_entry = bool(PerlinNoise::get(NOISE_NAME_ENTRY_POS_RIGHT)->get_color_at(position.first, position.second).r%2);
        
        structure.position = Main::get_vector2i_of(position);
        structure.sides = sides;
        structure.contains_doors = !parent_structure.contains_doors;

        if(!is_new_structure(position) && make_link(structure, parent_structure) && !is_visited(position)){
            all_new_structures.push_back(structure);
            all_new_structures_position.push_back(position);
            all_visited_point.push_back(position);
        }
        return structure;
    }

    void build_all_structs(){
        if(ThreadManager::get_state_of(ThreadManager::Thread::structure_search) == ThreadManager::State::Actived){
            if(all_struct_position_to_check.empty()){
                all_struct_position_to_check.push_back(struct_spawn_point);
                all_new_structures.clear();
                all_new_structures_position.clear();
                parent_points_position.clear();
            }

            if(!all_struct_position_to_check.empty() && TheApocalipse::is_running() && 
                    ThreadManager::get_state_of(ThreadManager::Thread::structure_search) == ThreadManager::Actived){
                if(all_struct_position_to_check.front() == struct_spawn_point){
                    Structure::Sides sides;
                    sides.up.position = Structure::EntryPosition(int(PerlinNoise::get(NOISE_NAME_ENTRY_POS_UP)->get_color_at(struct_spawn_point.first, struct_spawn_point.second).r%5));
                    sides.down.position = Structure::EntryPosition(int(PerlinNoise::get(NOISE_NAME_ENTRY_POS_DOWN)->get_color_at(struct_spawn_point.first, struct_spawn_point.second).r%5));
                    sides.left.position = Structure::EntryPosition(int(PerlinNoise::get(NOISE_NAME_ENTRY_POS_LEFT)->get_color_at(struct_spawn_point.first, struct_spawn_point.second).r%5));
                    sides.right.position = Structure::EntryPosition(int(PerlinNoise::get(NOISE_NAME_ENTRY_POS_RIGHT)->get_color_at(struct_spawn_point.first, struct_spawn_point.second).r%5));

                    Structure::Instance first_structure;
                    first_structure.position = Main::get_vector2i_of(struct_spawn_point);
                    first_structure.sides = sides;
                    first_structure.contains_doors = false;

                    if(!is_structure(struct_spawn_point) && !is_visited(struct_spawn_point)){
                        all_new_structures.push_back(first_structure);
                        all_new_structures_position.push_back(struct_spawn_point);
                        all_visited_point.push_back(struct_spawn_point);
                    }

                    all_struct_position_to_check.erase(all_struct_position_to_check.begin());
                    for(std::pair<int, int> point : get_adjacent_points_of(struct_spawn_point, first_structure.sides)){
                        if(is_spawn_point(point)){
                            all_struct_position_to_check.push_back(point);
                            parent_points_position[point] = struct_spawn_point;
                        }
                    }
                }
                
                else{
                    std::pair<int, int> point = all_struct_position_to_check.front();

                    Structure::Instance new_struct = build_struct_at(point, get_structure(Main::get_vector2i_of(parent_points_position[point]), true));
                    
                    for(std::pair<int, int> adj_point : get_adjacent_points_of(point, new_struct.sides)){
                        if(is_spawn_point(adj_point) && !is_new_structure(adj_point) && !is_visited(adj_point)){
                            all_struct_position_to_check.push_back(adj_point);
                            parent_points_position[adj_point] = point;
                        }
                    }
                    all_struct_position_to_check.erase(all_struct_position_to_check.begin());
                }
            }
        }
        if(all_struct_position_to_check.empty()){
            Debugging::write("(Structure manager) - ****VERIFYING STRUCTS****\n", "../algo.log", true, true);
            sort_points(all_new_structures_position);
            set_walls_seen_points();
            search_state = Verifying_structs;
        }
    }
    
    void verify_structs(){
        int i = Counter::struct_to_be_valid;
        std::vector<Structure::Instance> adj_structs = get_adjacent_struct_of(all_new_structures[i], true);
        for(int j = 0; j < adj_structs.size() && TheApocalipse::is_running(); j++){
            if(all_new_structures[i].position.x < adj_structs[j].position.x && all_new_structures[i].sides.right.position != adj_structs[j].sides.left.position){
                all_new_structures[i].sides.right.position = Structure::EntryPosition::none;
                adj_structs[j].sides.left.position = Structure::EntryPosition::none;
            }
            else if(all_new_structures[i].position.x > adj_structs[j].position.x && all_new_structures[i].sides.left.position != adj_structs[j].sides.right.position){
                all_new_structures[i].sides.left.position = Structure::EntryPosition::none;
                adj_structs[j].sides.right.position = Structure::EntryPosition::none;
            }
            if(all_new_structures[i].position.y < adj_structs[j].position.y && all_new_structures[i].sides.down.position != adj_structs[j].sides.up.position){
                all_new_structures[i].sides.down.position = Structure::EntryPosition::none;
                adj_structs[j].sides.up.position = Structure::EntryPosition::none;
            }
            else if(all_new_structures[i].position.y > adj_structs[j].position.y && all_new_structures[i].sides.up.position != adj_structs[j].sides.down.position){
                all_new_structures[i].sides.up.position = Structure::EntryPosition::none;
                adj_structs[j].sides.down.position = Structure::EntryPosition::none;
            }
        }
        sf::Vector2i struct_pos = all_new_structures[i].position;
        if(!is_new_structure({struct_pos.x, struct_pos.y - 1}) && all_new_structures[i].sides.up.position == Structure::EntryPosition::full)
            all_new_structures[i].sides.up.position = Structure::EntryPosition::middle;
        if(!is_new_structure({struct_pos.x, struct_pos.y + 1}) && all_new_structures[i].sides.down.position == Structure::EntryPosition::full)
            all_new_structures[i].sides.down.position = Structure::EntryPosition::middle;
        if(!is_new_structure({struct_pos.x - 1, struct_pos.y}) && all_new_structures[i].sides.left.position == Structure::EntryPosition::full)
            all_new_structures[i].sides.left.position = Structure::EntryPosition::middle;
        if(!is_new_structure({struct_pos.x + 1, struct_pos.y}) && all_new_structures[i].sides.right.position == Structure::EntryPosition::full)
            all_new_structures[i].sides.right.position = Structure::EntryPosition::middle;

        Counter::struct_to_be_valid++;
        if(Counter::struct_to_be_valid >= all_new_structures.size()){
            Counter::struct_to_be_valid = 0;
            Debugging::write("(Structure manager) - ****ADDING NEW STRUCTS****\n", "../algo.log", true, true);
            search_state = Adding_new_structs;
        }
    }

    void add_new_structs(){
        if(!all_new_structures.empty() && TheApocalipse::is_running() && 
                ThreadManager::get_state_of(ThreadManager::Thread::structure_search) == ThreadManager::Actived){
            sf::Vector2i position = all_new_structures[0].position;
            all_structures.push_back(all_new_structures[0]);
            all_structures_position.push_back(Main::get_pair2i_of(position));
            all_new_structures.erase(all_new_structures.begin());
            Counter::new_struct++;
        }
        else{
            Debugging::write("(Structure manager) - " + std::to_string(Counter::new_struct) + " structures has been added\n", "../algo.log", true, true);
            search_state = Searching_spawn_point;
            Counter::new_struct = 0;
            Debugging::write("(Structure manager) - ****SEARCHING SPAWN POINTS****\n", "../algo.log", true, true);
        }
    }

    void set_all_points_to(std::vector<std::pair<int, int>> all_new_points){
        search_terminated = false;
        Counter::point = 0;
        Counter::struct_to_be_valid = 0;
        search_state = Searching_spawn_point;
        all_points = all_new_points;
        Debugging::write("(Structure manager) - ****SEARCHING SPAWN POINTS****\n", "../algo.log", true, true);
    }

    void search_structures(){
        Debugging::write("(Structure manager) - Algorythm initiated", "../algo.log", true, true);
        actived = true;
        while(ThreadManager::get_state_of(ThreadManager::Thread::structure_search) != ThreadManager::State::Terminated){
            if(ThreadManager::get_state_of(ThreadManager::Thread::structure_search) == ThreadManager::Actived && !search_terminated){
                paused = false;
                if(search_state == Searching_spawn_point){
                    if(!all_points.empty() && !is_visited(all_points[Counter::point]) && !is_new_structure(all_points[Counter::point]) && !is_structure(all_points[Counter::point])){
                        std::pair<int, int> point = all_points[Counter::point];
                        if(PerlinNoise::get(NOISE_NAME_STRUCT_SPAWN) != nullptr && PerlinNoise::get(NOISE_NAME_STRUCT_SPAWN)->get_color_at(point.first, point.second) == get_color_of(NoiseParts::struct_spawn_points)){
                            Debugging::write("(Structure manager) - Point (" + std::to_string(point.first) + ", " + std::to_string(point.second) + ") : SPAWN POINT\n", "../algo.log", true, true);
                            if(TheApocalipse::is_running()){
                                Debugging::write("(Structure manager) - Getting all spawn points", "../algo.log", true, true);
                                get_all_spawn_points_from(point);
                                Debugging::write("(Structure manager) - Spawn points : " + std::to_string(all_spawn_points.size()), "../algo.log", true, true);
                            }
                            if(TheApocalipse::is_running()){
                                Debugging::write("(Structure manager) - Sorting spawn points", "../algo.log", true, true);
                                sort_points(all_spawn_points);
                            }
                            if(TheApocalipse::is_running()){
                                Debugging::write("(Structure manager) - Getting initialization spawn point", "../algo.log", true, true);
                                struct_spawn_point = all_spawn_points[(all_spawn_points.size() - 1)/2];
                                Debugging::write("(Structure manager) - Initialization spawn point : (" + std::to_string(struct_spawn_point.first) + ", " + std::to_string(struct_spawn_point.second) + ")\n", "../algo.log", true, true);
                                Debugging::write("(Structure manager) - ****BUILDING STRUCTS****\n", "../algo.log", true, true);
                                search_state = Building_structs;
                            }
                        }
                    }
                    Counter::point++;
                    if(Counter::point >= all_points.size()){
                        for(int i = 0; i < all_points.size(); i++){
                            std::pair<int, int> point = all_points[i];
                            if(!is_visited(all_points[i]))
                                all_visited_point.push_back(point);
                        }
                        search_terminated = true;
                        search_state = Stopped;
                        Debugging::write("(Structure manager) - SEARCH TERMINATED\n", "../algo.log", true, true);
                    }
                }
                if(search_state == Building_structs && TheApocalipse::is_running())
                    build_all_structs();
                if(search_state == Verifying_structs&& TheApocalipse::is_running())
                    verify_structs();
                if(search_state == Adding_new_structs&& TheApocalipse::is_running())
                    add_new_structs();
            }
            else if(ThreadManager::get_state_of(ThreadManager::Thread::structure_search) == ThreadManager::Paused && !paused){
                paused = true;
                Debugging::write("(Structure manager) - Algorythm paused", "../algo.log", true, true);
            }
        }
        Debugging::write("(Structure manager) - Terminating algorithm", "../algo.log", true, true);
        all_points.clear();
        all_visited_point.clear();
        all_spawn_points.clear();
        all_new_structures.clear();
        all_new_structures_position.clear();
        all_structures.clear();
        all_structures_position.clear();
        parent_points_position.clear();
        Debugging::write("(Structure manager) - Algorythm terminated", "../algo.log", true, true);
        actived = false;
    }

    void reset(){
        all_points.clear();
        all_visited_point.clear();
        all_spawn_points.clear();
        all_new_structures.clear();
        all_new_structures_position.clear();
        all_structures.clear();
        all_structures_position.clear();
        parent_points_position.clear();
        Counter::new_struct = 0;
        Counter::point = 0;
        Counter::struct_to_be_valid = 0;
    }

};