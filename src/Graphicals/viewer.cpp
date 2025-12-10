#include "../../headers/Graphicals/viewer.hpp"

namespace Viewer{

    enum Satiation_level{
        empty, low, half, little_full, full
    };

    std::map<Satiation_level, sf::IntRect> satiation_lvl_textures{
        {full, sf::IntRect({0,0}, {32,32})},
        {little_full, sf::IntRect({32,0}, {32,32})},
        {half, sf::IntRect({64,0}, {32,32})},
        {low, sf::IntRect({96,0}, {32,32})},
        {empty, sf::IntRect({128,0}, {32,32})}
    };

    void draw_container(Container::Object* container, MapElement::Object* element, Map::Object* map, bool draw_selector){
        sf::Sprite element_sprite = element->get_sprite(map);

        sf::Vector2f element_position = element_sprite.getPosition();
        sf::Vector2f element_origin = element_sprite.getOrigin();
        sf::Vector2f element_scale = element_sprite.getScale();

        sf::Vector2f element_default_position;
        element_default_position.x = element_position.x - element_origin.x*element_scale.x;
        element_default_position.y = element_position.y - element_origin.y*element_scale.y;

        sf::IntRect container_slot_location = sf::IntRect({0,0}, {32,32});

        sf::Vector2f container_scale = sf::Vector2f(map->get_scale().x, map->get_scale().y), slot_size;
        sf::Vector2f container_size;
        container_size.x = (container->get_max_size() - 1)*container_scale.x*container_slot_location.size.x;
        container_size.y = container_slot_location.size.y*container_scale.y;

        slot_size.x = container_scale.x*container_slot_location.size.x;
        slot_size.y = container_scale.y*container_slot_location.size.y;

        sf::Vector2f slot_position = element_default_position, first_slot_position;
        slot_position.x -= container_size.x/2;
        slot_position.x += (element_sprite.getTextureRect().size.x*container_scale.x)/2;
        slot_position.y -= container_size.y;
        first_slot_position = slot_position;

        sf::Vector2f item_name_position = slot_position;
        item_name_position.x += (element_sprite.getTextureRect().size.x*container_scale.x)/2 + container_size.x/2;
        item_name_position.y -= slot_size.y;

        for(int location = 0; location < container->get_max_size(); location++){
            sf::Sprite slot_sprite(*AllTextures::get("inventory"));
            slot_sprite.setTextureRect(container_slot_location);
            slot_sprite.setScale(container_scale);
            slot_sprite.setPosition(slot_position);
            
            Main::window()->draw(slot_sprite);

            if(container->get_contain()[location] != nullptr){
                sf::Sprite item_sprite(*AllTextures::get(container->get_contain()[location]->get_texture_name()));
                item_sprite.setTextureRect(container->get_contain().at(location)->get_texture_on(Item::Direction::right));
                item_sprite.setScale(container_scale);
                item_sprite.setPosition(slot_position);
                Main::window()->draw(item_sprite);

                sf::Text item_number(*FontManager::get_font(FontManager::Arial));
                sf::Vector2f item_number_position = slot_position;
                item_number_position.x += slot_size.x;
                item_number_position.y += slot_size.y;

                item_number.setFillColor(sf::Color::White);

                int number = 0;
                number = container->get_contain().at(location)->get_number();

                item_number.setString(to_string(number));
                item_number.setCharacterSize(10*map->get_scale().x);

                item_number_position.x -= item_number.getGlobalBounds().size.x*2;
                item_number_position.y -= item_number.getGlobalBounds().size.y*2;

                item_number.setPosition(item_number_position);
                Main::window()->draw(item_number);

                if(location == container->get_current_location()){
                    sf::Text item_name(*FontManager::get_font(FontManager::Arial));
                    item_name.setFillColor(sf::Color::White);
                    item_name.setString(container->get_contain().at(location)->get_name());
                    item_name.setCharacterSize(20*map->get_scale().x);

                    item_name_position.x -= item_name.getGlobalBounds().size.x/2;
                    item_name_position.y -= item_name.getGlobalBounds().size.y/2;

                    item_name.setPosition(item_name_position);
                    Main::window()->draw(item_name);
                }
            }

            container_slot_location.position.x += 32;
            sf::Sprite selector(*AllTextures::get("inventory"));
            selector.setTextureRect(container_slot_location);
            selector.setScale(container_scale);
            selector.setPosition(slot_position);

            if(location == container->get_current_location() && draw_selector == true)
                Main::window()->draw(selector);

            slot_position.x += container_scale.x*container_slot_location.size.x;
            container_slot_location.position.x -= 32;
        }
    }

