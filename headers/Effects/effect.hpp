#ifndef __EFFECT__
    #define __EFFECT__

    #include "../main.hpp"
    
    namespace Entity{class Object;};

    class Effect_manager;

    namespace Effect{
        // All types of effects
        enum Type{
            nothing, healing, hungry
        };

        // All textures location
        const std::map<Type, sf::IntRect> textures_location{
            {nothing, sf::IntRect({0,0}, {32,32})},
            {healing, sf::IntRect({64,0}, {32,32})},
            {hungry, sf::IntRect({96,0}, {32,32})}
        };

        // An effect object
        class Object{
            public:
                Object();
                std::string get_id();
                Type get_effect_type();
                int get_life();
                int get_max_life();
                void set_max_life(int new_max_life);
                sf::IntRect get_texture_location();
                std::string get_texture_name();
                void virtual effect(Entity::Object* entity);
                void virtual update();
                void virtual add_to(Effect_manager* effect_manager);
                bool virtual is_usable();
                void save_to(std::string destination_path);
                void load_from(std::string effect_file_path);
            protected:
                void virtual save_effect_to(std::string destination_path);
                void virtual load_effect_from(std::string effect_file_path);
                std::string id;
                Type effect_type = nothing;
                int max_life = 0;
                int life = 0;
                int affecting_elapse_time = 0;
                sf::Clock elapse_time_clock;
                std::string texture_name = "effects";
        };
    };

    #include "effect_manager.hpp"
    #include "../Entities/entity.hpp"

#endif