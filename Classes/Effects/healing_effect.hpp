#ifndef __HEALING_EFFECT__
    #define __HEALING_EFFECT__

    #include "effect.hpp"

    class Healing_effect : public Effect::Object{
        public:
            /*
            Constructs a healing effect
            The "efficiency" parameter represents the value of life we want to
            add each healing time
            */
            Healing_effect(int seconds, int healing_value, int healing_time_lapse);
            void add_to(Effect_manager* effect_manager) override;
            void update() override;
            void effect(Entity::Object* entity) override;
        private:
            void save_effect_to(std::string destination) override;
            void load_effect_from(std::string effect_file_path) override;
            int healing_value = 0;
    };

#endif