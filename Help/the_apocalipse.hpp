#ifndef __THE_APOCALIPSE__
    #define __THE_APOCALIPSE__

    #include "texture_manager.hpp"
    #include "thread_manager.hpp"
    #include "../Classes/Map/map_manager.hpp"
    #include "../Classes/Map/Structures/structure_manager.hpp"
    #include "../Classes/Others/perlin_noise.hpp"

    #include "../Classes/Entities/entity.hpp"
    #include "../Classes/Entities/player.hpp"
    #include "../Classes/Entities/zombie.hpp"

    #include "../Classes/InteractiveObjects/all_interactive_objects.hpp"

    #include "../Classes/Items/all_items.hpp"
    #include "../Classes/Others/bullet.hpp"

    #include "../Classes/Graphicals/life_bar.hpp"
    #include "../Classes/Graphicals/KeysManager.hpp"
    #include "../Classes/Graphicals/viewer.hpp"

    #include "../Classes/Others/probability.hpp"

    namespace TheApocalipse{
        // Inits all components
        void init();

        // Ends all components
        void terminates();
        
        // Verifies if the Apocalipse's elements are running
        bool is_running();
    }

#endif