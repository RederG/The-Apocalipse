#include "syringe.hpp"

std::vector<std::shared_ptr<Syringe::Object>> all_syringes;

namespace Syringe{
    Object::Object(Type syringe_type, Type old_syringe_type){
        this->item_name = Names.at(syringe_type);
        this->id = std::to_string(Main::random_value() % 1000) + "_effect_item_syringe_" + std::to_string(Main::random_value() % 1000);
        this->attack = 0;
        this->direction = Item::right;
        this->type = Item::Type::effect;
        this->item_texture = textures_location.at(syringe_type);
        this->HandleZone = Handling_location;
        this->max_number = 3;
        this->texture_name = "syringes";
        this->syringe_type = syringe_type;
        this->old_syringe_type = old_syringe_type;
        this->durability = 1;
        this->max_durability = 1;
        this->set_number();
    }

    Object::Object(Type syringe_type, Type old_syringe_type, int number){
        this->item_name = Names.at(syringe_type);
        this->id = std::to_string(Main::random_value() % 1000) + "_effect_item_syringe_" + std::to_string(Main::random_value() % 1000);
        this->attack = 0;
        this->direction = Item::right;
        this->type = Item::Type::effect;
        this->item_texture = textures_location.at(syringe_type);
        this->HandleZone = Handling_location;
        this->number = number;
        this->max_number = 3;
        if(this->number > this->max_number)
            this->number = this->max_number;
        if(this->number <= 0)
            this->number = 1;
        this->texture_name = "syringes";
        this->syringe_type = syringe_type;
        this->old_syringe_type = old_syringe_type;
        this->durability = 1;
        this->max_durability = 1;
    }

    void Object::create_effect_for(Entity::Object* entity){
        bool used_syringe_exists = false;
        std::shared_ptr<Object> used_syringe = nullptr;
        for(int i = 0; i < entity->get_inventory()->get_max_size(); i++){
            auto item = entity->get_inventory()->get_contain()[i];
            if(item != nullptr && item->get_number() < this->max_number){
                used_syringe = get_syringe(item->get_name(), item->get_id());
                if(used_syringe != nullptr && used_syringe->syringe_type == Type::used){
                    used_syringe_exists = true;
                    break;
                }
            }
        }
        if(used_syringe != nullptr || entity->get_inventory()->is_full() == false){
            switch(this->syringe_type){
                case Type::clean:
                    break;
                case Type::used:
                    break;
                case Type::zombie:{
                    Object* new_syringe = create_new(Type::used, Type::zombie, 1);
                    if(new_syringe->add_to_container(entity->get_inventory()) == true){
                        Healing_effect* healing_effect = new Healing_effect(20, 8, 5);
                        healing_effect->add_to(entity->get_effect_manager());

                        Hungry_effect* hungry_effect = new Hungry_effect(30, 5, 10);
                        hungry_effect->add_to(entity->get_effect_manager());

                        this->number--;
                    }
                    else{
                        for(int i = 0; i < all_syringes.size(); i++){
                            if(all_syringes[i]->get_name() == new_syringe->get_name() && all_syringes[i]->get_id() == new_syringe->get_id()){
                                all_syringes.erase(all_syringes.begin() + i);
                                break;
                            }
                        }
                    }
                    break;
                }
            }
        }
    }

    void Object::add_to(Container::Object* container, int location){
        if(location >= 0 && location < container->get_max_size() && container->get_item_at(location) == nullptr){
            container->get_contain()[location] = std::make_shared<Object>(*this);
        }
    }

    bool Object::add_to_container(Container::Object* container, bool aleatory){
        bool added = false;
        std::shared_ptr<Object> same_syringe = nullptr;
        for(int container_location = 0; container_location < container->get_max_size(); container_location++){
            if(container->get_item_at(container_location) != nullptr && typeid(*this) == typeid(*container->get_item_at(container_location))){
                auto item = container->get_contain()[container_location];
                same_syringe = get_syringe(item->get_name(), item->get_id());
                if(same_syringe != nullptr && same_syringe->syringe_type != this->syringe_type){
                    same_syringe = nullptr;
                }
                if(same_syringe != nullptr)
                    break;
            }
        }
        if(same_syringe == nullptr && container->is_full() == false){
            if(aleatory == false){
                for(int location = 0; location < container->get_max_size(); location++){
                    if(container->get_contain()[location] == nullptr){
                        container->get_contain()[location] = get_syringe(this->get_name(), this->get_id());
                        added = true;
                        break;
                    }
                }
            }
            else{
                while(added == false){
                    int location = Main::random_value() % container->get_max_size();
                    if(container->get_contain()[location] == nullptr){
                        added = true;
                        container->get_contain()[location] = get_syringe(this->get_name(), this->get_id());
                    }
                }
            }
        }
        else if(same_syringe != nullptr){
            same_syringe->number += this->number;
            this->number = 0;
            if(same_syringe->number > same_syringe->max_number){
                int extra_number = same_syringe->number - same_syringe->max_number;
                this->number += extra_number;
                same_syringe->number = same_syringe->max_number;
                for(int i = 0; i < container->get_max_size(); i++){
                    if(container->get_contain()[i] == nullptr){
                        container->get_contain()[i] = get_syringe(this->get_name(), this->get_id());
                        break;
                    }
                }
            }
            added = true;
        }

        return added;
    }

    std::string Object::get_extra_info(){
        return all_extra_infos.at(this->syringe_type).at(Main::lang());
    }

    std::string Object::get_file_name(){
        return Syringe::Names.at(this->syringe_type).at(Main::english);
    }

    void Object::set_number(){
        std::string number = Probability::get_universe("syringe_number")->get_result();
        if(number == "max"){
            this->number = this->max_number;
        }
        if(number == "half"){
            this->number = this->max_number/2;
        }
        if(number == "low" || number == "very_low"){
            this->number = 1;
        }
    }

    Object* create_new(Type syringe_type, Type old_syringe_type){
        std::shared_ptr<Object> new_syringe = std::make_shared<Object>(*new Object(syringe_type, old_syringe_type));
        all_syringes.push_back(new_syringe);
        return &*new_syringe;
    }

    Object* create_new(Type syringe_type, Type old_syringe_type, int number){
        std::shared_ptr<Object> new_syringe = std::make_shared<Object>(*new Object(syringe_type, old_syringe_type, number));
        all_syringes.push_back(new_syringe);
        return &*new_syringe;
    }

    std::shared_ptr<Object> get_syringe(std::string name, std::string id){
        std::shared_ptr<Object> syringe_to_get = nullptr;
        for(auto syringe_ptr : all_syringes){
            if(syringe_ptr->get_name() == name && syringe_ptr->get_id() == id){
                syringe_to_get = syringe_ptr;
                break;
            }
        }
        return syringe_to_get;
    }

    void clear(){
        all_syringes.clear();
    }
}