    void draw_satiation(Player::Object* player){
        float satiation = player->get_satiation();
        float max_satiation = player->get_max_satiation();

        const float SATIATION_LVL_TO_DRAW = 10;
        if(satiation >= 0 && max_satiation >= 0 && satiation <= max_satiation && SATIATION_LVL_TO_DRAW > 0){
            float satiation_lvl = satiation*SATIATION_LVL_TO_DRAW;
            satiation_lvl /= max_satiation;

            float low_satiation = 0.25;
            float half_satiation = 0.5;
            float little_full_satiation = 0.75;

            int full_satiation_number = int(satiation_lvl);

            const sf::Vector2f SCALE = sf::Vector2f(1.8, 1.8);

            sf::Vector2f satiation_lvl_position;
            satiation_lvl_position.x = Main::window()->getSize().x/2;
            satiation_lvl_position.y = Main::window()->getSize().y - (32*SCALE.y + 10);
            satiation_lvl_position.x -= 32*SCALE.x*SATIATION_LVL_TO_DRAW/2;

            bool empty_satiation_drawing = false;
            for(int i = 0; i < SATIATION_LVL_TO_DRAW; i++){
                sf::Sprite satiation_lvl_sprite(*AllTextures::get("satiation"));
                satiation_lvl_sprite.setScale(SCALE);
                satiation_lvl_sprite.setPosition(satiation_lvl_position);

                if(full_satiation_number > 0){
                    satiation_lvl_sprite.setTextureRect(satiation_lvl_textures.at(full));
                    full_satiation_number--;
                }
                else if(empty_satiation_drawing == false){
                    float satiation_lvl_variation = satiation_lvl - int(satiation_lvl);
                    if(satiation_lvl_variation == 0)
                        satiation_lvl_sprite.setTextureRect(satiation_lvl_textures.at(empty));
                    else if(satiation_lvl_variation > 0 && satiation_lvl_variation <= low_satiation)
                        satiation_lvl_sprite.setTextureRect(satiation_lvl_textures.at(low)); 
                    else if(satiation_lvl_variation > low_satiation && satiation_lvl_variation <= half_satiation)
                        satiation_lvl_sprite.setTextureRect(satiation_lvl_textures.at(half));
                    else if(satiation_lvl_variation > half_satiation && satiation_lvl_variation <= little_full_satiation)
                        satiation_lvl_sprite.setTextureRect(satiation_lvl_textures.at(little_full));
                    else
                        satiation_lvl_sprite.setTextureRect(satiation_lvl_textures.at(full));
                    empty_satiation_drawing = true;
                }
                else if(empty_satiation_drawing == true)
                    satiation_lvl_sprite.setTextureRect(satiation_lvl_textures.at(empty));
                
                Main::window()->draw(satiation_lvl_sprite);
                satiation_lvl_position.x += 32*SCALE.x;
            }
        }
    }

