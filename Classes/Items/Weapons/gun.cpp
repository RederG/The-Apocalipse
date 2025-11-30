#include "gun.hpp"

#include "../../Others/bullet.hpp"

namespace Gun{
    Object* temporary_gun = nullptr;
    std::list<std::shared_ptr<Object>> all_guns;

    Object::Object(Nature type_of_gun){
        this->item_name = names.at(type_of_gun);
        this->gun_type = type_of_gun;
        this->id = to_string(Main::random_value() % 1000) + "_weapon_gun_" + names.at(type_of_gun).at(Main::english) + "_" + to_string(Main::random_value() % 1000);
        this->attack = 0;
        this->type = Item::Type::guns;
        this->number = 1;
        this->max_number = 1;
        this->durability = durabilities.at(type_of_gun);
        this->max_durability = durabilities.at(type_of_gun);
        this->item_texture = Weapon::sprite_sheet_position;
        this->fire_mode_texture_rect = textures_fire_mode;
        this->item_dependance = ammo_name.at(type_of_gun);
        this->ammo = 1;
        this->max_ammo = Max_ammo.at(type_of_gun);
        this->HandleZone = HandlingZone.at(this->gun_type);
        this->texture_name = textures_name.at(this->gun_type);
        this->set_durability();
        this->set_ammo();
    }

    Object::Object(Object* obj){}

    Object::Object(Item::Object& obj){}

    void Object::set_ammo(){
        std::string number_ammo = Probability::get_universe("ammo")->get_result();
        if(number_ammo == "max"){
            this->ammo = this->max_ammo;
        }
        if(number_ammo == "half"){
            this->ammo = this->max_ammo/2;
        }
        if(number_ammo == "low"){
            this->ammo = this->max_ammo/3;
        }
        if(number_ammo == "very low"){
            this->ammo = this->max_ammo/5;
        }

        int random;
        // Reducing or adding
        if(random % 2 == 0){
            this->ammo -= Main::random_value() % 5;
        }
        else if(random % 2 == 1){
            this->ammo += Main::random_value() % 5;
        }

        // Verifing the value
        if(this->ammo > this->max_ammo){
            this->ammo = max_ammo;
        }
        if(this->ammo <= 0){
            this->ammo = 1;
        }
    }

    void Object::use_item(Entity::Object& entity, bool special_use){
        if(special_use == false){
            this->fire();
        }
        if(special_use == true){
            this->recharge_with(Cartridge::get_temporary_cartridge());
        }
    }

    void Object::add_to(Container::Object* container, int location){
        if(location >= 0 && location < container->get_max_size() && container->get_item_at(location) == nullptr){
            container->get_contain()[location] = std::make_shared<Object>(*this);
        }
    }

    sf::IntRect Object::get_texture_on(Item::Direction direction){
        std::map<Item::Direction, sf::IntRect> rect;
        this->verify_fire_mode_clock();
        if(this->fire_mode == true){
            rect = this->fire_mode_texture_rect;
        }
        else{
            rect = this->item_texture;
        }
        return rect.at(direction);
    }

    void Object::fire(){
        if(this->ammo > 0 && this->can_fire == true){
            this->ammo -= 1;
            this->durability -= 1;
            set_temporary_gun(this);
            Bullet::new_bullet(this->gun_type);
            this->can_fire_clock.restart();
            reset_temporary_gun();
            this->can_fire = false;
            this->fire_mode = true;
        }
        if(this->ammo < 0){
            this->ammo = 0;
        }
        if(this->can_fire == false){
            float seconds = Gun::time_pause_after_fire.at(this->gun_type);
            if(Main::clock_seconds(seconds, this->can_fire_clock, false) == Main::Reached){
                this->can_fire = true;
                this->fire_mode = false;
            }
        }
    }

    void Object::verify_fire_mode_clock(){
        if(this->fire_mode == true){
            if(this->can_fire_clock.getElapsedTime().asSeconds() >= 0.1){
                this->fire_mode = false;
            }
        }
    }
    
    sf::Vector2f Object::get_scale(){
        return Gun::Scales.at(this->gun_type);
    }
    
