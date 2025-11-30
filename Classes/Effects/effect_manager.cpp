#include "effect_manager.hpp"

Effect_manager::Effect_manager(){
    this->all_effects.clear();
}

void Effect_manager::update_on(Entity::Object* entity){
    if(entity != nullptr){
        for(auto effect : this->all_effects){
            effect->effect(entity);
        }
    }
}

void Effect_manager::update(){
    for(auto effect : this->all_effects){
        effect->update();
    }
    for(int i = 0; i < this->all_effects.size(); i++){
        if(this->all_effects[i] != nullptr && this->all_effects[i]->is_usable() == false){
            this->all_effects.erase(this->all_effects.begin() + i);
            i = 0;
        }
    }
}

void Effect_manager::clear(){
    for(auto effect : this->all_effects){
        delete effect;
        effect = nullptr;
    }
    this->all_effects.clear();
}

std::vector<Effect::Object*>& Effect_manager::get_all_effects(){
    return this->all_effects;
}

void Effect_manager::save_to(std::string destination_path, std::string file_name, std::string effects_files_path){
    std::string path = destination_path + "/" + file_name + ".effman";

    std::fstream file(path, std::ios::out | std::ios::binary);
    if(file.is_open()){
        std::size_t effect_number = this->all_effects.size();
        file.write(reinterpret_cast<char*>(&effect_number), sizeof(std::size_t));

        for(int i = 0; i < this->all_effects.size(); i++){
            this->all_effects[i]->save_to(effects_files_path);

            Effect::Type effect_type = this->all_effects[i]->get_effect_type();
            file.write(reinterpret_cast<char*>(&effect_type), sizeof(Effect::Type));

            std::size_t effect_id_size = this->all_effects[i]->get_id().size();
            file.write(reinterpret_cast<char*>(&effect_id_size), sizeof(std::size_t));
            file.write(this->all_effects[i]->get_id().c_str(), sizeof(char)*effect_id_size);
        }
        file.close();
    }
}

void Effect_manager::load_from(std::string effect_manager_file_path, std::string effects_files_path){
    std::fstream file(effect_manager_file_path, std::ios::in | std::ios::binary);
    if(file.is_open()){
        std::size_t effect_number = 0;
        file.read(reinterpret_cast<char*>(&effect_number), sizeof(std::size_t));

        for(int i = 0; i < effect_number; i++){
            Effect::Type effect_type = Effect::Type::nothing;
            file.read(reinterpret_cast<char*>(&effect_type), sizeof(Effect::Type));

            std::size_t effect_name_length = 0;
            file.read(reinterpret_cast<char*>(&effect_name_length), sizeof(std::size_t));
            std::string effect_name;
            effect_name.resize(effect_name_length);
            file.read(&effect_name[0], sizeof(char)*effect_name_length);

            if(effect_type == Effect::Type::healing){
                Healing_effect* healing_effect = new Healing_effect(0, 0, 0);
                healing_effect->load_from(effects_files_path + "/" + effect_name + ".effect");
                healing_effect->add_to(this);
            }
            if(effect_type == Effect::Type::hungry){
                Hungry_effect* hungry_effect = new Hungry_effect(0, 0, 0);
                hungry_effect->load_from(effects_files_path + "/" + effect_name + ".effect");
                hungry_effect->add_to(this);
            }
        }
        file.close();
    }
}