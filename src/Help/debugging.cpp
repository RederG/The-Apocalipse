#include "../../headers/Help/debugging.hpp"
#include <ctime>

std::map<Debugging::Type, bool> Debugging_types_state{
    {Debugging::Button, false},
    {Debugging::File, false},
    {Debugging::Description, false},
    {Debugging::In_game, false},
    {Debugging::Probability, false},
    {Debugging::Nothing, false}
};

namespace Debugging{
    std::mutex mutex1;
    std::mutex mutex2;

    std::string get_time(){
        time_t now = std::time(0);
        char* string_now = ctime(&now);

        std::string time(string_now);
        time.erase(time.begin() + time.size() - 1);
        return time;
    }

    void write(std::string something, bool allowed, bool back_to_line){
        std::unique_lock<std::mutex> lock(mutex1);
        
        something = "[" + get_time() + "] : " + something;

        std::fstream log_write;
        log_write.open("../all_events.log", std::ios_base::app);
        if(allowed == true && back_to_line == true){
            log_write << something;
            log_write << "\n";
        }
        if(allowed == true && back_to_line == false)
            log_write << something;
        log_write.close();

        lock.unlock();
    }

    void write(std::string something, std::string log_file_location, bool allowed, bool back_to_line){
        std::unique_lock<std::mutex> lock(mutex2);

        something = "[" + get_time() + "] : " + something;

        std::fstream log_write;
        log_write.open(log_file_location, std::ios_base::app);
        if(allowed == true && back_to_line == true){
            log_write << something;
            log_write << "\n";
        }
        if(allowed == true && back_to_line == false)
            log_write << something;
        log_write.close();
        
        lock.unlock();
    }

    void clear(){
        std::fstream log_file;
        log_file.open("../all_events.log", std::ios_base::out);
        if(log_file.is_open()){
            log_file << "";
            log_file.close();
        }
        log_file.open("../algo.log", std::ios_base::out);
        if(log_file.is_open()){
            log_file << "";
            log_file.close();
        }
    }

    bool get_state_of(Type type){
        return Debugging_types_state[type];
    }

    void enable_all(){
        for(int i = 0; i < Debugging_types_state.size(); i++){
            Type type = Type(i);
            Debugging_types_state[type] = true;
        }
    }

    void disable_all(){
        for(int i = 0; i < Debugging_types_state.size(); i++){
            Type type = Type(i);
            Debugging_types_state[type] = false;
        }
    }

    void enable(Type type){
        Debugging_types_state[type] = true;
    }

    void disable(Type type){
        Debugging_types_state[type] = false;
    }
}