    void draw_effect(Effect_manager* effect_manager, int effect_number_limit_to_draw){
        sf::Vector2f effect_coordinates = sf::Vector2f(0,0);
        sf::Vector2f window_size;
        window_size.x = Main::window()->getSize().x;
        window_size.y = Main::window()->getSize().y;

        sf::Vector2f scale = sf::Vector2f(1.8, 1.8);
        int space = 10;
        int effects_number = effect_manager->get_all_effects().size();
        int limit = effect_number_limit_to_draw;
        int effect_number_to_draw = 0;

        if(effects_number > limit)
            effect_number_to_draw = limit;
        else
            effect_number_to_draw = effects_number;

        effect_coordinates.x = window_size.x/2 - ((32*scale.x*effect_number_to_draw + space*(effect_number_to_draw - 1))/2);
        effect_coordinates.y = window_size.y - ((32*scale.y + space)*2);

        for(int i = 0; i < effects_number && effects_number > 0; i++){
            Effect::Object* effect = effect_manager->get_all_effects()[i];
            if(effect != nullptr && i < limit){
                sf::Sprite effect_sprite(*AllTextures::get(effect->get_texture_name()));
                effect_sprite.setColor(sf::Color(255,255,255,255));
                effect_sprite.setTextureRect(effect->get_texture_location());
                effect_sprite.setScale(scale);
                effect_sprite.setPosition(effect_coordinates);

                sf::Text effect_time(*FontManager::get_font(FontManager::Arial));
                std::string effect_time_txt = std::to_string(effect->get_max_life() - effect->get_life());
                effect_time.setCharacterSize(10*scale.x);
                effect_time.setFillColor(sf::Color::White);
                effect_time.setString(effect_time_txt);
                sf::Vector2f effect_time_txt_pos;
                effect_time_txt_pos.x = (effect_coordinates.x + 32*scale.x) - effect_time.getGlobalBounds().size.x - 4*scale.x;
                effect_time_txt_pos.y = (effect_coordinates.y + 32*scale.y) - effect_time.getGlobalBounds().size.y - 6*scale.y;
                effect_time.setPosition(effect_time_txt_pos);

                Main::window()->draw(effect_sprite);
                Main::window()->draw(effect_time);

                effect_coordinates.x += 32*scale.x + space;
            }
            if(i == limit && i < effects_number){
                sf::Sprite effect_sprite(*AllTextures::get("effects"));
                effect_sprite.setColor(sf::Color::White);
                effect_sprite.setTextureRect(sf::IntRect({32,0}, {32,32}));
                effect_sprite.setScale(scale);
                effect_sprite.setPosition(effect_coordinates);

                Main::window()->draw(effect_sprite);
            }
        }
        if(effects_number <= 0){
            effect_coordinates.x = window_size.x/2 - 32*scale.x/2;
            sf::Sprite no_effect_sprite(*AllTextures::get("effects"));
            no_effect_sprite.setColor(sf::Color(255,255,255,255));
            no_effect_sprite.setTextureRect(sf::IntRect({0,0}, {32,32}));
            no_effect_sprite.setScale(scale);
            no_effect_sprite.setPosition(effect_coordinates);

            Main::window()->draw(no_effect_sprite);
        }
    }

    namespace Items{

        sf::Sprite  item_sprite =       *new sf::Sprite(*new sf::Texture());
        sf::Text    item_name =         *new sf::Text(*new sf::Font()), 
                    item_life =         *new sf::Text(*new sf::Font()), 
                    item_extra_info =   *new sf::Text(*new sf::Font());
        sf::RectangleShape green_rect, red_rect;
        Item::Object* item_target = nullptr;
        bool default_mode = false;

        void change_to(Item::Object* item){
            item_target = item;
            update();
        }

        void draw(){
            if(default_mode == false){
                float x, y, space = 20, space_x = 5;
                x = Main::window()->getSize().x - item_life.getGlobalBounds().size.x;
                y = Main::window()->getSize().y - item_life.getGlobalBounds().size.y;
                y -= space;
                x -= space_x;
                item_life.setPosition({x, y});

                x -= red_rect.getSize().x;
                x -= space_x;
                y = Main::window()->getSize().y - green_rect.getGlobalBounds().size.y;
                y -= space;
                green_rect.setPosition({x, y});
                red_rect.setPosition({x, y});

                x = Main::window()->getSize().x - item_extra_info.getGlobalBounds().size.x;
                y -= item_extra_info.getGlobalBounds().size.y;
                y -= space;
                x -= space_x;
                item_extra_info.setPosition({x, y});

                x = Main::window()->getSize().x - item_name.getGlobalBounds().size.x;
                y -= item_name.getGlobalBounds().size.y;
                y -= space;
                x -= space_x;
                item_name.setPosition({x, y});

                x = Main::window()->getSize().x - item_sprite.getGlobalBounds().size.x;
                y -= item_sprite.getGlobalBounds().size.y;
                y -= space;
                x -= space_x;
                item_sprite.setPosition({x, y});

                Main::window()->draw(item_life);
                Main::window()->draw(red_rect);
                Main::window()->draw(green_rect);
                Main::window()->draw(item_extra_info);
                Main::window()->draw(item_name);
                Main::window()->draw(item_sprite);
            }
        }

