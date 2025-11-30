#ifndef __CONTAINER__
    #define __CONTAINER__

    #include "../../main.hpp"

    namespace Item{
        class Object;
    };
    
    // All about the container on the game
    namespace Container{

        // All directions that the selector can go
        enum Direction{
            Right, Left
        };

        // All types of container
        enum Type{
            simple, chest
        };

        // A container object
        class Object{
            public:
                // The constructor
                Object(int max_size);

                // Gets the max size of the container
                int get_max_size();

                // Gets the current location ( selector ) of the container
                int get_current_location();

                // Gets a specific item on the container
                Item::Object* get_item_at(int location);

                // Gets the current item on the container
                Item::Object* get_current_item();

                // Gets the contain of the container
                std::map<int, std::shared_ptr<Item::Object>>& get_contain();

                // Verifies if the container is full or not
                // - Returns True if the container is full
                // - Returns False if not
                bool is_full();

                // Verifies if the container is empty
                // - Returns True if the container is empty
                // - Returns False if not
                bool is_empty();

                // Removes a specific item
                Item::Object* remove_item_on(int location);

                // Removes the current item
                Item::Object* remove_current_item();

                // Removes all items
                void clear();

                // Move the selector
                void move_selector_to(Direction direction);

                // Transfers every item to another container
                void transfer_items_to(Object* container);

                // Updates the container
                void update();

                void save_to(std::string destination_path, std::string file_name, std::string items_files_path);
                void load_from(std::string container_file_name, std::string items_files_path);

            protected:
                std::map<int, std::shared_ptr<Item::Object>> contain; // The contain of the container
                int max_size; // The max size of the container
                int current_location; // The current location of the container
        };
    
    };

    #include "../Items/item.hpp"

#endif