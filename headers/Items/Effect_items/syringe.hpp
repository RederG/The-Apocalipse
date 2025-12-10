#ifndef __SYRINGE__
    #define __SYRINGE__

    #include "effect_item.hpp"

    namespace Effect{
        class Effect_item;
    }
    
    namespace Syringe{
        // All types of syringe
        enum Type{
            clean, used, zombie
        };

        const std::map<Type, std::map<Main::Languages, sf::String>> Names{
            {clean, {
                {Main::english, "Clean syringe"},
                {Main::french, "Seringue utilisable"},
                {Main::malagasy, "Tsindro azo ampiasaina"}
            }},

            {used, {
                {Main::english, "Used syringe"},
                {Main::french, "Seringue post-utilisable"},
                {Main::malagasy, "Tsindro avy nampiasaina"}
            }},

            {zombie, {
                {Main::english, "Zombiefication syringe"},
                {Main::french, "Seringue de zombification"},
                {Main::malagasy, "Tsindro mampihinana olona"}
            }}
        };

        const std::map<Type, std::map<Item::Direction, sf::IntRect>> textures_location{
            {clean, {
                {Item::down, {sf::IntRect({0,0}, {32,32})}},
                {Item::up, {sf::IntRect({0,0}, {32,32})}},
                {Item::right, {sf::IntRect({0,0}, {32,32})}},
                {Item::left, {sf::IntRect({0,0}, {32,32})}}
            }},
            
            {used, {
                {Item::down, {sf::IntRect({32,0}, {32,32})}},
                {Item::up, {sf::IntRect({32,0}, {32,32})}},
                {Item::right, {sf::IntRect({32,0}, {32,32})}},
                {Item::left, {sf::IntRect({32,0}, {32,32})}}
            }},
            
            {zombie, {
                {Item::down, {sf::IntRect({64,0}, {32,32})}},
                {Item::up, {sf::IntRect({64,0}, {32,32})}},
                {Item::right, {sf::IntRect({64,0}, {32,32})}},
                {Item::left, {sf::IntRect({64,0}, {32,32})}}
            }}
        };

        const std::map<Type, std::map<Main::Languages, sf::String>> all_extra_infos{
            {clean, {
                {Main::english, "We can fill it"},
                {Main::french, "On peut le remplir"},
                {Main::malagasy, "Afaka fenoana zavatra"}
            }},
            
            {used, {
                {Main::english, "Clean it before filling it"},
                {Main::french, "Desinfecter avant de remplir"},
                {Main::malagasy, "Sasao alohan'ny fenoana"}
            }},
            
            {zombie, {
                {Main::english, "Heal but give hungry"},
                {Main::french, "Soigne mais donne faim"},
                {Main::malagasy, "Mitsabo fa maha-noana"}
            }}
        };

        const std::map<Item::Direction, sf::Vector2f> Handling_location{
            {Item::down, {sf::Vector2f(16,16)}},
            {Item::up, {sf::Vector2f(16,16)}},
            {Item::right, {sf::Vector2f(16,16)}},
            {Item::left, {sf::Vector2f(16,16)}}
        };

        // A syringe object
        class Object : public Effect_item::Object{
            public:
                Object(Type syringe_type, Type old_syringe_type);
                Object(Type syringe_type, Type old_syringe_type, int number);
                void create_effect_for(Entity::Object* entity) override;
                bool add_to_container(Container::Object* container, bool aleatory = false) override;
                void add_to(Container::Object* container, int location) override;
                std::string get_extra_info() override;
                std::string get_file_name() override;
                void set_number();
            private:
                Type syringe_type;
                Type old_syringe_type;
        };

        Object* create_new(Type syringe_type, Type old_syringe_type);

        Object* create_new(Type syringe_type, Type old_syringe_type, int number);

        std::shared_ptr<Object> get_syringe(std::string name, std::string id);

        void clear();
    };

#endif