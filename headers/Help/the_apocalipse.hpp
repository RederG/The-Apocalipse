#ifndef __THE_APOCALIPSE__
    #define __THE_APOCALIPSE__

    #include "texture_manager.hpp"
    #include "thread_manager.hpp"
    #include "../Map/map_manager.hpp"
    #include "../Map/Structures/structure_manager.hpp"
    #include "../Others/perlin_noise.hpp"

    #include "../Entities/entity.hpp"
    #include "../Entities/player.hpp"
    #include "../Entities/zombie.hpp"

    #include "../InteractiveObjects/all_interactive_objects.hpp"

    #include "../Items/all_items.hpp"
    #include "../Others/bullet.hpp"

    #include "../Graphicals/life_bar.hpp"
    #include "../Graphicals/KeysManager.hpp"
    #include "../Graphicals/viewer.hpp"

    #include "../Others/probability.hpp"

    namespace TheApocalipse{
        // Inits all components
        void init();

        // Ends all components
        void terminates();
        
        // Verifies if the Apocalipse's elements are running
        bool is_running();
    }

#endif