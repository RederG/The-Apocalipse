#include "../../headers/Others/container.hpp"
#include "../../headers/Items/all_items.hpp"

namespace Container{

    Object::Object(int max_size){
        this->max_size = max_size;
        this->current_location = 0;
        this->contain.clear();
    }

    int Object::get_max_size(){
        return this->max_size;
    }

    int Object::get_current_location(){
        return this->current_location;
    }

    Item::Object* Object::get_item_at(int location){
        Item::Object* item = nullptr;
        if(location >= 0 && location < this->max_size && this->contain[location] != nullptr){
            item = &*this->contain.at(location);
        }
        else if(location < 0 && this->contain[0] != nullptr){
            item = &*this->contain.at(0);
        }
        else if(location >= this->max_size && this->contain[this->max_size - 1] != nullptr){
            item = &*this->contain[this->max_size - 1];
        }
        return item;
    }

    Item::Object* Object::get_current_item(){
        Item::Object* item = nullptr;
        if(this->contain[this->current_location] != nullptr){
            item = &*this->contain[this->current_location];
        }
        return item;
    }

    std::map<int, std::shared_ptr<Item::Object>>& Object::get_contain(){
        return this->contain;
    }

    bool Object::is_full(){
        bool full = false;
        int size = 0;
        for(int location = 0; location < this->max_size; location++){
            if(this->contain[location] != nullptr){
                size++;
            }
        }
        if(size >= this->max_size)
            full = true;
        return full;
    }

    bool Object::is_empty(){
        bool empty = true;
        for(int location = 0; location < this->max_size; location++){
            if(this->contain[location] != nullptr){
                empty = false;
                break;
            }
        }
        return empty;
    }

    Item::Object* Object::remove_item_on(int location){
        Item::Object* item_deleted = nullptr;
        if(location >= 0 && location < this->max_size && this->contain[location] != nullptr){
            item_deleted = &*this->contain[location];
            this->contain[location].reset();
        }
        return item_deleted;
    }

    Item::Object* Object::remove_current_item(){
        Item::Object* item_deleted = this->remove_item_on(this->current_location);
        return item_deleted;
    }

    void Object::move_selector_to(Direction direction){
        if(direction == Direction::Left)
            this->current_location -= 1;
        if(direction == Direction::Right)
            this->current_location += 1;
        if(this->current_location >= this->max_size)
            this->current_location = this->max_size - 1;
        if(this->current_location < 0)
            this->current_location = 0;
    }

    void Object::transfer_items_to(Object* container){
        if(this->is_empty() == false && container->is_empty() == true && container->max_size >= this->max_size){
            for(int location = 0; location < this->max_size; location++){
                if(this->contain[location] != nullptr){
                    container->get_contain()[location] = this->contain[location];
                }
            }
            this->clear();
        }
    }

    void Object::clear(){
        for(int location = 0; location < this->max_size; location++){
            if(this->contain[location] != nullptr){
                this->contain[location].reset();
            }
        }
        this->contain.clear();
    }

    void Object::update(){
        for(int location = 0; location < this->max_size; location++){
            if(this->contain[location] != nullptr && (this->contain[location]->get_number() <= 0 || this->contain[location]->get_durability() <= 0)){
                this->remove_item_on(location);
            }
        }
    }

