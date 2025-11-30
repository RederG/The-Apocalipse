#include "item.hpp"
#include "Weapons/all_weapons.hpp"
#include "../Others/bullet.hpp"

namespace Item{

    Object::Object(){
        item_name = Item::Names::simple;
        id = to_string(Main::random_value() % 1000) + "_item_" + to_string(Main::random_value() % 1000);
        attack = 0;
        direction = right;
        type = Type::simple;
        sf::IntRect sprite_rect({0,0}, {32,32});
        item_texture = {
            {left, sf::IntRect({0,0}, {32,32})},
            {right, sf::IntRect({0,64}, {32,32})},
            {up, sf::IntRect({0,96}, {32,32})},
            {down, sf::IntRect({0,32}, {32,32})},
        };
        number = 1;
        max_number = 1;
    }

    Object::Object(Object *item){
        if(item != nullptr){
            item_name = item->item_name;
            id = to_string(Main::random_value() % 1000) + "_item_" + to_string(Main::random_value() % 1000);
            attack = item->attack;
            direction = right;
            type = item->get_type();
            texture_name = item->texture_name;
        }
    }

    void Object::draw_to(sf::RenderWindow &window, Entity::Object &entity){
        if (this != nullptr){
            sf::Sprite sprite_to_draw(*AllTextures::get(this->texture_name));
            sf::IntRect texture = this->get_texture_on(Item::Direction(entity.get_direction()));
            sf::IntRect sprite_rect(texture);

            sprite_to_draw.setTextureRect(sprite_rect);
            sprite_to_draw.setScale(this->get_scale());

            this->direction = Direction(entity.get_direction());
            this->set_position(entity, sprite_to_draw);
            window.draw(sprite_to_draw);
        }
    }

    sf::Sprite Object::draw_to(Entity::Object* entity, Map::Object* map, bool draw){
        sf::Sprite entity_sprite = entity->get_sprite(map);

        sf::Vector2f entity_position = entity_sprite.getPosition();
        sf::Vector2f entity_origin = entity_sprite.getOrigin();
        sf::Vector2f entity_scale = entity_sprite.getScale();

        sf::Vector2f entity_default_position;
        entity_default_position.x = entity_position.x - entity_origin.x*entity_scale.x;
        entity_default_position.y = entity_position.y - entity_origin.y*entity_scale.y;

        sf::Vector2f item_scale = map->get_scale();
        item_scale.x *= Item::Scales.at(this->type).x;
        item_scale.y *= Item::Scales.at(this->type).y;

        sf::Vector2f item_position = entity_default_position;
        item_position.x += entity->get_hand_location(map).x;
        item_position.y += entity->get_hand_location(map).y;

        Item::Direction item_direction = Item::Direction(int(entity->get_direction()));

        sf::Sprite item_sprite(*AllTextures::get(this->texture_name));
        item_sprite.setTextureRect(this->get_texture_on(item_direction));
        item_sprite.setScale(item_scale);
        item_sprite.setOrigin(this->HandleZone.at(item_direction));
        item_sprite.setPosition(item_position);

        if(map->get_window() != nullptr && draw == true)
            map->get_window()->draw(item_sprite);
        return item_sprite;
    }

    void Object::add_to(Container::Object* container, int location){
        if(location >= 0 && location < container->get_max_size() && container->get_item_at(location) == nullptr){
            container->get_contain()[location] = std::make_shared<Object>(*this);
        }
    }

    bool Object::add_to_container(Container::Object* container, bool aleatory){
        bool item_added = false;
        if(aleatory == true){
            bool added = false;
            if(container->is_full() == false){
                while(added == false){
                    int location = Main::random_value() % container->get_max_size();
                    if(container->get_item_at(location) == nullptr){
                        this->add_to(container, location);
                        added = true;
                        item_added = true;
                    }
                }
            }
        }
        else{
            for(int location = 0; location < container->get_max_size(); location++){
                if(container->get_contain()[location] == nullptr){
                    this->add_to(container, location);
                    item_added = true;
                    break;
                }
            }
        }
        return item_added;
    }

