#include "../../headers/Others/probability.hpp"

namespace Probability{

    std::vector<Universe*> all_universes;

    void init(){
        Debugging::write("Intialising all probabilities", Debugging::get_state_of(Debugging::In_game));

        std::map<std::string, char> event_can_have_knife{
            {"yes", 1},
            {"no", 1}
        };
        std::map<std::string, char> event_can_have_gun{
            {"yes", 1},
            {"no", 4}
        };
        std::map<std::string, char> event_can_have_specific_gun{
            {"pistolet", 70},
            {"AK47", 30}
        };
        std::map<std::string, char> event_can_have_cartridge{
            {"yes", 5},
            {"no", 5}
        };
        std::map<std::string, char> event_can_have_specific_cartridge{
            {"pistolet", 70},
            {"AK47", 30}
        };
        std::map<std::string, char> event_durability{
            {"max", 5},
            {"half", 20},
            {"low", 30},
            {"very low", 45}
        };
        std::map<std::string, char> event_ammo{
            {"max", 5},
            {"half", 20},
            {"low", 30},
            {"very low", 45}
        };
        std::map<std::string, char> event_cartridge_number{
            {"max", 5},
            {"half", 20},
            {"low", 30},
            {"very low", 45}
        };
        std::map<std::string, char> event_syringe_number{
            {"max", 5},
            {"half", 20},
            {"low", 30},
            {"very low", 45}
        };
        std::map<std::string, char> event_zombie{
            {"doctor", 5},
            {"soldier", 30},
            {"citizen", 65}
        };
        std::map<std::string, char> event_have_syringe{
            {"yes", 1},
            {"no", 1}
        };
        std::map<std::string, char> event_have_specific_syringe{
            {"zombie_syringe", 1},
            {"no", 0}  
        };
        std::map<std::string, char> event_have_food{
            {"yes", 1},
            {"no", 1}  
        };
        std::map<std::string, char> event_have_specific_food{
            {"damaged_food_bag", 1},
            {"no", 0}  
        };
        std::map<std::string, char> event_food_number{
            {"max", 5},
            {"half", 20},
            {"low", 30},
            {"very low", 45}
        };
        new_universe("have_knife", event_can_have_knife, 2);
        new_universe("have_gun", event_can_have_gun, 5);
        new_universe("have_specific_gun", event_can_have_specific_gun, 100);
        new_universe("have_cartridge", event_can_have_cartridge, 10);
        new_universe("have_specific_cartridge", event_can_have_specific_cartridge, 100);
        new_universe("durability", event_durability, 100);
        new_universe("ammo", event_ammo, 100);
        new_universe("cartridge_number", event_cartridge_number, 100);
        new_universe("zombie", event_zombie, 100);
        new_universe("have_syringe", event_have_syringe, 2);
        new_universe("have_specific_syringe", event_have_specific_syringe, 1);
        new_universe("syringe_number", event_syringe_number, 100);
        new_universe("have_food", event_have_food, 2);
        new_universe("have_specific_food", event_have_specific_food, 1);
        new_universe("food_number", event_food_number, 100);
        Debugging::write("Intialising all probabilities terminated", Debugging::get_state_of(Debugging::In_game));
    }

    void terminate(){
        Debugging::write("Destroying all probabilities", Debugging::get_state_of(Debugging::In_game));
        while(!all_universes.empty()){
            all_universes[0]->terminate();
            delete all_universes[0];
            all_universes.erase(all_universes.begin());
        }
        all_universes.clear();
        Debugging::write("Destroying all probabilities terminated", Debugging::get_state_of(Debugging::In_game));
    }

    Universe::Universe(std::string name, std::map<std::string, char> all_events, char max_percentage){
        this->universe_name = name;
        this->universe_events_proba = all_events;
        this->max_percentage = max_percentage;
        for(auto event : this->universe_events_proba)
            this->all_events.push_back(event.first);
        this->init();
    }

    void Universe::init(){
        this->contain.clear();
        char count = 0;
        for(auto event : this->universe_events_proba){
            count += event.second;
        }
        if(count == this->max_percentage){
            std::map<std::string, char> all_proba = this->universe_events_proba;
            int proba_added = 0;
            while(proba_added < this->all_events.size()){
                int proba_to_add_location = Main::random_value() % this->all_events.size();
                if(all_proba[this->all_events[proba_to_add_location]] > 0){
                   all_proba[this->all_events[proba_to_add_location]]--;
                   this->contain.push_back(proba_to_add_location);
                }
                proba_added = 0;
                for(auto proba : all_proba){
                    if(proba.second <= 0){
                        proba_added++;
                    }
                }
            }
        }
    }

    void Universe::terminate(){
        this->universe_events_proba.clear();
        this->contain.clear();
        this->all_events.clear();
    }

    std::string Universe::get_result(){
        std::string result = "";
        char result_location = Main::random_value() % this->max_percentage;
        result = this->all_events[this->contain[result_location]];
        return result;
    }

    std::map<std::string, char> Universe::get_events(){
        return this->universe_events_proba;
    }

    std::string Universe::get_name(){
        return this->universe_name;
    }

    std::vector<std::string> Universe::get_content(){
        std::vector<std::string> content;
        for(int i = 0; i < this->max_percentage; i++){
            content.push_back(this->all_events[this->contain[i]]);
        }
        return content;
    }

    void new_universe(std::string name, std::map<std::string, char> all_events, char max_percentage){
        all_universes.push_back(new Universe(name, all_events, max_percentage));
    }

    void delete_universe(std::string name){
        for(int i = 0; i < all_universes.size(); i++){
            if(all_universes[i]->get_name() == name){
                all_universes.erase(all_universes.begin() + i);
                break;
            }
        }
    }

    Universe* get_universe(std::string name){
        Universe* universe = nullptr;
        for(int i = 0; i < all_universes.size(); i++){
            if(all_universes[i]->get_name() == name){
                universe = &*all_universes[i];
                break;
            }
        }
        return universe;
    }

}