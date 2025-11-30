#include "hungry_effect.hpp"

Hungry_effect::Hungry_effect(int seconds, int satiation_value_to_reduce, int time_lapse){
    this->id = std::to_string(Main::random_value() % 1000) + "_effect_hungry_" + std::to_string(Main::random_value()%1000); 
    this->affecting_elapse_time = time_lapse;
    this->max_life = seconds;
    this->effect_type = Effect::hungry;
    this->hungry_value = satiation_value_to_reduce;
}

void Hungry_effect::add_to(Effect_manager* effect_manager){
    bool hungry_effect_exists = false;
    for(int i = 0; i < effect_manager->get_all_effects().size(); i++){
        Effect::Object* effect = effect_manager->get_all_effects()[i];
        if(effect != nullptr && effect->get_effect_type() == Effect::Type::hungry){
            hungry_effect_exists = true;
            effect->set_max_life(effect->get_max_life() + this->max_life);
        }
    }
    if(!hungry_effect_exists)
        effect_manager->get_all_effects().push_back(&*this);
}

void Hungry_effect::update(){
    if(this->elapse_time_clock.getElapsedTime().asSeconds() >= 1){
        this->affecting_elapse_time++;
        this->life++;
        this->elapse_time_clock.restart();
    }
}

void Hungry_effect::effect(Entity::Object* entity){
    if(this->affecting_elapse_time > 5){
        this->affecting_elapse_time = 0;
        entity->get_hungry(this->hungry_value);
    }
}

void Hungry_effect::save_effect_to(std::string destination){
    std::fstream file(destination + "/" + this->id + ".effect", std::ios::app | std::ios::binary);
    if(file.is_open()){
        file.write(reinterpret_cast<char*>(&this->hungry_value), sizeof(int));
        file.close();
    }
}

void Hungry_effect::load_effect_from(std::string effect_file_path){
    std::fstream file(effect_file_path, std::ios::in | std::ios::binary);
    if(file.is_open()){
        std::size_t id_size = 0;
        file.read(reinterpret_cast<char*>(&id_size), sizeof(std::size_t));

        file.seekg(sizeof(char)*id_size + sizeof(int)*3, std::ios::cur);
        file.read(reinterpret_cast<char*>(&this->hungry_value), sizeof(int));
        
        file.close();
    }
}