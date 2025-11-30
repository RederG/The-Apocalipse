#include "thread_manager.hpp"
#include "../Classes/Entities/zombie.hpp"
#include "../Classes/Map/Structures/structure_manager.hpp"
#include "../window_menu/game_window.hpp"

namespace ThreadManager{
    std::thread* zombie_thread = nullptr;
    std::thread* structure_thread = nullptr;
    std::thread* game_updater_thread = nullptr;
    
    std::map<Thread, State> all_threads;

    void init(){
        Debugging::write("(Thread manager) - Intitializing all threads");
        
        zombie_thread = new std::thread(Zombie::everyone_search_player);
        all_threads[Thread::zombie_pathfinding] = State::Paused;

        structure_thread = new std::thread(StructureManager::search_structures);
        all_threads[Thread::structure_search] = State::Paused;

        game_updater_thread = new std::thread(GameWindow::UpdateOnThread);
        all_threads[Thread::game_thread] = State::Paused;

        if(zombie_thread->joinable())
            zombie_thread->detach();
        if(structure_thread->joinable())
            structure_thread->detach();
        if(game_updater_thread->joinable())
            game_updater_thread->detach();
            
        Debugging::write("(Thread manager) - Initializing all threads terminated");
    }

    void terminates_all(){
        Debugging::write("(Thread manager) - Terminating all threads");
        for(auto& thread : all_threads)
            thread.second = State::Terminated;
        
        bool zombie_thread_terminated = false, structure_thread_terminated = false;
        while(!zombie_thread_terminated || !structure_thread_terminated){
            if(!StructureManager::is_actived() && !structure_thread_terminated){
                structure_thread_terminated = true;
                Debugging::write("(Thread manager) - structure thread terminated");
            }
            if(!Zombie::are_searching() && !zombie_thread_terminated){
                zombie_thread_terminated = true;
                Debugging::write("(Thread manager) - zombie thread terminated");
            }
        }
        Debugging::write("(Thread manager) - Terminating all threads complete");
    }

    void kill_all(){
        Debugging::write("(Thread manager) - Killing all threads");
        structure_thread = nullptr;
        zombie_thread = nullptr;
        game_updater_thread = nullptr;
        Debugging::write("(Thread manager) - Killing all threads complete");
    }

    void pause_all(){
        for(auto& thread : all_threads)
            thread.second = State::Paused;
        Debugging::write("(Thread manager) - All threads paused");
    }

    void active_all(){
        for(auto& thread : all_threads)
            thread.second = State::Actived;
        Debugging::write("(Thread manager) - All threads actived");
    }

    State get_state_of(Thread thread_name){
        return all_threads[thread_name];
    }

    void set_state_of(Thread thread_name, State new_state){
        all_threads[thread_name] = new_state;
    }
};