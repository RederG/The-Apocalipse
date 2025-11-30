#ifndef __THREAD_MANAGER__
    #define __THREAD_MANAGER__

    #include "../main.hpp"
    #include <thread>
    #include <vector>

    namespace ThreadManager{
        enum Thread{
            zombie_pathfinding, game_thread, structure_search
        };

        enum State{
            Paused, Actived, Terminated
        };

        void init();

        void terminates_all();

        void kill_all();

        void pause_all();

        void active_all();

        State get_state_of(Thread thread_name);

        void set_state_of(Thread thread_name, State to_state);
    };

#endif