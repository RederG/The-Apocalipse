#ifndef __DEBUGGING__
    #define __DEBUGGING__

    #include "../main.hpp"

    // Controller of the debugging information on the
    // terminal
    namespace Debugging{
        enum Type{
            Button, File, Description, In_game, Probability, Nothing
        };

        // Writes something on a log file
        void write(std::string something = "Something", bool allowed = true, bool back_to_line = true);

        // Writes something on a log file
        void write(std::string something, std::string log_file_location, bool allowed = true, bool back_to_line = true);

        // Clears the contain of the file log
        void clear();

        // Gets the state(true or false) of one type of debugging
        bool get_state_of(Type type);

        // Allows all debugging
        void enable_all();

        // Disable all debugging
        void disable_all();

        // Activate a specific type of debugging
        void enable(Type type);

        // Disable a specific type of debugging
        void disable(Type type);
    }

#endif