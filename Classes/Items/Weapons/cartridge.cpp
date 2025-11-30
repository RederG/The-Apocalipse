#include "cartridge.hpp"

namespace Cartridge{
    Object* temporary_munition = nullptr;
    std::vector<std::shared_ptr<Object>> all_cartridges;
    
    Object::Object(Type type_chosen){
        this->item_name = names.at(type_chosen);
        this->id = to_string(Main::random_value() % 1000) + "_" + this->item_name.at(Main::english) + "_" + to_string(Main::random_value() % 1000);
        this->attack = Cartridge::damage.at(type_chosen);
        this->direction = Item::Direction::left;
        this->item_texture = Cartridge::textures.at(type_chosen);
        this->max_number = Cartridge::max_number.at(type_chosen);
        this->max_ammo = Cartridge::max_ammo.at(type_chosen);
        this->cartridge_type = type_chosen;
        this->type = Item::Type::cartridge;
        
        sf::Vector2f handlezone = sf::Vector2f(16, 22);
        this->HandleZone = {
            {Item::Direction::left, handlezone},
            {Item::Direction::right, handlezone},
            {Item::Direction::down, handlezone},
            {Item::Direction::up, handlezone}
        };
        this->texture_name = "cartridges";
        this->set_number();
        this->set_number_ammo();
    }

    Object::Object(Object& obj){
        this->item_name = obj.item_name;
        this->id = obj.id;
        this->attack = obj.attack;
        this->direction = obj.direction;
        this->item_texture = obj.item_texture;
        this->max_number = obj.max_number;
        this->max_ammo = obj.max_ammo;
        this->cartridge_type = obj.cartridge_type;
        this->type = obj.type;
        this->HandleZone = obj.HandleZone;
        this->number = obj.number;
        this->all_ammo = obj.all_ammo;
        this->max_ammo = obj.max_ammo;
        this->texture_name = obj.texture_name;
    }

    Object::Object(Object* ptr){
        this->item_name = ptr->item_name;
        this->id = ptr->id;
        this->attack = ptr->attack;
        this->direction = ptr->direction;
        this->item_texture = ptr->item_texture;
        this->max_number = ptr->max_number;
        this->max_ammo = ptr->max_ammo;
        this->cartridge_type = ptr->cartridge_type;
        this->type = ptr->type;
        this->HandleZone = ptr->HandleZone;
        this->number = ptr->number;
        this->all_ammo = ptr->all_ammo;
        this->max_ammo = ptr->max_ammo;
        this->texture_name = ptr->texture_name;
    }

    void Object::set_number(){
        int random;
        std::string munition_number = Probability::get_universe("cartridge_number")->get_result();
        // Setting the value using probability
        if(munition_number == "max"){
            this->number = this->get_max_number();
        }
        if(munition_number == "half"){
            this->number = (this->get_max_number() / 2);
        }
        if(munition_number == "low"){
            this->number = (this->get_max_number() / 3);
        }
        if(munition_number == "very low"){
            this->number = (this->get_max_number() / 5);
        }

        // Reducing or adding
        if(random % 2 == 0){
            this->number -= Main::random_value() % 5;
        }
        else if(random % 2 == 1){
            this->number += Main::random_value() % 5;
        }

        // Verifing the value
        if(this->number > this->max_number){
            this->number = max_number;
        }
        if(this->number <= 0){
            this->number = 1;
        }
    }

    void Object::set_number_ammo(){
        for(int i = 0; i < this->number; i++){
            std::string ammo_number = Probability::get_universe("ammo")->get_result();
            // Setting the value using probability
            if(ammo_number == "max"){
                this->all_ammo.push(this->max_ammo);
            }
            if(ammo_number == "half"){
                this->all_ammo.push(this->max_ammo / 2);
            }
            if(ammo_number == "low"){
                this->all_ammo.push(this->max_ammo / 3);
            }
            if(ammo_number == "very low"){
                this->all_ammo.push(1);
            }

            int random;
            // Reducing or adding
            if(random % 2 == 0){
                int real_number_ammo = this->all_ammo.top() - Main::random_value() % 5;
                this->all_ammo.pop();
                this->all_ammo.push(real_number_ammo);
            }
            else if(random % 2 == 1){
                int real_number_ammo = this->all_ammo.top() + Main::random_value() % 5;
                this->all_ammo.pop();
                this->all_ammo.push(real_number_ammo);
            }

            // Verifing the value
            if(this->all_ammo.top() > this->max_ammo){
                this->all_ammo.pop();
                this->all_ammo.push(this->max_ammo);
            }
            if(this->all_ammo.top() <= 0){
                this->all_ammo.pop();
                this->all_ammo.push(1);
            }
        }
    }

