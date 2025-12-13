#ifndef __PROBABILITY__
    #define __PROBABILITY__

    #include "../main.hpp"

    // For all probabilities for the game
    namespace Probability{
        // The universe of all probabilities
        class Universe{
            public:
                // Contructs an universe
                Universe(std::string name, std::map<std::string, char> all_events_proba, char max_percentage);

                // Gets the result of hasard of the universe
                std::string get_result();

                // Gets all events that can appears on the universe
                std::map<std::string, char> get_events();

                // Gets the contain of the universe
                std::vector<std::string> get_content();

                // Gets the name of the universe
                std::string get_name();
                
                void init();

                void terminate();

            private:
                std::string universe_name;
                std::map<std::string, char> universe_events_proba;
                char max_percentage;
                std::vector<std::string> all_events;
                std::vector<char> contain;
        };
        
        // Creates a new universe
        void new_universe(std::string name, std::map<std::string, char> events_proba, char max_percentage);

        // Delete a specific universe
        void delete_universe(std::string name);

        // Gets the universe with the specific name
        Universe* get_universe(std::string name);

        // Inits all universes
        void init();

        // Deletes all universes
        void terminate();
    }

#endif