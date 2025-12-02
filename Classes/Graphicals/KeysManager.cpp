#include "KeysManager.hpp"

namespace KeysManager{
    sf::Vector2f keys_scale({1.5, 1.5});

    std::map<Action, int> all_actions_to_keys{
        {Action::move_up,       int(sf::Keyboard::Key::Z)},
        {Action::move_left,     int(sf::Keyboard::Key::Q)},
        {Action::move_down,     int(sf::Keyboard::Key::S)},
        {Action::move_right,    int(sf::Keyboard::Key::D)},
        {Action::remove_item,   int(sf::Keyboard::Key::R)},
        {Action::take_item,     int(sf::Keyboard::Key::T)},
        {Action::use_item,      int(sf::Keyboard::Key::Space)},
        {Action::pause,         int(sf::Keyboard::Key::Escape)},
    };

    std::map<Action, std::string> all_actions_to_special_keys{
        {Action::open_inventory,    "ctrl"},
        {Action::close_inventory,   "ctrl"},
        {Action::interaction,       "alt"},
    };

    void draw_key(Action action, sf::Vector2f position){
        sf::Sprite key_sprite(*AllTextures::get("keys"));
        if(all_actions_to_keys.find(action) != all_actions_to_keys.end())
            key_sprite.setTextureRect(textures_keys.at(all_actions_to_keys.at(action)));
        else if(all_actions_to_special_keys.find(action) != all_actions_to_special_keys.end())
            key_sprite.setTextureRect(textures_special_keys.at(all_actions_to_special_keys.at(action)));
        key_sprite.setScale(keys_scale);
        key_sprite.setOrigin({0, 0});
        key_sprite.setPosition(position);

        sf::Text key_info(*FontManager::get_font(FontManager::Fonts::Arial), " " + KeysManager::infos.at(action).at(Main::lang()), 20);
        key_info.setFillColor(sf::Color::White);
        key_info.setOrigin({0, key_info.getGlobalBounds().size.y/2});
        position.x += key_sprite.getGlobalBounds().size.x;
        position.y += key_sprite.getGlobalBounds().size.y/2;
        key_info.setPosition(position);

        Main::window()->draw(key_sprite);
        Main::window()->draw(key_info);
    }

    bool try_to(Action action, bool ctrl, bool alt, bool shift){
        if(all_actions_to_keys.find(action) != all_actions_to_keys.end()){
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key(all_actions_to_keys[action])))
                return true;
            else
                return false;
        }
        else if(ctrl && all_actions_to_special_keys.find(action) != all_actions_to_special_keys.end() && all_actions_to_special_keys[action] == "ctrl")
            return true;
        else if(alt && all_actions_to_special_keys.find(action) != all_actions_to_special_keys.end() && all_actions_to_special_keys[action] == "alt")
            return true;
        else
            return false;
    }

    bool can_do(Action action, Player::Object* player, int min_distance){
        Player::State state = player->get_state();
        if(((action == move_down || action == move_up || action == move_right || action == move_left) 
            && state == Player::State::Playing) || action == pause)
            return true;
        else if((action == move_right || action == move_left) && (state == Player::State::Look_at_Inventory
            || state == Player::State::Interacting_with_objects))
            return true;
        else if(action == open_inventory
                && (state == Player::State::Playing || state == Player::State::Interacting_with_objects))
            return true;
        else if(action == close_inventory 
                && (state == Player::State::Look_at_Inventory || state == Player::State::Interacting_with_objects))
            return true;
        else if((action == remove_item || action == Action::take_item) 
                && (state == Player::State::Look_at_Inventory || state == Player::State::Interacting_with_objects)
                && player->get_inventory()->get_current_item() != nullptr)
            return true;
        else if(action == interaction && (player->get_nearest_interactive_object() != nullptr))
            return true;
        else
            return false;
    }

    void draw_all(Player::Object* player, int minimal_distance){
        sf::Sprite key_sprite(*AllTextures::get("keys"));
        key_sprite.setScale(keys_scale);
        key_sprite.setOrigin({0, 0});
        key_sprite.setTextureRect(textures_keys.at(int(sf::Keyboard::Key::A)));

        float size_y = key_sprite.getGlobalBounds().size.y;
        
        sf::Vector2f position({0.0f, float(Main::window()->getSize().y - size_y)});
        for(std::pair<Action, std::string> action : all_actions_to_special_keys){
            if(can_do(action.first, player, minimal_distance)){
                draw_key(action.first, position);
                position.y -= size_y;
            }
        }
        for(std::pair<Action, int> action : all_actions_to_keys){
            if(can_do(action.first, player, minimal_distance)){
                draw_key(action.first, position);
                position.y -= size_y;
            }
        }
    }

};