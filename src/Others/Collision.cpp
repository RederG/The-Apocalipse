#include "../../headers/Others/Collision.hpp"

namespace Collision{
    bool verify(sf::Sprite sprite1, sf::Sprite sprite2, char correct_color_alpha){
        sf::Image image1 = sprite1.getTexture().copyToImage();
        sf::Image image2 = sprite2.getTexture().copyToImage();
        sf::Vector2f position1 = sprite1.getPosition();
        sf::Vector2f position2 = sprite2.getPosition();
        position1.x -= sprite1.getScale().x*sprite1.getOrigin().x;
        position1.y -= sprite1.getScale().y*sprite1.getOrigin().y;

        position2.x -= sprite2.getScale().x*sprite2.getOrigin().x;
        position2.y -= sprite2.getScale().y*sprite2.getOrigin().y;

        bool collision = false;
        std::optional intersection = sprite1.getGlobalBounds().findIntersection(sprite2.getGlobalBounds());
        if(intersection.has_value()){
            for(int y = intersection->position.y; y <= intersection->position.y + intersection->size.y; y++){
                for(int x = intersection->position.x; x <= intersection->position.x + intersection->size.x; x++){
                    sf::Vector2u pixel1 = {(unsigned int)(x - int(position1.x)), (unsigned int)(y - int(position1.y))};
                    sf::Vector2u pixel2 = {(unsigned int)(x - int(position2.x)), (unsigned int)(y - int(position2.y))};

                    sf::Color img1_color = image1.getPixel(pixel1);
                    sf::Color img2_color = image2.getPixel(pixel2);
                    if(img1_color.a > correct_color_alpha && img2_color.a > correct_color_alpha){
                        collision = true;
                        break;
                    }
                }
                if(collision)
                    break;
            }
        }
        return collision;
    }
}