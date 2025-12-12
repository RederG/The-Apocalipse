#ifndef __KEYS_MANAGER__
    #define __KEYS_MANAGER__

    #include "../main.hpp"
    #include "../Entities/player.hpp"
    
    namespace KeysManager{
        enum Action{
            move_up, move_down, move_right, move_left,
            open_inventory, close_inventory,
            interact,
            use_item, remove_item, take_item,
            pause
        };

        const std::map<Action, std::map<Main::Languages, sf::String>> infos{
            {move_up, {
                {Main::english, L"Move up"},
                {Main::french, L"Diriger vers le haut"},
                {Main::malagasy, L"Mandeha any ambony"},
            }},

            {move_down, {
                {Main::english, L"Move down"},
                {Main::french, L"Diriger vers le bas"},
                {Main::malagasy, L"Mandeha any ambany"},
            }},

            {move_left, {
                {Main::english, L"Move left"},
                {Main::french, L"Diriger vers la gauche"},
                {Main::malagasy, L"Mandeha any havanana"},
            }},

            {move_right, {
                {Main::english, L"Move right"},
                {Main::french, L"Diriger vers la droite"},
                {Main::malagasy, L"Mandeha any havia"},
            }},

            {open_inventory, {
                {Main::english, L"Open your inventory"},
                {Main::french, L"Ouvrir ton inventaire"},
                {Main::malagasy, L"Sokafana ny kitaponao"},
            }},

            {close_inventory, {
                {Main::english, L"Close your inventory"},
                {Main::french, L"Fermer ton inventaire"},
                {Main::malagasy, L"Idina ny kitaponao"},
            }},

            {use_item, {
                {Main::english, L"Use the object"},
                {Main::french, L"Utiliser l'objet"},
                {Main::malagasy, L"Ampiasaina ilay zavatra"},
            }},

            {remove_item, {
                {Main::english, L"Remove the object"},
                {Main::french, L"Enlever l'objet"},
                {Main::malagasy, L"Esorina ilay zavatra"},
            }},

            {take_item, {
                {Main::english, L"Take the object"},
                {Main::french, L"Prendre l'objet"},
                {Main::malagasy, L"Alaina ilay zavatra"},
            }},

            {interact, {
                {Main::english, L"Interact"},
                {Main::french, L"Interagir"},
                {Main::malagasy, L"Ampiasaina"},
            }},

            {pause, {
                {Main::english, L"Pause"},
                {Main::french, L"Pause"},
                {Main::malagasy, L"Miala sasatra"},
            }},
        };

        const std::map<int, sf::IntRect> textures_keys{
            {int(sf::Keyboard::Key::A), sf::IntRect({0,0}, {32,32})},
            {int(sf::Keyboard::Key::B), sf::IntRect({32,0}, {32,32})},
            {int(sf::Keyboard::Key::C), sf::IntRect({32*2,0}, {32,32})},
            {int(sf::Keyboard::Key::D), sf::IntRect({32*3,0}, {32,32})},
            {int(sf::Keyboard::Key::E), sf::IntRect({32*4,0}, {32,32})},
            {int(sf::Keyboard::Key::F), sf::IntRect({32*5,0}, {32,32})},
            {int(sf::Keyboard::Key::G), sf::IntRect({32*6,0}, {32,32})},
            {int(sf::Keyboard::Key::H), sf::IntRect({0,32}, {32,32})},
            {int(sf::Keyboard::Key::I), sf::IntRect({32,32}, {32,32})},
            {int(sf::Keyboard::Key::J), sf::IntRect({32*2,32}, {32,32})},
            {int(sf::Keyboard::Key::K), sf::IntRect({32*3,32}, {32,32})},
            {int(sf::Keyboard::Key::L), sf::IntRect({32*4,32}, {32,32})},
            {int(sf::Keyboard::Key::M), sf::IntRect({32*5,32}, {32,32})},
            {int(sf::Keyboard::Key::N), sf::IntRect({32*6,32}, {32,32})},
            {int(sf::Keyboard::Key::O), sf::IntRect({0,32*2}, {32,32})},
            {int(sf::Keyboard::Key::P), sf::IntRect({32,32*2}, {32,32})},
            {int(sf::Keyboard::Key::Q), sf::IntRect({32*2,32*2}, {32,32})},
            {int(sf::Keyboard::Key::R), sf::IntRect({32*3,32*2}, {32,32})},
            {int(sf::Keyboard::Key::S), sf::IntRect({32*4,32*2}, {32,32})},
            {int(sf::Keyboard::Key::T), sf::IntRect({32*5,32*2}, {32,32})},
            {int(sf::Keyboard::Key::U), sf::IntRect({32*6,32*2}, {32,32})},
            {int(sf::Keyboard::Key::V), sf::IntRect({0,32*3}, {32,32})},
            {int(sf::Keyboard::Key::W), sf::IntRect({32,32*3}, {32,32})},
            {int(sf::Keyboard::Key::X), sf::IntRect({32*2,32*3}, {32,32})},
            {int(sf::Keyboard::Key::Y), sf::IntRect({32*3,32*3}, {32,32})},
            {int(sf::Keyboard::Key::Z), sf::IntRect({32*4,32*3}, {32,32})},
            {int(sf::Keyboard::Key::Space), sf::IntRect({32*3,32*5}, {32,32})},
            {int(sf::Keyboard::Key::Num0), sf::IntRect({0,32*4}, {32,32})},
            {int(sf::Keyboard::Key::Num1), sf::IntRect({32,32*4}, {32,32})},
            {int(sf::Keyboard::Key::Num2), sf::IntRect({32*2,32*4}, {32,32})},
            {int(sf::Keyboard::Key::Num3), sf::IntRect({32*3,32*4}, {32,32})},
            {int(sf::Keyboard::Key::Num4), sf::IntRect({32*4,32*4}, {32,32})},
            {int(sf::Keyboard::Key::Num5), sf::IntRect({32*5,32*4}, {32,32})},
            {int(sf::Keyboard::Key::Num6), sf::IntRect({32*6,32*4}, {32,32})},
            {int(sf::Keyboard::Key::Num7), sf::IntRect({0,32*5},    {32,32})},
            {int(sf::Keyboard::Key::Num8), sf::IntRect({32,32*5},   {32,32})},
            {int(sf::Keyboard::Key::Num9), sf::IntRect({32*2,32*5}, {32,32})},
            {int(sf::Keyboard::Key::Numpad0), sf::IntRect({0,32*4}, {32,32})},
            {int(sf::Keyboard::Key::Numpad1), sf::IntRect({32,32*4}, {32,32})},
            {int(sf::Keyboard::Key::Numpad2), sf::IntRect({32*2,32*4}, {32,32})},
            {int(sf::Keyboard::Key::Numpad3), sf::IntRect({32*3,32*4}, {32,32})},
            {int(sf::Keyboard::Key::Numpad4), sf::IntRect({32*4,32*4}, {32,32})},
            {int(sf::Keyboard::Key::Numpad5), sf::IntRect({32*5,32*4}, {32,32})},
            {int(sf::Keyboard::Key::Numpad6), sf::IntRect({32*6,32*4}, {32,32})},
            {int(sf::Keyboard::Key::Numpad7), sf::IntRect({0,32*5},    {32,32})},
            {int(sf::Keyboard::Key::Numpad8), sf::IntRect({32,32*5},   {32,32})},
            {int(sf::Keyboard::Key::Numpad9), sf::IntRect({32*2,32*5}, {32,32})},
            {int(sf::Keyboard::Key::Escape), sf::IntRect({32*5,32*5}, {32,32})},
        };

        const std::map<std::string, sf::IntRect> textures_special_keys{
            {"ctrl", sf::IntRect({32*5,32*3}, {32,32})},
            {"alt", sf::IntRect({32*4,32*5}, {32,32})},
            {"shift", sf::IntRect({32*6,32*3}, {32,32})},
        };

        bool try_to(Action action, bool ctrl = false, bool alt = false, bool shift = false);
        bool can_do(Action action, Player::Object* player, int minimal_distance);
        void draw_key(Action action, sf::Vector2f position);
        void draw_all(Player::Object* player, int minimal_distance);
    };
#endif