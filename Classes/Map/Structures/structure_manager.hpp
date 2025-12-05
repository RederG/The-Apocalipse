#ifndef __STRUCTURE_MANAGER__
    #define __STRUCTURE_MANAGER__

    #include "../../../main.hpp"
    #include "../map_element.hpp"
    #include "structure.hpp"

    namespace Map{
        class Area;
    };

    namespace Structure{
        struct Instance;
    };

    namespace StructureManager{
        enum NoiseParts{
            empty, struct_spawn_points, biome_house
        };

        const std::string NOISE_NAME_STRUCT_SPAWN = "Structure_spawn";
        const std::string NOISE_NAME_BIOME = "Biome";
        const std::string NOISE_NAME_ENTRY_POS_UP = "Entry_pos_up";
        const std::string NOISE_NAME_ENTRY_POS_DOWN = "Entry_pos_down";
        const std::string NOISE_NAME_ENTRY_POS_RIGHT = "Entry_pos_right";
        const std::string NOISE_NAME_ENTRY_POS_LEFT = "Entry_pos_left";
        
        MapElement::WorldContent get_environment_at(unsigned int x, unsigned int y, sf::Image image);

        bool is_structure(std::pair<int, int> point);

        bool search_finished();

        bool is_actived();

        bool is_walls_seen(std::pair<int, int> point);

        bool is_perimeter(std::pair<int, int> point);

        Structure::Instance& get_structure(std::pair<int, int> position, bool is_new_structure = false);

        Structure::Instance& get_structure(sf::Vector2i position, bool is_new_structure = false);

        std::vector<Structure::Instance> get_all();

        void set_up(long long default_seed);

        void search_structures();

        void reset();

        void set_all_points_to(std::vector<std::pair<int, int>> all_points);
    };

#endif