    int Object::get_number_ammo(){
        return this->all_ammo.top();
    }

    Type Object::get_cartridge_type(){
        return this->cartridge_type;
    }

    std::string Object::get_extra_info(){
        std::string attack_txt = std::to_string(this->attack) + Item::Extra_info::weapon.at(Main::lang());
        std::string number_txt = std::to_string(this->number) + Item::Extra_info::munition_number.at(Main::lang());
        return attack_txt + ", " + number_txt;
    }

    sf::Vector2f Object::set_position(Entity::Object &entity, sf::Sprite &sprite_to_draw){
        sf::Vector2f item_pos;
        if (entity.get_direction() == Entity::Direction::up){
            item_pos.x += 50;
            item_pos.y += 20;
        }
        else if (entity.get_direction() == Entity::Direction::down){
            item_pos.x += 14;
            item_pos.y += 40;
        }
        else if (entity.get_direction() == Entity::Direction::right){
            item_pos.x += 64;
            item_pos.y += 30;
        }
        else if (entity.get_direction() == Entity::Direction::left){
            item_pos.y += 30;
        }
        sprite_to_draw.setPosition(item_pos);
        return item_pos;
    }

    void Object::use_item(Entity::Object &entity, bool special_use){
        auto inventory = entity.get_inventory();
        if(inventory != nullptr && special_use == false){
            for(int location = 0; location <= inventory->get_max_size(); location++){
                if(inventory->get_item_at(location) != nullptr && inventory->get_item_at(location)->get_dependance() == this->get_name()){
                    Cartridge::set_temporary_cartridge(this);
                    inventory->get_item_at(location)->use_item(entity, true);
                    Cartridge::reset_temporary_cartridge();
                    Debugging::write("Destroying temporary munition : OK", true);
                    break;
                }
            }
        }
    }

    void Object::add_to(Container::Object* container, int location){
        Cartridge::Object* cartridge = nullptr;
        for(int container_location = 0; container_location < container->get_max_size(); container_location++){
            if(container->get_item_at(container_location) != nullptr && typeid(*this) == typeid(*container->get_item_at(container_location))){
                for(auto munition_ptr : all_cartridges){
                    if(munition_ptr->get_name() == container->get_item_at(container_location)->get_name() && munition_ptr->get_id() == container->get_item_at(container_location)->get_id()){
                        cartridge = &*munition_ptr;
                        break;
                    }
                }
                if(cartridge->cartridge_type != this->cartridge_type)
                    cartridge = nullptr;
            }
        }
        if(cartridge != nullptr){
            int munition_to_add = cartridge->max_number - cartridge->number;
            for(int i = 0; i < munition_to_add; i++){
                if(this->all_ammo.empty() == false){
                    cartridge->number++;
                    cartridge->all_ammo.push(this->all_ammo.top());
                    this->all_ammo.pop();
                    this->number--;
                }
                else{
                    break;
                }
            }
            if(this->number > 0 && !container->is_full()){
                for(int container_location = 0; container_location < container->get_max_size(); container_location++){
                    if(container->get_contain()[container_location] == nullptr){
                        container->get_contain()[container_location] = std::make_shared<Cartridge::Object>(*this);
                        break;
                    }
                }
            }
        }
        else if(container->get_contain()[location] == nullptr){
            container->get_contain()[location] = std::make_shared<Cartridge::Object>(*this);
        }
    }

