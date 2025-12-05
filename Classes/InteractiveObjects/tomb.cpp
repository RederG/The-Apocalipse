#include "tomb.hpp"
#include "../Entities/entity.hpp"

namespace Tomb{
    std::vector<Object*> container;

    Object::Object(){
        owner.name = "none";
        owner.id = "none";
        name = "none";
        id = "tomb_" + name + "_" + std::to_string(Main::random_value()%1000);
        element_type = MapElement::Type::World;
        interaction_type = InteractiveObjects::Type::container;
        inventory = new Container::Object(5);
    }

    Container::Object* Tomb::Object::get_inventory(){
        return this->inventory;
    }

    sf::Sprite Object::get_sprite(Map::Object* map, sf::Color color){
        sf::Sprite tomb_sprite(*AllTextures::get("tomb"));

        sf::Vector2f tomb_origin(tomb_sprite.getGlobalBounds().size.x/2, tomb_sprite.getGlobalBounds().size.y);
        tomb_sprite.setOrigin(tomb_origin);

        sf::Vector2f tomb_scale(Entity::ENTITY_SCALE.x*map->get_scale().x, Entity::ENTITY_SCALE.y*map->get_scale().y);
        tomb_sprite.setScale(tomb_scale);

        tomb_sprite.setPosition(this->get_window_position_on(map));
        return tomb_sprite;
    }

    sf::RectangleShape Object::get_collision_rect(Map::Object* map, sf::Color color){
        sf::Sprite tomb_sprite = this->get_sprite(map);
        sf::Vector2f tomb_scale = tomb_sprite.getScale();
        sf::RectangleShape rect;

        sf::Vector2f rect_size = tomb_sprite.getGlobalBounds().size;
        rect_size.x /= 2; rect_size.x -= 2*tomb_scale.x;
        rect_size.y /= 2; rect_size.y -= 2*tomb_scale.y;
        rect.setSize(rect_size);

        sf::Vector2f rect_origin;
        rect_origin.x = rect.getSize().x/2;
        rect_origin.y = rect.getSize().y + tomb_scale.y;
        rect.setOrigin(rect_origin);

        rect.setPosition(tomb_sprite.getPosition());
        rect.setFillColor(color);
        return rect;
    }

    sf::Vector2f Object::get_map_position(){
        return this->map_position;
    }

    std::string Object::get_owner_name(){
        return owner.name;
    }

    std::string Object::get_owner_id(){
        return owner.id;
    }

    std::string Object::get_name(){
        return this->name;
    }

    std::string Object::get_id(){
        return this->id;
    }

    void Object::set_map_position_to(sf::Vector2f new_position){
        this->map_position = new_position;
    }

    void Object::set_owner_name_to(std::string new_name){
        this->owner.name = new_name;
    }

    void Object::set_owner_id_to(std::string new_id){
        this->owner.id = new_id;
        this->id = "tomb_" + this->owner.id + "_" + to_string(Main::random_value()%1000);
    }

    void Object::set_name_to(std::string new_name){
        this->name = new_name;
    }

    bool Object::verify_life_time(){
        if(Main::clock_seconds(1, this->clock_before_disapear) == Main::Reached){
            this->life -= 1;
        }
        if(this->life <= 0){
            container.erase(container.begin());
            return true;
        }
        else return false;
    }

    void Object::save_to(std::string destination){
        std::string path = destination + "/" + this->id + ".tomb";

        std::fstream file(path, std::ios::out | std::ios::binary);
        if(file.is_open()){
            std::size_t owner_name_length = this->owner.name.size();
            file.write(reinterpret_cast<char*>(&owner_name_length), sizeof(std::size_t));
            file.write(this->owner.name.c_str(), sizeof(char)*owner_name_length);

            std::size_t owner_id_length = this->owner.id.size();
            file.write(reinterpret_cast<char*>(&owner_id_length), sizeof(std::size_t));
            file.write(this->owner.id.c_str(), sizeof(char)*owner_id_length);

            std::size_t name_length = this->name.size();
            file.write(reinterpret_cast<char*>(&name_length), sizeof(std::size_t));
            file.write(this->name.c_str(), sizeof(char)*name_length);

            std::size_t id_length = this->id.size();
            file.write(reinterpret_cast<char*>(&id_length), sizeof(std::size_t));
            file.write(this->id.c_str(), sizeof(char)*id_length);

            file.write(reinterpret_cast<char*>(&this->life), sizeof(int));

            file.write(reinterpret_cast<char*>(&this->map_position.x), sizeof(float));
            file.write(reinterpret_cast<char*>(&this->map_position.y), sizeof(float));

            file.close();
        }
    }

