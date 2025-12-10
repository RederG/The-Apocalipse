#include "../../../headers/Map/Structures/structure.hpp"
#include "../../../headers/InteractiveObjects/wall.hpp"
#include "../../../headers/InteractiveObjects/roof.hpp"

namespace Structure{

    void fill_image(sf::Image& img_to_fill, sf::Vector2u init_pos, sf::Vector2u end_pos, sf::Texture texture_to_use, sf::IntRect texture_rect){
        sf::Image image_to_use = texture_to_use.copyToImage();
        for(unsigned int y_img = init_pos.y, y = texture_rect.position.y; y_img <= end_pos.y, y < texture_rect.position.y + texture_rect.size.y; y_img++, y++){
            for(unsigned int x_img = init_pos.x, x = texture_rect.position.x; x_img <= end_pos.x, x < texture_rect.position.x + texture_rect.size.x; x_img++, x++){
                sf::Color img_to_use_pixel = image_to_use.getPixel({x, y});
                img_to_fill.setPixel({x_img, y_img}, img_to_use_pixel);
            }
        }
    }

    sf::Image create_image_of(Instance structure){
        sf::Image struct_img;
        struct_img.resize({18, 18}, sf::Color::Transparent);
        
        sf::Texture* entries_texture = AllTextures::get("struct_entries");
        if(entries_texture != nullptr){
            if(structure.sides.up.position != EntryPosition::full){
                sf::IntRect up_rect = entry_texture.at("up_down").at(structure.sides.up.position);
                if(structure.sides.up.big_entry && structure.sides.up.position != EntryPosition::none)
                    up_rect.position.y += 3;
                fill_image(struct_img, {0, 0}, {17, 0}, *entries_texture, up_rect);
            }
            if(structure.sides.down.position != EntryPosition::full){
                sf::IntRect down_rect = entry_texture.at("up_down").at(structure.sides.down.position);
                if(structure.sides.down.big_entry && structure.sides.down.position != EntryPosition::none)
                    down_rect.position.y += 3;
                fill_image(struct_img, {0, 17}, {17, 17}, *entries_texture, down_rect);
            }
            if(structure.sides.left.position != EntryPosition::full){
                sf::IntRect left_rect = entry_texture.at("left_right").at(structure.sides.left.position);
                if(structure.sides.left.big_entry && structure.sides.left.position != EntryPosition::none)
                    left_rect.position.x += 3;
                fill_image(struct_img, {0, 0}, {0, 17}, *entries_texture, left_rect);
            }
            if(structure.sides.right.position != EntryPosition::full){
                sf::IntRect right_rect = entry_texture.at("left_right").at(structure.sides.right.position);
                if(structure.sides.right.big_entry && structure.sides.right.position != EntryPosition::none)
                    right_rect.position.x += 3;
                fill_image(struct_img, {17, 0}, {17, 17}, *entries_texture, right_rect);
            }
        }
        return struct_img;
    }

    void set_walls_of(Instance& structure){
        if(!structure.contains_walls){
            if(AllTextures::get_structure(Main::get_pair2i_of(structure.position)) == nullptr)
                AllTextures::set_structure(Main::get_pair2i_of(structure.position), create_image_of(structure));
            sf::Image img = AllTextures::get_structure(Main::get_pair2i_of(structure.position))->copyToImage();
            structure.contain.walls.clear();
            unsigned int y = 0;
            for(unsigned int x = 0; x < 18; x++){
                sf::Vector2i wall_position = {
                    int(structure.position.x*18 + x), 
                    int(structure.position.y*18 + y)
                };
                Wall::Type wall_type = Wall::get_from(img.getPixel({x, y}));
                if(wall_type == Wall::Type::with_window && StructureManager::is_structure({structure.position.x, structure.position.y - 1}))
                    wall_type = Wall::Type::simple;
                if((wall_type == Wall::Type::with_door && structure.contains_doors) || (wall_type == Wall::Type::simple) || (wall_type == Wall::Type::down_simple) || (wall_type == Wall::Type::with_window))
                    structure.contain.walls.push_back(new Wall::Object(wall_type, wall_position));
            }
            for(unsigned int y2 = 1; y2 < 17; y2++){
                unsigned int x = 0;
                sf::Vector2i wall_position = {
                    int(structure.position.x*18 + x), 
                    int(structure.position.y*18 + y2)
                };
                Wall::Type wall_type = Wall::get_from(img.getPixel({x, y2}));
                if(wall_type == Wall::Type::down_with_window && StructureManager::is_structure({structure.position.x - 1, structure.position.y}))
                    wall_type = Wall::Type::down_simple;
                if((wall_type == Wall::Type::down_with_door && structure.contains_doors) || (wall_type == Wall::Type::down_simple) || (wall_type == Wall::Type::down_with_window))
                    structure.contain.walls.push_back(new Wall::Object(wall_type, wall_position));
                    
                x = 17;
                wall_position = {
                    int(structure.position.x*18 + x), 
                    int(structure.position.y*18 + y2)
                };
                wall_type = Wall::get_from(img.getPixel({x, y2}));
                if(wall_type == Wall::Type::down_with_window && StructureManager::is_structure({structure.position.x + 1, structure.position.y}))
                    wall_type = Wall::Type::down_simple;
                if((wall_type == Wall::Type::down_with_door && structure.contains_doors) || (wall_type == Wall::Type::down_simple) || (wall_type == Wall::Type::down_with_window))
                    structure.contain.walls.push_back(new Wall::Object(wall_type, wall_position));
            }
            y = 17;
            for(unsigned int x = 0; x < 18; x++){
                sf::Vector2i wall_position = {
                    int(structure.position.x*18 + x), 
                    int(structure.position.y*18 + y)
                };
                Wall::Type wall_type = Wall::get_from(img.getPixel({x, y}));
                if(wall_type == Wall::Type::with_window && StructureManager::is_structure({structure.position.x, structure.position.y + 1}))
                    wall_type = Wall::Type::simple;
                if((wall_type == Wall::Type::with_door && structure.contains_doors) || (wall_type == Wall::Type::simple) || (wall_type == Wall::Type::down_simple) || (wall_type == Wall::Type::with_window))
                    structure.contain.walls.push_back(new Wall::Object(wall_type, wall_position));
            }
            
            structure.contains_walls = true;
        }
    }

    void set_roof_of(Instance& structure){
        structure.contain.roof = nullptr;
        sf::Vector2i position = structure.position;
        position.x *= 18;
        position.y *= 18;
        structure.contain.roof = new Roof::Object(Roof::Type::default_roof, position);
    }

}