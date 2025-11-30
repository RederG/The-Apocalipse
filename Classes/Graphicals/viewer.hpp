#ifndef __VIEWER__
    #define __VIEWER__

    #include "../../main.hpp"
    #include "../Others/container.hpp"
    #include "../Map/map_manager.hpp"
    #include "../Entities/player.hpp"
    #include "../Items/item.hpp"
    #include "../../Help/texture_manager.hpp"
    #include "../../Help/font_manager.hpp"
    #include "../Effects/effect_manager.hpp"

    namespace Item{
        class Object;
    };

    namespace Container{
        class Object;
    };

    namespace MapElement{
        class Object;
    };

    namespace Map{
        class Object;
    };

    namespace Player{
        class Object;
    };

    namespace Viewer{
        void draw_container(Container::Object* container, MapElement::Object* element, Map::Object* map, bool draw_selector);
        
        void draw_satiation(Player::Object* player);

        void draw_effect(Effect_manager* effect_manager, int effect_number_limit_to_draw);

        // All inforamtions for the item
        namespace Items{
            
            void change_to(Item::Object* item);

            void draw();

            void update(); 

            void set_to_default();
        
            namespace Other{
                const std::map<Main::Languages, std::string> item_number{
                    {Main::english, " left"},
                    {Main::french, " restant"},
                    {Main::malagasy, " sisa"}
                };

                const std::map<Main::Languages, std::string> cartridge{
                    {Main::english, " bullet(s)"},
                    {Main::french, " balle(s)"},
                    {Main::malagasy, " bala(s)"}
                };

                const std::map<Main::Languages, sf::String> durability{
                    {Main::english, " durability"},
                    {Main::french, L" durabilité"},
                    {Main::malagasy, " fahasimbana"}
                };
            };
        }
    };

#endif