    void Object::load_from(std::string tomb_file_path){
        std::fstream file(tomb_file_path, std::ios::in | std::ios::binary);
        if(file.is_open()){
            std::size_t owner_name_length = 0;
            file.read(reinterpret_cast<char*>(&owner_name_length), sizeof(std::size_t));
            this->owner.name.resize(owner_name_length);
            file.read(&this->owner.name[0], sizeof(char)*owner_name_length);

            std::size_t owner_id_length = 0;
            file.read(reinterpret_cast<char*>(&owner_id_length), sizeof(std::size_t));
            this->owner.id.resize(owner_id_length);
            file.read(&this->owner.id[0], sizeof(char)*owner_id_length);

            std::size_t name_length = 0;
            file.read(reinterpret_cast<char*>(&name_length), sizeof(std::size_t));
            this->name.resize(name_length);
            file.read(&this->name[0], sizeof(char)*name_length);

            std::size_t id_length = 0;
            file.read(reinterpret_cast<char*>(&id_length), sizeof(std::size_t));
            this->id.resize(id_length);
            file.read(&this->id[0], sizeof(char)*id_length);

            file.read(reinterpret_cast<char*>(&this->life), sizeof(int));

            file.read(reinterpret_cast<char*>(&this->map_position.x), sizeof(float));
            file.read(reinterpret_cast<char*>(&this->map_position.y), sizeof(float));
            file.close();
        }
    }

    void set_tomb_of(Entity::Object &entity){
        Object* tomb = new Object();
        tomb->set_name_to(entity.get_name() + "_tomb");
        tomb->set_owner_name_to(entity.get_name());
        tomb->set_owner_id_to(entity.get_id());
        tomb->set_map_position_to(entity.get_map_position());

        entity.get_inventory()->transfer_items_to(tomb->get_inventory());
        
        container.push_back(tomb);
    }
    
    bool verify_all_life_time(){
        bool there_is_a_tomb_which_disapear = false;
        for(auto tomb : container){
            if(tomb->verify_life_time() == true){
                there_is_a_tomb_which_disapear = true;
                break;
            }
        }
        return there_is_a_tomb_which_disapear;
    }

    void save_all_to(std::string destination_path, std::string inventories_path, std::string items_path){
        std::filesystem::remove_all(destination_path);
        std::filesystem::create_directories(destination_path);

        if(std::filesystem::exists(inventories_path) && std::filesystem::exists(items_path)){
            for(auto tomb : container){
                tomb->save_to(destination_path);
                tomb->get_inventory()->save_to(inventories_path, tomb->get_id(), items_path);
            }
        }
        else 
            Debugging::write("Inventory saving impossible (Saving tombs)");
    }

    void load_all_from(std::string tombs_files_path, std::string inventories_path, std::string items_path){
        if(!std::filesystem::exists(tombs_files_path))
            Debugging::write(tombs_files_path + " does not exists(Saving tombs)");
        else if(std::filesystem::exists(inventories_path) && std::filesystem::exists(items_path)){
            std::list<std::string> all_tombs;
            Main::get_files_in(tombs_files_path + "/", all_tombs);

            for(std::string tomb_file : all_tombs){
                Tomb::Object* tomb = new Tomb::Object();
                container.push_back(tomb);
                tomb->load_from(tombs_files_path + "/" + tomb_file);
                tomb->get_inventory()->load_from(inventories_path + "/" + tomb->get_id() + ".inventory", items_path);
            }
        }
        else 
            Debugging::write("Inventory saving impossible(Saving tombs)");
    }

}