#ifndef __FOOD__
    #define __FOOD__

    #include "../item.hpp"

    namespace Item{
        class Object;
    };

    namespace Food{
        enum Type{
            natural, artificial
        };

        const std::map<Type, std::map<Main::Languages, sf::String>> Names{
            {natural, {
                {Main::english, "Natural food"},
                {Main::french, "Nourriture naturelle"},
                {Main::malagasy, "Sakafo natoraly"},
            }},
            {artificial, {
                {Main::english, "Artificial food"},
                {Main::french, "Nourriture artificielle"},
                {Main::malagasy, "Sakafo namboarina"},
            }},
        };

        const std::map<Type, sf::String> food_type_textures{
            {natural, "natural_foods"},
            {artificial, "artificial_foods"}
        };

        class Object : public Item::Object{
            public:
                Object();
                Object(Type food_type);
                Type get_food_type();
                void use_item(Entity::Object &entity, bool special_use = false) override;
                void set_number();
            protected:
                float satiation_value = 1;
                Type food_type = natural;
        };

        extern std::vector<std::shared_ptr<Object>> all_foods;

        std::shared_ptr<Object> get_food(std::string name, std::string id);
    };

#endif