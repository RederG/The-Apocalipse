#ifndef __ARTIFICIAL_FOOD__
    #define __ARTIFICIAL_FOOD__

    #include "food.hpp"

    namespace Food{
        class Object;
    };

    namespace Artificial_food{
        enum Foods_names{
            damaged_food_bag
        };

        const std::map<Foods_names, std::map<Main::Languages, sf::String>> Names{
            {damaged_food_bag, {
                {Main::english, "Damaged food bag"},
                {Main::french, L"Sac de nourriture détruit"},
                {Main::malagasy, "Fasiana sakafo simba"}
            }}
        };

        const std::map<Main::Languages, std::string> Extra_info{
            {Main::english, "Give "},
            {Main::french, "Donne "},
            {Main::malagasy, "Manome "}
        };

        const std::map<Main::Languages, sf::String> Extra_info_suite{
            {Main::english, " of satiation"},
            {Main::french, L" de satiété"},
            {Main::malagasy, " fahavokisana"}
        };

        const std::map<Foods_names, sf::IntRect> textures_location{
            {damaged_food_bag, sf::IntRect({0,0}, {32,32})}
        };

        const std::map<Foods_names, float> satiation_values{
            {damaged_food_bag, 20}
        };

        class Object : public Food::Object{
            public:
                Object(Foods_names food_name);
                bool add_to_container(Container::Object* container, bool aleatory = false) override;
                void add_to(Container::Object* container, int location) override;
                std::string get_extra_info() override;
                std::string get_file_name() override;
            private:
                Foods_names food_name;
        };

        Object* create_new(Foods_names food_name);
    };

#endif