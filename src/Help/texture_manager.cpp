#include "../../headers/Help/texture_manager.hpp"

// All textures on the game
std::map<std::string, sf::Texture*> all_textures;
std::map<std::pair<int, int>, sf::Texture*> all_structures_texture;

namespace AllTextures{

    void init(){
        Debugging::write("Intialising all textures", Debugging::get_state_of(Debugging::In_game));

        for(auto struct_texture : all_structures_texture)
            struct_texture.second = nullptr;
        all_structures_texture.clear();
        
        all_textures["the_apocalipse_map_elements"] = new sf::Texture();
        if(!all_textures["the_apocalipse_map_elements"]->loadFromFile("../res/images/World/Structures/the_apocalipse_map_elements.png"))
            Debugging::write("Map elements texture not loaded");

        all_textures["struct_entries"] = new sf::Texture();
        if(!all_textures["struct_entries"]->loadFromFile("../res/images/World/Structures/entries_types.png"))
            Debugging::write("Structures' entries not loaded");

        all_textures["struct_default_roof"] = new sf::Texture();
        if(!all_textures["struct_default_roof"]->loadFromFile("../res/images/World/Structures/Roofs/default_roof.png"))
            Debugging::write("Structures' default roof not loaded");

        all_textures["struct_default_wall"] = new sf::Texture();
        if(!all_textures["struct_default_wall"]->loadFromFile("../res/images/World/Structures/Walls/default_wall.png"))
            Debugging::write("Structures' default wall not loaded");

        all_textures["soldier"] = new sf::Texture();
        if(!all_textures["soldier"]->loadFromFile("../res/images/Entities/soldier.png"))
            Debugging::write("Soldier texture not loaded");

        all_textures["soldier_handling_guns"] = new sf::Texture();
        if(!all_textures["soldier_handling_guns"]->loadFromFile("../res/images/Entities/soldier_handling_guns.png"))
            Debugging::write("Soldier handling guns texture not loaded");

        all_textures["soldier_handling_item_or_weapon"] = new sf::Texture();
        if(!all_textures["soldier_handling_item_or_weapon"]->loadFromFile("../res/images/Entities/soldier_handling_item_or_weapon.png"))
            Debugging::write("Soldier handling item or weapon texture not loaded");

        all_textures["zombie"] = new sf::Texture();
        if(!all_textures["zombie"]->loadFromFile("../res/images/Entities/zombie.png"))
            Debugging::write("Zombie texture not loaded");

        all_textures["tomb"] = new sf::Texture();
        if(!all_textures["tomb"]->loadFromFile("../res/images/Entities/tomb.png"))
            Debugging::write("Tomb texture not loaded");

        all_textures["entity_default"] = new sf::Texture();
        if(!all_textures["entity_default"]->loadFromFile("../res/images/Entities/entity_default.png"))
            Debugging::write("Entity default texture not loaded");

        all_textures["inventory"] = new sf::Texture();
        if(!all_textures["inventory"]->loadFromFile("../res/images/Inventories/inventory.png"))
            Debugging::write("Inventory texture not loaded");

        all_textures["knife"] = new sf::Texture();
        if(!all_textures["knife"]->loadFromFile("../res/images/Items/knife.png"))
            Debugging::write("Knife texture not loaded");

        all_textures["pistolet"] = new sf::Texture();
        if(!all_textures["pistolet"]->loadFromFile("../res/images/Items/pistolet.png"))
            Debugging::write("Pistolet texture not loaded");

        all_textures["AK47"] = new sf::Texture();
        if(!all_textures["AK47"]->loadFromFile("../res/images/Items/AK47.png"))
            Debugging::write("AK47 texture not loaded");

        all_textures["cartridges"] = new sf::Texture();
        if(!all_textures["cartridges"]->loadFromFile("../res/images/Items/cartridges.png"))
            Debugging::write("Cartridges texture not loaded");

        all_textures["syringes"] = new sf::Texture();
        if(!all_textures["syringes"]->loadFromFile("../res/images/Items/syringes.png"))
            Debugging::write("Syringes texture not loaded");

        all_textures["natural_foods"] = new sf::Texture();
        if(!all_textures["natural_foods"]->loadFromFile("../res/images/Items/natural_foods.png"))
            Debugging::write("Natural texture not loaded");

        all_textures["artificial_foods"] = new sf::Texture();
        if(!all_textures["artificial_foods"]->loadFromFile("../res/images/Items/artificial_foods.png"))
            Debugging::write("Artificial foods texture not loaded");

        all_textures["bullets"] = new sf::Texture();
        if(!all_textures["bullets"]->loadFromFile("../res/images/Items/bullets.png"))
            Debugging::write("Bullets texture not loaded");

        all_textures["effects"] = new sf::Texture();
        if(!all_textures["effects"]->loadFromFile("../res/images/Others/effects.png"))
            Debugging::write("Effects texture not loaded");

        all_textures["satiation"] = new sf::Texture();
        if(!all_textures["satiation"]->loadFromFile("../res/images/Others/satiation.png"))
            Debugging::write("Satiation texture not loaded");

        all_textures["keys"] = new sf::Texture();
        if(!all_textures["keys"]->loadFromFile("../res/images/Others/Keys.png"))
            Debugging::write("Keys texture not loaded");
        
        Debugging::write("Intialising all textures terminated", Debugging::get_state_of(Debugging::In_game));
    }

    void destroy(){
        Debugging::write("Destroying all textures", Debugging::get_state_of(Debugging::In_game));
        for(auto& struct_texture : all_structures_texture){
            delete struct_texture.second;
            struct_texture.second = nullptr;
        }
        all_structures_texture.clear();
        for(auto& texture_ptr : all_textures){
            delete texture_ptr.second;
            texture_ptr.second = nullptr;
        }
        all_textures.clear();

        Debugging::write("Destroying all textures terminated", Debugging::get_state_of(Debugging::In_game));
    }

    sf::Texture* get(std::string name){
        sf::Texture* texture = nullptr;
        if(all_textures[name] != nullptr)
            texture = &*all_textures[name];
        return texture;
    }

    sf::Texture* get_structure(std::pair<int, int> point){
        return all_structures_texture[point];
    }

    void set_structure(std::pair<int, int> point, sf::Image image){
        all_structures_texture[point] = new sf::Texture(image);
    }

}