    sf::Vector2f Object::set_position(Entity::Object &entity, sf::Sprite &sprite_to_draw){
        sf::Vector2f item_pos;
        if (entity.get_direction() == Entity::Direction::up){
            if(this->gun_type == Gun::Nature::pistolet){
                item_pos.x += 45;
                item_pos.y += 15;
            }
            else if(this->gun_type == Gun::Nature::AK_47){
                item_pos.x += 40;
                item_pos.y += 5;
            }
        }
        if (entity.get_direction() == Entity::Direction::down){
            if(this->gun_type == Gun::Nature::pistolet){
                item_pos.x += 12;
                item_pos.y += 40;
            }
            else if(this->gun_type == Gun::Nature::AK_47){
                item_pos.x += 10;
                item_pos.y += 35;
            }
        }
        if (entity.get_direction() == Entity::Direction::right){
            if(this->gun_type == Gun::Nature::pistolet){
                item_pos.x += 60;
                item_pos.y += 20;
            }
            else if(this->gun_type == Gun::Nature::AK_47){
                item_pos.x += 45;
                item_pos.y += 10;
            }
        }
        if (entity.get_direction() == Entity::Direction::left){
            if(this->gun_type == Gun::Nature::pistolet){
                item_pos.x -= 5;
                item_pos.y += 20;
            }
            else if(this->gun_type == Gun::Nature::AK_47){
                item_pos.y += 10;
            }
        }
        sprite_to_draw.setPosition(item_pos);
        return item_pos;
    }

    std::string Object::get_extra_info(){
        return std::to_string(this->ammo) + Item::Extra_info::gun.at(Main::lang());
    }

    std::string Object::get_file_name(){
        return Gun::names.at(this->gun_type).at(Main::english);
    }

    bool Object::recharge_with(Cartridge::Object* cartridge){
        if(this->get_dependance() == cartridge->get_name()){
            this->ammo = cartridge->get_number_ammo();
            cartridge->reduce(1);
            cartridge->next_ammo();
            Debugging::write("Recharging complete", Debugging::get_state_of(Debugging::In_game));
            return true;
        }
        else{ 
            Debugging::write("Cannot recharge with this", Debugging::get_state_of(Debugging::In_game));
            return false;
        }
    }

    void Object::set_ammo(int new_number_ammo){
        this->ammo = new_number_ammo;
    }

    void Object::save_to(std::string destination_path){
        std::string path = destination_path + "/" + this->id + ".item";

        std::fstream file(path, std::ios::out | std::ios::binary);
        if(file.is_open()){
            std::size_t id_size = this->id.size();
            file.write(reinterpret_cast<char*>(&id_size), sizeof(std::size_t));
            file.write(this->id.c_str(), sizeof(char)*id_size);

            file.write(reinterpret_cast<char*>(&this->number), sizeof(int));
            file.write(reinterpret_cast<char*>(&this->max_number), sizeof(int));
            file.write(reinterpret_cast<char*>(&this->durability), sizeof(unsigned int));
            file.write(reinterpret_cast<char*>(&this->max_durability), sizeof(unsigned int));
            file.write(reinterpret_cast<char*>(&this->ammo), sizeof(int));

            file.close();
        }
    }

    void Object::load_from(std::string item_file_path){
        std::fstream file(item_file_path, std::ios::in | std::ios::binary);
        if(file.is_open()){
            std::size_t id_size = this->id.size();
            file.read(reinterpret_cast<char*>(&id_size), sizeof(std::size_t));
            this->id.resize(id_size);
            file.read(&this->id[0], sizeof(char)*id_size);

            file.read(reinterpret_cast<char*>(&this->number), sizeof(int));
            file.read(reinterpret_cast<char*>(&this->max_number), sizeof(int));
            file.read(reinterpret_cast<char*>(&this->durability), sizeof(unsigned int));
            file.read(reinterpret_cast<char*>(&this->max_durability), sizeof(unsigned int));
            file.read(reinterpret_cast<char*>(&this->ammo), sizeof(int));

            file.close();
        }
    }

    void clear_list(){
        all_guns.clear();
    }
    
    void verify_guns_usability(){
        for(auto gun : all_guns){
            if(gun->is_usable() == false){
                all_guns.remove(gun);
                break;
            }
        }
    }

    void set_temporary_gun(Object* gun){
        temporary_gun = gun;
    }

    Object* get_temporary_gun(){
        return temporary_gun;
    }

    void reset_temporary_gun(){
        temporary_gun = nullptr;
    }

    void verify_all_fire_mode_clocks(){
        for(auto gun : all_guns){
            gun->verify_fire_mode_clock();
        }
    }
}