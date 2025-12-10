
#ifndef COLLISION_H
    #define COLLISION_H

    #include "../main.hpp"

    namespace Collision {
        bool verify(sf::Sprite sprite1, sf::Sprite sprite2, char correct_color_alpha = 0);
    }

#endif