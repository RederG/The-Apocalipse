#include "effect.hpp"

namespace Effect{
    Object::Object(){
        this->id = std::to_string(Main::random_value() % 1000) + "_effect_default_" + std::to_string(Main::random_value()%1000); 
    }

    void Object::effect(Entity::Object* entity){}

    bool Object::is_usable(){
        bool usable = true;
        if(this->life > this->max_life)
            usable = false;
        return usable;
    }

    std::string Object::get_id(){
        return this->id;
    }

    Type Object::get_effect_type(){
        return this->effect_type;
    }

    int Object::get_max_life(){
        return this->max_life;
    }

    int Object::get_life(){
        return this->life;
    }

    sf::IntRect Object::get_texture_location(){
        return textures_location.at(this->effect_type);
    }

    std::string Object::get_texture_name(){
        return this->texture_name;
    }

    void Object::set_max_life(int new_activity_time){
        this->max_life = new_activity_time;
    }

    void Object::update(){
        if(this->elapse_time_clock.getElapsedTime().asSeconds() >= 1)
            this->life++;
    }

    void Object::add_to(Effect_manager* effect_manager){}

    void Object::save_to(std::string destination_path){
        std::fstream file(destination_path + "/" + this->id + ".effect", std::ios::out | std::ios::binary);
        if(file.is_open()){
            std::size_t id_size = this->id.size();
            file.write(reinterpret_cast<char*>(&id_size), sizeof(std::size_t));

            file.write(this->id.c_str(), sizeof(char)*id_size);
            file.write(reinterpret_cast<char*>(&this->life), sizeof(int));
            file.write(reinterpret_cast<char*>(&this->max_life), sizeof(int));
            file.write(reinterpret_cast<char*>(&this->affecting_elapse_time), sizeof(int));

            file.close();
        }
        this->save_effect_to(destination_path);
    }

    void Object::load_from(std::string effect_file_path){
        std::fstream file(effect_file_path, std::ios::in | std::ios::binary);
        if(file.is_open()){
            std::size_t id_size = 0;
            file.read(reinterpret_cast<char*>(&id_size), sizeof(std::size_t));

            this->id.resize(id_size);
            file.read(&this->id[0], sizeof(char)*id_size);
            file.read(reinterpret_cast<char*>(&this->life), sizeof(int));
            file.read(reinterpret_cast<char*>(&this->max_life), sizeof(int));
            file.read(reinterpret_cast<char*>(&this->affecting_elapse_time), sizeof(int));

            file.close();
        }
        this->load_effect_from(effect_file_path);
    }

    void Object::load_effect_from(std::string effect_file_path){}

    void Object::save_effect_to(std::string destination_path){}
}