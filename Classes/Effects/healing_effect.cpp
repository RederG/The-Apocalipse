#include "healing_effect.hpp"

Healing_effect::Healing_effect(int seconds, int healing_value, int healing_time_lapse){
    this->id = std::to_string(Main::random_value() % 1000) + "_effect_heal_" + std::to_string(Main::random_value()%1000); 
    this->effect_type = Effect::Type::healing;
    this->max_life = seconds;
    this->healing_value = healing_value;
    if(this->healing_value <= 0)
        this->healing_value = 1;
}

void Healing_effect::add_to(Effect_manager* effect_manager){
    bool healing_effect_exists = false;
    for(int i = 0; i < effect_manager->get_all_effects().size(); i++){
        Effect::Object* effect = effect_manager->get_all_effects()[i];
        if(effect != nullptr && effect->get_effect_type() == Effect::Type::healing){
            healing_effect_exists = true;
            effect->set_max_life(effect->get_max_life() + this->max_life);
        }
    }
    if(!healing_effect_exists)
        effect_manager->get_all_effects().push_back(&*this);
}

void Healing_effect::effect(Entity::Object* entity){
    if(this->is_usable() && this->affecting_elapse_time > 5){
        this->affecting_elapse_time = 0;
        entity->get_healed(this->healing_value);
    }
}

void Healing_effect::update(){
    if(this->elapse_time_clock.getElapsedTime().asSeconds() >= 1){
        this->affecting_elapse_time++;
        this->life++;
        this->elapse_time_clock.restart();
    }
}

void Healing_effect::save_effect_to(std::string destination){
    std::fstream file(destination + "/" + this->id + ".effect", std::ios::app | std::ios::binary);
    if(file.is_open()){
        file.write(reinterpret_cast<char*>(&this->healing_value), sizeof(int));
        file.close();
    }
}

void Healing_effect::load_effect_from(std::string effect_file_path){
    std::fstream file(effect_file_path, std::ios::in | std::ios::binary);
    if(file.is_open()){
        std::size_t id_size = 0;
        file.read(reinterpret_cast<char*>(&id_size), sizeof(std::size_t));

        file.seekg(sizeof(char)*id_size + sizeof(int)*3, std::ios::cur);
        file.read(reinterpret_cast<char*>(&this->healing_value), sizeof(int));
        
        file.close();
    }
}