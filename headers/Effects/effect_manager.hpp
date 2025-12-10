#ifndef __EFFECT_MANAGER__
    #define __EFFECT_MANAGER__

    #include "../main.hpp"

    namespace Effect{class Object;};
    namespace Entity{class Object;};

    // The controller of all effects
    class Effect_manager{
        public:
            Effect_manager();
            void update_on(Entity::Object* entity);
            void update();
            void clear();
            std::vector<Effect::Object*>& get_all_effects();

            // - The " destination_path " must ends with the ".effman" extension
            // - The " file_name " must not ends with ".effman" extension
            // - The " effects_files_path " must contains every effects' files with ".effect" extension
            void save_to(std::string destination_path, std::string file_name, std::string effects_files_path);

            // - The " location_path " must ends with the ".effman" extension
            // - The " effects_files_path " must contains every effects' files with ".effect" extension
            void load_from(std::string location_path, std::string effects_files_path);
        private:
            std::vector<Effect::Object*> all_effects;
    };
    
    #include "all_effects.hpp"
    
#endif