    sf::Vector2f Object::set_position(Entity::Object &entity, sf::Sprite &sprite_to_draw){
        sf::Vector2f item_pos;
        if (entity.get_direction() == Entity::Direction::up){
            item_pos.x += 40;
            item_pos.y += 5;
        }
        if (entity.get_direction() == Entity::Direction::down){
            item_pos.y += 10;
        }
        if (entity.get_direction() == Entity::Direction::right){
            item_pos.x += 45;
            item_pos.y += 10;
        }
        if (entity.get_direction() == Entity::Direction::left){
            item_pos.y += 10;
            item_pos.x -= 15;
        }
        sprite_to_draw.setPosition(item_pos);
        return item_pos;
    }

    sf::Vector2f Object::get_scale(){
        return Scales.at(this->type);
    }

    void Object::set_number_to(int new_number){
        this->number = new_number;
    }

    string Object::get_id(){
        return id;
    }

    sf::String Object::get_name(){
        return this->item_name[Main::lang()];
    }

    sf::IntRect Object::get_texture_on(Direction direction){
        return item_texture[direction];
    }

    std::string Object::get_texture_name(){
        return this->texture_name;
    }

    Type Object::get_type(){
        return this->type;
    }

    Direction Object::get_direction(){
        return this->direction;
    }

    unsigned int Object::get_number(bool special){
        int result;
        if(special == false){
            result = this->number;
        }
        if(special == true){
            result = 0;
        }
        return result;
    }

    unsigned int Object::get_max_number(bool special){
        int result;
        if(special == false){
            result = this->max_number;
        }
        if(special == true){
            result = 0;
        }
        return result;
    }

    sf::Clock* Object::get_using_timelapse_clock(){
        return &this->using_timelapse;
    }

    void Object::use_item(Entity::Object &entity, bool speacial_use){
        Debugging::write("Item default usage", Debugging::get_state_of(Debugging::In_game));
    }

    std::string Object::get_dependance(){
        return this->item_dependance.at(Main::lang());
    }

    std::string Object::get_file_name(){
        return this->item_name.at(Main::english);
    }

    void Object::add(unsigned number_plus){
        this->number += number_plus;
        if(this->number > this->max_number){
            this->number = this->max_number;
        }
        if(this->number < 0){
            this->number = 0;
        }
    }

    void Object::reduce(unsigned number_minus){
        this->number -= number_minus;
        if(this->number < 0){
            this->number = 0;
            this->durability = 0;
        }
    }

    bool Object::is_stackable(){
        if(this->number < this->max_number)
            return true;
        else
            return false;
    }

    void Object::draw_attack_rect(sf::RenderWindow &window){
        Debugging::write(std::to_string(window.getSize().x), Debugging::get_state_of(Debugging::Nothing));
    }

    bool Object::operator==(Object& item){
        if(this->item_name == item.item_name){
            return true;
        }
        else{
            return false;
        }
    }

    std::string Object::get_extra_info(){
        return Extra_info::simple.at(Main::lang());
    }

    Usability Object::get_usability(){
        return this->usability;
    }

    void Object::reduce_durability_to(int number){
        this->durability -= number;
    }

    bool Object::is_usable(){
        if(this->durability <= 0){
            return false;
        }
        else{
            return true;
        }
    }

    unsigned int Object::get_durability(){
        return this->durability;
    }

    unsigned int Object::get_max_durability(){
        return this->max_durability;
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

            file.close();
        }
    }

    void init(){
        Debugging::write("Intialising all items", Debugging::get_state_of(Debugging::In_game));
        Gun::clear_list();
        Bullet::clear_bullet_list();
        Cartridge::clear();
        Debugging::write("Intialising all items terminated", Debugging::get_state_of(Debugging::In_game));
    }

    void terminates(){
        Debugging::write("Destroying all items", Debugging::get_state_of(Debugging::In_game));
        Gun::clear_list();
        Bullet::clear_bullet_list();
        Cartridge::clear();
        Syringe::clear();
        Debugging::write("Destroying all items terminated", Debugging::get_state_of(Debugging::In_game));
    }

}