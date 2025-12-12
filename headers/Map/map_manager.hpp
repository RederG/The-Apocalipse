#ifndef __MAP_MANAGER__
    #define __MAP_MANAGER__

    #include "../main.hpp"
    #include "../Help/texture_manager.hpp"
    #include "../Others/perlin_noise.hpp"
    #include "map_element.hpp"
    #include "Structures/structure_manager.hpp"

    namespace MapElement{
        class Object;
        class Environment;
    };

    namespace Structure{
        struct Instance;
    };

    namespace Map{
        // All types of entity
        enum Entity_Type{
            Entity, Player, Zombie
        };

        class Object;

        // An area of the map
        class Area{
            public:
                // The default constructor
                Area();

                // The pointer copier
                Area(Area* area);

                // The other constructor
                Area(sf::Vector2i size, sf::Vector2i map_position, sf::Vector2f window_position = sf::Vector2f(0, 0));

                // Gets the size of the area
                sf::Vector2i get_size();

                // Gets the position of the area on the map
                sf::Vector2i get_position();

                // Gets the position of the area on the window
                sf::Vector2f get_window_position();

                // Generates the element on a specific position on the area
                MapElement::WorldContent get_element_at(int x, int y, PerlinNoise::Object* noise);

                // Generates the element on a specific position on the area
                MapElement::WorldContent get_element_at(sf::Vector2i element_position, PerlinNoise::Object* noise);

                // Generates the element on a specific position on the area
                MapElement::WorldContent get_element_at(unsigned int x, unsigned int y, sf::Image area_image, bool is_structure = false);

                // Gets the element on a specific position on the area
                MapElement::WorldContent get_element_at(int x, int y);

                // Gets the element on a specific position on the area
                MapElement::WorldContent get_element_at(sf::Vector2i element_position);

                // Gets the environment on a specific position on the area 
                //( The position x and y must between 0 and the max size of the area )
                MapElement::Environment* get_environment_at(int x, int y);

                // Gets the environment on a specific position on the area 
                //( The position x and y must between 0 and the max size of the area )
                MapElement::Environment* get_environment_at(sf::Vector2i element_position);

                // Gets the list of all elements of the area
                std::vector<std::vector<MapElement::WorldContent>> get_all_elements();

                // Gets the list of all elements' visit state for path finding of the area
                std::map<std::pair<float, float>, bool> get_all_elements_visit();

                // Gets the list of all elements' g values for path finding of the area
                std::map<std::pair<float, float>, float*> get_all_elements_g_values();

                // Verifies if the area has a structure
                bool has_structure();
                
                // Verifies if the area are configured
                bool is_configurated();

                // Draws the area
                void draw_to(Object* map);

                // Sets the position of the area on the window
                void set_window_position_to(sf::Vector2f new_position);

                // Configure the list of elements of the area
                void configure_with(PerlinNoise::Object* noise);

                // Destroys all elements
                void clear();

                // Saves all elements on a file
                // - It must ends with a " / "
                // - the "save_location" must contains an "Area" folder
                void save_to(std::string save_location);

                // Load the area from a file
                void load_from(std::string load_location);

            private:
                sf::Vector2i size; // The size of the area
                sf::Vector2i map_position; // The position of the area on the map
                sf::Vector2f window_position; // The area's position on the window
                std::vector<std::vector<MapElement::WorldContent>> all_elements; // All elements of the area
                std::map<std::pair<float, float>, bool> all_elements_visit_for_pathfinding;
                std::map<std::pair<float, float>, float*> all_elements_g_values_for_pathfinding;
                bool configured = false;
                bool structure_exists = false;
        };
        
        // The controller of all areas
        class AreaGroup{
            public:
                // The default constructor
                // - Size = (5, 5)
                // - Position = (0, 0)
                // - Area size = (16, 16)
                AreaGroup();

                // Contructor personisable
                AreaGroup(sf::Vector2i size, sf::Vector2i area_group_position, sf::Vector2i area_size);

                // Contructor personisable
                AreaGroup(sf::Vector2i size, sf::Vector2f area_start_position, sf::Vector2i area_size);

                // Gets the position of the group of areas
                sf::Vector2i get_map_position();

                // Gets the size of the group of areas
                sf::Vector2i get_size();

                // Gets a specific area
                Area* get_area_at(float x, float y);

                // Gets a specific area
                Area* get_area_at(sf::Vector2f area_map_position);

                // Adds a new area on the group
                Area* add_area(float x, float y);

                // Adds a new area on the group
                Area* add_area(sf::Vector2f area_position);

                // Adds a new area on the group
                Area* add_area(Area* area);

                // Gets all areas
                std::vector<Area*> get_all_areas();

                // Clears all areas
                void clear();

                // Verifies if the coordinates are in the area group
                bool is_mine(float x, float y);

                // Verifies if the coordinates are in the area group
                bool is_mine(sf::Vector2f position);

            private:
                std::vector<Area*> all_areas; // All areas on the group of areas
                sf::Vector2i position; // The position of the group of area
                sf::Vector2i area_group_size; // The size of the group of area
                sf::Vector2i area_size; // The size of each area on the group
                std::pair<float, float> accepted_area_position_x;
                std::pair<float, float> accepted_area_position_y;
                void init();
        };
        
        // A map
        class Object{
            public:
                // the constructor
                Object(sf::RenderWindow &window, PerlinNoise::Object &noise, sf::Vector2i area_size = sf::Vector2i(16, 16), sf::Vector2i area_group_size = sf::Vector2i(5, 5), sf::Vector2f scale = sf::Vector2f(2, 2), sf::Vector2i terrain_generation_size = sf::Vector2i(1, 1), sf::Vector2i updating_radius = sf::Vector2i(1, 1));

                // The other constructor
                Object(sf::RenderWindow &window);

                // Gets the window of the map
                sf::RenderWindow* get_window();

                // Gets the noise of the map
                PerlinNoise::Object* get_noise();

                // Gets the scale of the map's cells
                sf::Vector2f get_scale();

                // Gets the position of the player on the window
                sf::Vector2f get_player_pos_on_window();

                // Gets the player's position on the map
                sf::Vector2f get_player_map_pos();

                // Gets the distance between the middle of the screen and the player
                sf::Vector2f get_distance_player_and_window_middle();

                // Converts a window position to a map position
                sf::Vector2f get_map_position_of(sf::Vector2f window_position);

                // Gets the size of the area of the map
                sf::Vector2i get_area_size();

                // Gets the size of the terrain generation
                sf::Vector2i get_terrain_generation_size();

                // Gets the area where the player is
                Area* get_player_area();

                // Gets the area of a specific entity
                Area* get_area_of(MapElement::Object* element);

                // Gets the area on a specific position of the map 
                Area* get_area_at(float area_x, float area_y);

                // Gets the area on a specific position of the map
                Area* get_area_at(sf::Vector2i area_map_position);

                // Gets the position of the spawn based on the seed
                sf::Vector2i get_spawn_position(int seed);

                // Gets the position of a specific area
                sf::Vector2i get_area_position(float x, float y);

                // Generates a new area
                Area* new_area(int x, int y);

                // Generates a new area
                Area* new_area(sf::Vector2i map_position);

                // Put a new area
                Area* new_area(Area* area);

                // Gets the environment on a specific position
                MapElement::Environment* get_environment_at(sf::Vector2f map_position);

                // Gets the environment on a specific position
                MapElement::Environment* get_environment_at(sf::Vector2i map_position);

                // Gets the environment on a specific position
                MapElement::Environment* get_environment_at(float x, float y);

                // Gets all working areas ( Used for structure generation algo, 
                //map elements' positioning, zombie spawning, ... )
                std::vector<Area*> get_all_working_areas();

                // Gets all areas for the rendering, updating MapElements like Zombies, 
                //structure's content, etc
                std::vector<Area*> get_all_updating_areas();

                // Gets all working areas' position
                std::vector<std::pair<int, int>> get_all_areas_position();

                // Generates a new area group
                AreaGroup* new_area_group(int x, int y);

                // Generates a new area group
                AreaGroup* new_area_group(sf::Vector2i area_group_position);

                // Generates a new area group
                AreaGroup* new_area_group_that_contains(sf::Vector2f area_position); 

                // Gets a specific area group
                AreaGroup* get_area_group(int x, int y);

                // Gets a specific area group
                AreaGroup* get_area_group(sf::Vector2i area_group_position);

                // Gets the position of a specific area group
                sf::Vector2i get_area_group_position(float area_x, float area_y);

                // Generates the working, rendering areas and others
                void update_areas();

                /* 
                Configure a specific area on a position on the map 
                with its position on the window
                - If the area was already created, its "window_position" will be updated
                */
                Area* configure_area(int x, int y, sf::Vector2f window_position);

                /* 
                Configure a specific area on a position on the map 
                with its position on the window
                - If the area was already created, its "window_position" will be updated
                */
                Area* configure_area(sf::Vector2i map_position, sf::Vector2f window_position);

                // Updates the order of drawing all elements (entities, interactive objects, ...)
                void set_elements_drawing_order();

                // Updates the map ( updates all areas around the player )
                // - These areas can be not all configurated
                void update();

                // Draws all elements
                void draw();
                
                // Reduces the scale of the map
                void reduce_scale();

                // Increases the scale of the map
                void increase_scale();

                // Destroys all areas
                void clear();

                /*
                Save all map's elements
                - The "save_file_location" is the path for saving the map
                - It must ends with a " / "
                */ 
                void save_to(std::string save_file_location);

                // Load the world from a file
                bool load_from(std::string load_location, std::string map_name);

                // Verifies if all areas are configurated
                bool is_updated();

                // Verifies if an area is on the working areas
                bool is_working(Area* area);

                // Verifies if an area is on the for the rendering
                bool is_for_render(Area* area);

                // Verifies if the player is on a new area
                bool is_player_changed_area();

            private:
                sf::RenderWindow* window_target; // The window that the map wiil use
                PerlinNoise::Object* noise_target; // The noise that tha map will use
                std::vector<AreaGroup*> all_areas_group; // All groupement of areas
                std::vector<Area*> all_working_areas; // all areas that should be render
                std::vector<Area*> all_updating_areas; // all areas that should be render
                sf::Vector2i old_player_area; // The area where the player is before
                sf::Vector2f player_pos_window; // the player's position on the window
                sf::Vector2f scale; // The scale of one environment element
                sf::Vector2i area_size; // The size of an area
                sf::Vector2i area_group_size; // The size of all group of areas
                sf::Vector2i terrain_generation_size; // The terrain generation radius
                sf::Vector2i event_updating_size; // The updating radius (Render distance)
                sf::Vector2i old_player_position;
                std::pair<float, float> player_area_x_limits = std::pair<float, float>(0,0);
                std::pair<float, float> player_area_y_limits = std::pair<float, float>(0,0);
                std::vector<MapElement::Object*> sorted_elements_to_draw;
                std::vector<MapElement::Object*> elements_to_draw;
                std::vector<sf::Vector2i> all_struct_contain_to_draw;
                std::vector<sf::Vector2i> all_struct_roof_to_draw;
                bool sorting_elements = false;
        };

        /*
        Save all maps on a file
        - The "save_file_location" is the path for saving the map
        - It must ends with a " / "
        */
        void save_all(std::string save_file_location);

        // Verifies if we can move on a specific element
        bool can_move_on(MapElement::WorldContent element);

        // Clears the list of maps
        void clear();

        // Creates a map
        void create(std::string map_name, sf::RenderWindow &window, PerlinNoise::Object* noise, sf::Vector2i area_size = sf::Vector2i(16, 16), sf::Vector2i area_group_size = sf::Vector2i(5, 5), sf::Vector2f scale = sf::Vector2f(2, 2), sf::Vector2i terrain_generation_size = sf::Vector2i(1, 1), sf::Vector2i update_radius = sf::Vector2i(1, 1));
        
        // Loads a map from a file
        // - The " file_path " must ends with a " / " and it must contains
        // a " World " folder
        bool load_from(std::string file_path, std::string map_name, sf::RenderWindow &window);

        // Removes a specific map
        void remove(std::string map_name);

        // Verifies if a map exists
        bool exists(std::string map_name);

        // Gets a specific map
        Object* get(std::string map_name);

        // Gets the spawn area based on a seed
        sf::Vector2i get_spawn_area(int seed);

        // The default name of the map
        const std::string NAME = "terrain";

        // The default name for the aleatory noise
        const std::string NAME_ALEATORY_NOISE = "aleatory_noise";
    };

#endif