#ifndef __STRUCTURE__
    #define __STRUCTURE__

    #include "../../../main.hpp"
    #include "../map_manager.hpp"
    #include "../../InteractiveObjects/interactive_object.hpp"

    namespace InteractiveObjects{
        class Object;
    };

    namespace Structure{
        enum EntryPosition{
            none, left, middle, right, full
        };
        
        struct EntrySpecification{
            EntryPosition position = none;
            bool big_entry = false;
        };

        struct Sides{
            EntrySpecification up;
            EntrySpecification down;
            EntrySpecification left;
            EntrySpecification right;
        };

        struct Contain{
            std::vector<InteractiveObjects::Object*> walls;
        };

        const std::map<std::string, std::map<EntryPosition, sf::IntRect>> entry_texture{
            {"up_down", {
                {left,      {{0, 0}, {18, 1}}},
                {middle,    {{0, 1}, {18, 1}}},
                {right,     {{0, 2}, {18, 1}}},
                {none,      {{0, 6}, {18, 1}}},
            }},
            {"left_right", {
                {left,      {{0, 7}, {1, 18}}},
                {middle,    {{1, 7}, {1, 18}}},
                {right,     {{2, 7}, {1, 18}}},
                {none,      {{6, 7}, {1, 18}}},
            }}
        };

        struct Instance{
            sf::Vector2i position = {0, 0};
            Sides sides;
            Contain contain;
            bool contains_walls = false;
            bool contains_doors = false;
        };

        sf::Image create_image_of(Instance structure);

        void set_walls_of(Instance& structure);
    };

#endif