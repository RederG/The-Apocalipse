#ifndef __EFFECT_ITEM__
    #define __EFFECT_ITEM__

    #include "../item.hpp"
    #include "../../Effects/all_effects.hpp"

    namespace Effect_item{

        // All names of a simple help item
        const std::map<Main::Languages, sf::String> Names{
            {Main::english, "Effect item"},
            {Main::french, "Item avec effet"},
            {Main::malagasy, "Zavatra mpanome toe-javetra"}
        };

        // All items that can make to the entities(boosters, healing, etc)
        class Object : public Item::Object{
            public:
                Object();
                void use_item(Entity::Object &entity, bool special_use = false) override;
                unsigned int get_number(bool special = false) override;
                void virtual create_effect_for(Entity::Object* entity);
        };
    };

#endif