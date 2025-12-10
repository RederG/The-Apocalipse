#include "../../headers/Help/the_apocalipse.hpp"

namespace TheApocalipse{
    bool running = false;

    void init(){
        Debugging::write("INITIALISING THE GAME............", Debugging::get_state_of(Debugging::Description));
        ThreadManager::active_all();
        ThreadManager::set_state_of(ThreadManager::Thread::game_thread, ThreadManager::State::Paused);
        running = true;

        AllTextures::init();

        Item::init();

        Probability::init();
        Map::clear();
        PerlinNoise::init();
        Entity::init();
    }

    void terminates(){
        Debugging::write("TERMINATING THE GAME............", Debugging::get_state_of(Debugging::Description));
        running = false;
        ThreadManager::pause_all();
        Entity::terminates();
        for(auto& tomb : Tomb::container)
            delete tomb;
        Tomb::container.clear();
        AllTextures::destroy();
        Probability::terminate();
        Map::clear();
        PerlinNoise::clear();
        StructureManager::reset();
    }

    bool is_running(){
        return running;
    }

};