    void Object::save_to(std::string destination_path, std::string file_name, std::string items_files_path){
        std::string path = destination_path + "/" + file_name + ".inventory";

        std::fstream file(path, std::ios::out | std::ios::binary);
        if(file.is_open()){
            std::size_t item_number = 0;
            for(int i = 0; i < this->max_size; i++){
                if(this->contain[i] != nullptr)
                    item_number++;
            }
            file.write(reinterpret_cast<char*>(&item_number), sizeof(std::size_t));

            for(int i = 0; i < this->max_size; i++){
                if(this->contain[i] != nullptr){
                    std::size_t item_location = i;
                    file.write(reinterpret_cast<char*>(&item_location), sizeof(std::size_t));

                    std::size_t item_name_size = this->contain[i]->get_file_name().size();
                    file.write(reinterpret_cast<char*>(&item_name_size), sizeof(std::size_t));

                    std::string item_name = this->contain[i]->get_file_name();
                    file.write(&item_name[0], sizeof(char)*item_name_size);

                    std::size_t item_id_size = this->contain[i]->get_id().size();
                    file.write(reinterpret_cast<char*>(&item_id_size), sizeof(std::size_t));

                    std::string item_id = this->contain[i]->get_id();
                    item_id.resize(item_id_size);
                    file.write(&item_id[0], sizeof(char)*item_id_size);

                    this->contain[i]->save_to(items_files_path);
                }
            }
            file.close();
        }
    }

    void Object::load_from(std::string container_file_path, std::string items_files_path){
        std::fstream file(container_file_path, std::ios::in | std::ios::binary);
        if(file.is_open()){
            std::size_t item_number = 0;
            file.read(reinterpret_cast<char*>(&item_number), sizeof(std::size_t));

            for(int i = 0; i <= item_number; i++){
                std::size_t item_location = 0;
                file.read(reinterpret_cast<char*>(&item_location), sizeof(std::size_t));

                std::size_t item_name_size = 0;
                file.read(reinterpret_cast<char*>(&item_name_size), sizeof(std::size_t));

                std::string item_name;
                item_name.resize(item_name_size);
                file.read(&item_name[0], sizeof(char)*item_name_size);

                std::size_t item_id_size = 0;
                file.read(reinterpret_cast<char*>(&item_id_size), sizeof(std::size_t));

                std::string item_id;
                item_id.resize(item_id_size);
                file.read(&item_id[0], sizeof(char)*item_id_size);

                std::string item_path = items_files_path + "/" + item_id + ".item";

                if(item_name == Item::Names::knife.at(Main::english)){
                    Knife* new_knife = new Knife();
                    new_knife->load_from(item_path);
                    new_knife->add_to(this, item_location);
                }
                if(item_name == Cartridge::Names::cartridge_pistolet.at(Main::english)){
                    Cartridge::Object* new_cartridge = Cartridge::create_new(Cartridge::pistolet);
                    new_cartridge->load_from(item_path);
                    new_cartridge->add_to_container(this, item_location);
                }
                if(item_name == Cartridge::Names::cartridge_AK47.at(Main::english)){
                    Cartridge::Object* new_cartridge = Cartridge::create_new(Cartridge::AK_47);
                    new_cartridge->load_from(item_path);
                    new_cartridge->add_to_container(this, item_location);
                }
                if(item_name == Gun::names.at(Gun::pistolet).at(Main::english)){
                    Gun::Object* new_gun = new Gun::Object(Gun::pistolet);
                    new_gun->load_from(item_path);
                    new_gun->add_to_container(this, item_location);
                }
                if(item_name == Gun::names.at(Gun::AK_47).at(Main::english)){
                    Gun::Object* new_gun = new Gun::Object(Gun::AK_47);
                    new_gun->load_from(item_path);
                    new_gun->add_to_container(this, item_location);
                }
                if(item_name == Syringe::Names.at(Syringe::Type::zombie).at(Main::english)){
                    Syringe::Object* zombie_syringe = Syringe::create_new(Syringe::Type::zombie, Syringe::Type::clean);
                    zombie_syringe->load_from(item_path);
                    zombie_syringe->add_to_container(this, item_location);
                }
                if(item_name == Artificial_food::Names.at(Artificial_food::Foods_names::damaged_food_bag).at(Main::english)){
                    Artificial_food::Object* food = Artificial_food::create_new(Artificial_food::Foods_names::damaged_food_bag);
                    food->load_from(item_path);
                    food->add_to_container(this, item_location);
                }
            }
            file.close();
        }
    }

};
