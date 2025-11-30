#ifndef __PERLIN_NOISE__
    #define __PERLIN_NOISE__

    #include "../../main.hpp"

    namespace PerlinNoise{
        // The vector
        struct Vector{
            float x = 0, y = 0;
        };

        // The vector but with integers variables
        struct Vector2i{
            int x = 0, y = 0;
        };

        // The color for configuring the noise
        struct Pixel_color{
            int red = 0, green = 0, blue = 0, alpha = 255;
        };

        // The interval used for the colors
        class Interval{
            public:
                // Constructor of an interval
                Interval();

                // Constructor of an interval with its maximum and minimum values
                Interval(int min, int max);

                // Gets the minimum value of the interval
                int get_min();

                // Gets the maximal value of the interval
                int get_max();

                // Verifies if the value is on the interval
                bool has(float value);

                void copy_to(Interval* interval);
            private:
                int max, min;
        };
        
        // Verifies if a noise exists or not :
        // - Returns True if the noise exists
        bool exists(std::string name);

        // Transform degrees to radians
        float deg_to_rad(unsigned int deg);

        // Transform radians to degrees
        int rad_to_deg(double rad);

        // Get the rotated vector
        Vector get_gradiant_at(int x, int y, int seed);

        // Do the scalar product of two points
        float scalar_product(int ix, int iy, float x, float y, int seed);
        
        class Object{
            public:
                // Constructs a perlin noise
                Object(std::string noise_name = "Noise", float frequence = 1.0f, float amplitude = 1.0f, int octave = 1, long long seed = 1, int cell_size = 20, sf::Vector2i grid_max_size = sf::Vector2i(500,500));
                
                // Constructs a perlin noise from a file
                // - The file must ends with the extention " . settings"
                Object(std::string file_name, bool default_settings, std::fstream *file);

                // Sets the name of the noise
                void set_name_to(std::string new_name);

                // Sets the frequence of the noise
                void set_frequence_to(float new_frequence);

                // Sets the amplitude of the noise
                void set_amplitude_to(float new_amplitude);

                // Sets the octave of the noise
                void set_octave_to(int new_octave);

                // Sets the seed of the noise
                void set_seed_to(int new_seed);

                // Sets the cell's size of the noise
                void set_cell_size_to(int new_cell_size);

                // Sets the grid's size of the noise
                void set_grid_size_to(sf::Vector2i new_grid_size);

                // Sets the new begining point of the noise
                void set_init_pixel_to(Vector2i new_begining_point);

                // Gets the name of the noise
                std::string get_name();

                // Gets the frequence of the noise
                float get_frequence();

                // Gets the amplitude of the noise
                float get_amplitude();

                // Gets the octave of the noise
                int get_octave();

                // Gets the seed of the noise
                long long get_seed();

                // Gets the cell's size of the noise
                int get_cell_size();

                // Gets the grid's size of the noise
                sf::Vector2i get_grid_size();

                // Gets the pixel that the noise is working on
                Vector2i get_loading_pixel();

                // Gets the total of pixels loaded
                Vector2i get_loaded_pixels();

                // Gets all the pixels of the noise
                uint8_t* get_pixels();

                // Gets the first pixel
                Vector2i get_first_pixel();

                // Gets all pixels colors
                std::map<std::shared_ptr<Interval>, Pixel_color> get_pixels_colors();

                // Verifies if all of the noise's pixels are loaded
                bool is_terminated();

                // Gets the noise value of a specific point
                float get_noise_at(float x, float y);

                // Updates the pixel that the noise is working on
                void update_pixels();

                // Resets the pixel that the noise is working on to (0, 0)
                void reset();

                // Gets the texture of the noise (if all of the pixels are loaded)
                sf::Texture get_texture();

                // Updates all the pixels of the noise
                void create_noise();

                // Sets the pixel that the noise is working on to the next
                void next_pixel();

                // Configures the color of the noise
                //
                //\param interval an interval between 0 and 255
                //\param color the color that is used on this interval
                void configure_color(Interval interval, sf::Color color);

                // Gives the corresponding color of the color of noise value
                sf::Color adapt_color(int color);

                // Gets the color of a specific pixel
                sf::Color get_color_at(int x, int y);

                // Prepare the noise to be destroyed
                void destroy();

            private:
                std::string name;
                float frequence, amplitude;
                int octave, cell_size;
                long long seed;
                sf::Vector2i grid_size;
                Vector2i pixel;
                Vector2i init_pixel;
                uint8_t* pixels;
                std::map<std::shared_ptr<Interval>, Pixel_color> pixels_color;
        };
    
        // Returns a pointer to the perlin noise with the same name
        Object* get(std::string name);

        // Adds a new noise
        void add(Object* new_noise);
        
        // Creates properly a Perlin Noise
        void new_noise(std::string noise_name = "Noise", float frequence = 1, float amplitude = 1, int octave = 1, long long seed = 1, int cell_size = 20, sf::Vector2i grid_max_size = sf::Vector2i(500,500));

        // Inits the perlin noise properly
        void init();

        // Removes a perlin noise with the same name
        void remove(std::string name);

        // Deletes all noises
        void clear();
    };

#endif