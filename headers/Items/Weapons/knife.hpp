#ifndef __KNIFE__
    #define __KNIFE__

    #include "weapon.hpp"

    // A knife
    class Knife : public Weapon::Object{
        public:
            // The default destructor
            Knife(); 

            // When we use the knife
            void use_item(Entity::Object &entity, bool special_use = false) override;

            // Draws the attack rect of the knife
            void draw_attack_rect(sf::RenderWindow &window);

            // Cuts a specific entity
            void cut(Entity::Object* entity);

            // Add the knife on a specific container
            void add_to(Container::Object* container, int location) override;

            // Returns the extra info of the knife
            std::string get_extra_info() override;

            std::string get_file_name() override;

        protected:
            sf::RectangleShape attack_rect;
    };

    // Creates properly a knife
    Knife* new_knife();

#endif