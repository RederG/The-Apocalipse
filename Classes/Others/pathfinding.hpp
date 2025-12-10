#ifndef __PATH_FINDING__
    #define __PATH_FINDING__

    #include "../../main.hpp"
    #include "../Map/map_manager.hpp"

    namespace Map{
        class Object;
    };

    // All about path finding
    namespace PathFinding{
        class A_star{
            public:
                // The constructor
                A_star();

                // Sets the start point, the finishing point and the map that is used to find the path
                void set_up(sf::Vector2f start_position, sf::Vector2f end_position, Map::Object* map);

                // Tries to find the path
                void find_path();
                
                // Verifies if the path finder found the path
                bool found_path();

                // Resets the path finder
                void reset();

                // Clears the list of paths
                void clear_path();

                // Gets the path
                std::vector<sf::Vector2f> get_path();

            private:
                sf::Vector2f start_point, end_point, true_end_point;
                std::map<float, std::pair<float, float>> all_points_to_check;
                // All values that is used to calculate the path
                std::list<float> all_values;
                // All points of the map associated with their visit state (true if visited and false if not)
                std::map<std::pair<int, int>, std::map<std::pair<float, float>, bool>> all_points;
                // All old points of the map associated with their visit state (true if visited and false if not)
                std::map<std::pair<int, int>, std::map<std::pair<float, float>, bool>> all_old_points;
                std::map<std::pair<int, int>, std::map<std::pair<float, float>, float*>> all_g_values;
                std::map<std::pair<int, int>, std::map<std::pair<float, float>, float*>> all_old_g_values;
                std::map<std::pair<float, float>, std::pair<float, float>> parent_points;
                std::vector<std::pair<float, float>> path;
                Map::Object* map_target;
                bool building_path = false;
                
                // Creates the path finding version of one map
                void create_map_of(Map::Object* map);

                // Searches the path
                void search();

                // Creates the path
                void create_path();

                // Gets all adjacent points of a specific point
                std::vector<std::pair<float, float>> get_adjacent_point_of(std::pair<float, float> point);

                // Gets the value that is used to find the path
                float get_value_of(std::pair<float, float> point, std::pair<float, float> parent_point);

                // Verifies if the path finder found the finish position
                bool found_end_position();

                // Verifies if the path finder can find the path
                bool can_find_path();

                // Verifies if a specific point is correct or not
                bool is_correct(std::pair<float, float> point);

                // Verifies if the parent point is really the parent point of the point
                bool is_parent_point_of(std::pair<float, float> point, std::pair<float, float> parent_point);
        };
    };

#endif