        void update(){
            if(item_target != nullptr){
                item_sprite.setTexture(*AllTextures::get(item_target->get_texture_name()));
                item_sprite.setTextureRect(item_target->get_texture_on(Item::Direction::right));
                item_sprite.setScale({2, 2});

                int SIZE = 25, green_rect_size = 1;
                int RECT_SIZE_X = 100, RECT_SIZE_Y = 10;
                sf::Font* FONT = FontManager::get_font(FontManager::Fonts::Arial);
                sf::Color COLOR = sf::Color::White;

                green_rect.setFillColor(sf::Color::Green);
                red_rect.setFillColor(sf::Color::Red);

                red_rect.setSize(sf::Vector2f(RECT_SIZE_X, RECT_SIZE_Y));
                green_rect.setSize(sf::Vector2f(RECT_SIZE_X, RECT_SIZE_Y));

                item_name.setCharacterSize(SIZE);
                item_name.setFont(*FONT);
                item_name.setFillColor(COLOR);
                item_name.setString(item_target->get_name());

                item_life.setCharacterSize(SIZE);
                item_life.setFont(*FONT);
                item_life.setFillColor(COLOR);
                if(item_target->get_type() == Item::Type::weapon || item_target->get_type() == Item::Type::guns){
                    sf::String info_bonus = Other::durability.at(Main::lang());
                    item_life.setString(std::to_string(item_target->get_durability()));
                    sf::String real_text = item_life.getString() + info_bonus;
                    item_life.setString(real_text);
                    green_rect_size = (item_target->get_durability()*100)/item_target->get_max_durability();
                }
                
                else if(item_target->get_type() == Item::Type::cartridge){
                    sf::String info_bonus = Other::cartridge.at(Main::lang());
                    item_life.setString(std::to_string(item_target->get_number(true)) + "/" + std::to_string(item_target->get_max_number(true)));
                    sf::String real_text = item_life.getString() + info_bonus;
                    item_life.setString(real_text);
                    green_rect_size = (item_target->get_number(true)*100)/item_target->get_max_number(true);
                }
                
                else{
                    sf::String info_bonus = Other::item_number.at(Main::lang());
                    item_life.setString(std::to_string(item_target->get_number()) + "/" + std::to_string(item_target->get_max_number()));
                    sf::String real_text = item_life.getString() + info_bonus;
                    item_life.setString(real_text);
                    green_rect_size = (item_target->get_number()*100)/item_target->get_max_number();
                }
                green_rect.setSize(sf::Vector2f(green_rect_size, RECT_SIZE_Y));

                item_extra_info.setCharacterSize(SIZE);
                item_extra_info.setFont(*FONT);
                item_extra_info.setFillColor(COLOR);
                item_extra_info.setString(item_target->get_extra_info());
                default_mode = false;
            }

            else{
                set_to_default();
            }
        }

        void set_to_default(){
            item_sprite.setTexture(*AllTextures::get("knife"));
            item_sprite.setTextureRect(sf::IntRect({0,0}, {32,32}));

            int SIZE = 30;
            int RECT_SIZE_X = 100, RECT_SIZE_Y = 10;
            sf::Font* FONT = FontManager::get_font(FontManager::Fonts::Arial);
            sf::Color COLOR = sf::Color::White;

            green_rect.setFillColor(sf::Color::Green);
            red_rect.setFillColor(sf::Color::Red);

            red_rect.setSize(sf::Vector2f(RECT_SIZE_X, RECT_SIZE_Y));
            green_rect.setSize(sf::Vector2f(RECT_SIZE_X, RECT_SIZE_Y));

            item_name.setCharacterSize(SIZE);
            item_name.setFont(*FONT);
            item_name.setFillColor(COLOR);
            item_name.setString("item");

            item_life.setCharacterSize(SIZE);
            item_life.setFont(*FONT);
            item_life.setFillColor(COLOR);
            item_life.setString("0/0");

            item_extra_info.setCharacterSize(SIZE);
            item_extra_info.setFont(*FONT);
            item_extra_info.setFillColor(COLOR);
            item_extra_info.setString("No info");
            default_mode = true;
        }
    }
}