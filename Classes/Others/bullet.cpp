#include "bullet.hpp"
#include "../Entities/entity.hpp"
#include "../InteractiveObjects/tomb.hpp"
using namespace Weapon;

namespace Bullet{
    std::list<std::shared_ptr<Object>> all_bullets;
    
    Object::Object(Gun::Nature type_of_gun){
        this->gun_type = type_of_gun;
        this->attack = all_attacks.at(type_of_gun);
        this->gun_used.gun_name = Gun::get_temporary_gun()->get_name();
        this->gun_used.gun_id = Gun::get_temporary_gun()->get_id();

        // The entity is used to configure the position of the gun for the bullets
        if(Entity::get_temporary_entity() != nullptr){
            this->map_position = Entity::get_temporary_entity()->get_map_position();
            sf::Sprite gun_sprite = Gun::get_temporary_gun()->draw_to(Entity::get_temporary_entity(), Entity::get_map(), false);

            sf::Vector2f gun_default_position = gun_sprite.getPosition();
            gun_default_position.x -= gun_sprite.getOrigin().x*gun_sprite.getScale().x;
            gun_default_position.y -= gun_sprite.getOrigin().y*gun_sprite.getScale().y;

            Item::Direction entity_direction = Item::Direction(int(Entity::get_temporary_entity()->get_direction()));
            sf::Vector2f map_scale = Entity::get_map()->get_scale();

            sf::Vector2f canon_position = gun_default_position;
            canon_position.x += Gun::CanonZone.at(this->gun_type).at(entity_direction).x*gun_sprite.getScale().x;
            canon_position.y += Gun::CanonZone.at(this->gun_type).at(entity_direction).y*gun_sprite.getScale().y;

            this->map_position = Entity::get_map()->get_map_position_of(canon_position);

            switch(Entity::get_temporary_entity()->get_direction()){
                case Entity::Direction::left:
                    velocity_x = -velocity.at(type_of_gun);
                    velocity_y = 0;
                    rotation = 180;
                    break;
                case Entity::Direction::right:
                    velocity_x = velocity.at(type_of_gun);
                    velocity_y = 0;
                    rotation = 0;
                    break;
                case Entity::Direction::down:
                    velocity_x = 0;
                    velocity_y = velocity.at(type_of_gun);
                    rotation = 90;
                    break;
                case Entity::Direction::up:
                    velocity_x = 0;
                    velocity_y = -velocity.at(type_of_gun);
                    rotation = 270;
                    break;
                default:
                    velocity_x = 0;
                    velocity_y = 0;
                    rotation = 0;
            }
        }
    }
    
    Object::Object(Object* bullet){
        this->gun_type = bullet->gun_type;
        this->gun_used = bullet->gun_used;
        this->attack = bullet->attack;
        this->position = bullet->position;
        this->velocity_x = bullet->velocity_x;
        this->velocity_y = bullet->velocity_y;
        this->rotation = bullet->rotation;
        this->map_position = bullet->map_position;
    }

    void Object::set_position(sf::Sprite &gun_sprite){
        // the half size of the gun
        float size_x = 0, size_y = 0;
        size_x = gun_sprite.getGlobalBounds().size.x/2;
        size_y = gun_sprite.getGlobalBounds().size.y/2;
        sf::Vector2f gun_pos = Gun::get_temporary_gun()->set_position(*Entity::get_temporary_entity(), gun_sprite);
        this->gun_used.gun_pos = gun_pos;
        gun_pos.x += size_x;
        gun_pos.y += size_y;
        this->position = gun_pos;
    }

    sf::Clock Object::get_living_clock(){
        return this->living_clock;
    }

    Gun::Nature Object::get_gun_nature(){
        return this->gun_type;
    }

    sf::Vector2f Object::get_position(){
        return this->position;
    }

    Gun_used Object::get_gun_used(){
        return this->gun_used;
    }

    sf::Sprite Object::get_sprite(Map::Object* map, sf::Color color){
        sf::Sprite bullet_sprite(*AllTextures::get("bullets"));

        sf::IntRect bullet_texture_location = Bullet::bullet_textures.at(this->get_gun_nature());
        bullet_sprite.setTextureRect(bullet_texture_location);

        sf::Vector2f bullet_origin;
        bullet_origin.x = bullet_texture_location.size.x/2;
        bullet_origin.y = bullet_texture_location.size.y/2;

        bullet_sprite.setOrigin(bullet_origin);

        bullet_sprite.setScale(map->get_scale());
        bullet_sprite.setRotation(sf::degrees(this->rotation));
        bullet_sprite.setPosition(this->get_window_position_on(map));
        bullet_sprite.setColor(color);
        return bullet_sprite;
    }

    sf::Vector2f Object::get_velocity(){
        sf::Vector2f velocity;
        velocity.x = velocity_x;
        velocity.y = velocity_y;

        return velocity;
    }

    int Object::get_attack(){
        return this->attack;
    }

    Object* new_bullet(Gun::Nature type_of_gun){
        auto bullet = std::make_shared<Object>(new Object(type_of_gun));
        all_bullets.push_back(bullet);
        return &*bullet;
    }

    void clear_bullet_list(){
        for(auto bullet : all_bullets)
            bullet.reset();
        all_bullets.clear();
    }

    void draw_all_bullets_to(Map::Object* map){
        if(all_bullets.empty() == false)
            for(auto bullet : all_bullets)
                bullet->draw_to(map);
    }

    void verify_bullets_lives(){
        for(auto bullet : all_bullets){
            auto clock = bullet->get_living_clock();
            if(Main::clock_seconds(10, clock) == Main::Reached){
                all_bullets.remove(bullet);
                break;
            }
        }
    }
    
    void move_all(int fps){
        if(all_bullets.empty() != true){
            for(auto bullet : all_bullets){
                bullet->move_on_map(bullet->get_velocity().x, bullet->get_velocity().y);
            }
        }
    }

    void verify_collision(){
        if(all_bullets.empty() != true && Entity::get_map() != nullptr){
            for(auto bullet : all_bullets){
                sf::Sprite bullet_sprite = bullet->get_sprite(Entity::get_map());

                Entity::Object* the_shooter = nullptr;
                bool shooter_found = false;
                // Configuration of the shooter
                for(auto entity : Entity::container){
                    for(auto item : entity->get_inventory()->get_contain()){
                        if(item.second != nullptr && item.second->get_name() == bullet->get_gun_used().gun_name && item.second->get_id() == bullet->get_gun_used().gun_id){
                            the_shooter = &*entity;
                            shooter_found = true;
                            break;
                        }
                    }
                    if(shooter_found == true)
                        break;
                }

                bool collision = false;
                // If the bullet hits an entity (not the shooter)
                for(auto entity : Entity::container){
                    if(entity->get_name() != the_shooter->get_name() || entity->get_id() != the_shooter->get_id()){
                        if(Collision::verify(entity->get_sprite(Entity::get_map()), bullet_sprite)){
                            collision = true;
                            all_bullets.remove(bullet);
                            entity->get_attacked(bullet->get_attack());
                            break;
                        }
                    }
                }

                // If the bullet hits a tomb
                for(auto tomb : Tomb::container){
                    if(Collision::verify(tomb->get_sprite(Entity::get_map()), bullet_sprite) == true){
                        all_bullets.remove(bullet);
                        collision = true;
                        break;
                    }
                }

                if(collision == true)
                    break;
            }
        }
    }

};