    bool Object::add_to_container(Container::Object* container, bool aleatory){
        bool added = false;
        std::shared_ptr<Object> same_munition = nullptr;
        for(int container_location = 0; container_location < container->get_max_size(); container_location++){
            if(container->get_item_at(container_location) != nullptr && typeid(*this) == typeid(*container->get_item_at(container_location))){
                same_munition = std::static_pointer_cast<Object>(container->get_contain()[container_location]);
                if(same_munition != nullptr && same_munition->cartridge_type != this->cartridge_type){
                    same_munition = nullptr;
                }
            }
        }
        if(same_munition == nullptr && container->is_full() == false){
            if(aleatory == false){
                for(int location = 0; location < container->get_max_size(); location++){
                    if(container->get_contain()[location] == nullptr){
                        container->get_contain()[location] = get_cartridge(this->get_name(), this->get_id());
                        added = true;
                        break;
                    }
                }
            }
            else{
                while(added == false){
                    int location = std::rand() % container->get_max_size();
                    if(container->get_contain()[location] == nullptr){
                        added = true;
                        container->get_contain()[location] = get_cartridge(this->get_name(), this->get_id());
                    }
                }
            }
        }
        else if(same_munition != nullptr){
            int munition_to_add = same_munition->max_number - same_munition->number;
            for(int i = 0; i < munition_to_add; i++){
                if(this->all_ammo.empty() == false){
                    same_munition->number++;
                    same_munition->all_ammo.push(this->all_ammo.top());
                    this->all_ammo.pop();
                    this->number--;
                }
                else{
                    break;
                }
            }
            if(this->number > 0 && !container->is_full()){
                for(int container_location = 0; container_location < container->get_max_size(); container_location++){
                    if(container->get_contain()[container_location] == nullptr){
                        container->get_contain()[container_location] = get_cartridge(this->get_name(), this->get_id());
                        break;
                    }
                }
            }
            added = true;
        }

        return added;
    }

    unsigned int Object::get_number(bool special){
        int nb = 0;
        if(special == false){
            nb = this->number;
        }
        else{ 
            if(this->all_ammo.empty() == false)
                nb = this->all_ammo.top();
            if(this->all_ammo.empty() == true)
                nb = 0;
        }
        return nb;
    }

    unsigned int Object::get_max_number(bool special){
        if(special == false){
            return this->max_number;
        }
        else{ 
            return this->max_ammo;
        }
    }

    std::string Object::get_file_name(){
        std::string file_name;
        if(this->cartridge_type == Cartridge::Type::pistolet)
            file_name = Cartridge::Names::cartridge_pistolet.at(Main::english);
        if(this->cartridge_type == Cartridge::Type::AK_47)
            file_name = Cartridge::Names::cartridge_AK47.at(Main::english);
        return file_name;
    }

    void Object::next_ammo(){
        this->all_ammo.pop();
        if(this->all_ammo.empty() == true)
            durability = 0;
    }
    
    int Object::get_max_ammo(){
        return this->max_ammo;
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

            std::stack<int> all_ammos = this->all_ammo;
            std::stack<int> inverse_ammos;
            for(int i = 0; i < this->number; i++){
                inverse_ammos.push(all_ammos.top());
                all_ammos.pop();
            }
            for(int i = 0; i < this->number; i++){
                int ammo = inverse_ammos.top();
                file.write(reinterpret_cast<char*>(&ammo), sizeof(int));
                inverse_ammos.pop();
            }
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

            this->all_ammo = std::stack<int>();
            for(int i = 0; i < this->number; i++){
                int ammo = 0;
                file.read(reinterpret_cast<char*>(&ammo), sizeof(int));
                this->all_ammo.push(ammo);
            }

            file.close();
        }
    }

    void set_temporary_cartridge(Cartridge::Object* munition){
        temporary_munition = munition;
    }

    Cartridge::Object* get_temporary_cartridge(){
        return temporary_munition;
    }

    void reset_temporary_cartridge(){
        temporary_munition = nullptr;
    }

    Object* create_new(Type type_chosen){
        std::shared_ptr<Object> munition_ptr = std::make_shared<Object>(*new Cartridge::Object(type_chosen));
        all_cartridges.push_back(munition_ptr);
        return &*munition_ptr;
    }

    std::shared_ptr<Object> get_cartridge(std::string name, std::string id){
        std::shared_ptr<Object> cartridge_to_get = nullptr;
        for(auto& cartridge_ptr : all_cartridges){
            if(cartridge_ptr->get_name() == name && cartridge_ptr->get_id() == id){
                cartridge_to_get = cartridge_ptr;
                break;
            }
        }
        return cartridge_to_get;
    }

    void clear(){
        for(auto& munition : all_cartridges){
            munition.reset();
        }
        all_cartridges.clear();
    }

}