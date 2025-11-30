#ifndef __LIFE_BAR__
    #define __LIFE_BAR__

    #include "../../main.hpp"

    namespace Entity{   
        class Object;
    };
    
    namespace Map{
        class Object;
    };
    
    namespace Life_Bar{
        
        // An object from the life bar class
        class Object{
            public:
                // the constructor
                Object(float max_life);

                // Draws the life bar
                void draw_to(Entity::Object* entity, Map::Object* map);

                // Sets the new life of the owner
                void set_new_life_to(int new_life);

                // Resets the timer of drawing the life bar
                void reset_timer();
                
            private:
                float life_now; // The life
                float max_life; // The maximum life
                int showing_time = 0;
                sf::Clock appearing_clock; // The timer for the drawing time
                sf::RectangleShape life_rect_red; // The red rectagle of the life bar
                sf::RectangleShape life_rect_green; // The green rectagle of the life bar
        };

        // Sets the pause drawing to a new value
        void set_pause_drawing_to(bool state);

    }

    #include "../Entities/entity.hpp"
    #include "../Map/map_manager.hpp"
#endif