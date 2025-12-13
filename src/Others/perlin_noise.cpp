#include "../../headers/Others/perlin_noise.hpp"
#include <math.h>

namespace PerlinNoise{

    std::list<Object*> all_noises;

    bool exists(std::string name){
        bool existance = false;
        for(auto noise : all_noises){
            if(noise->get_name() == name){
                existance = true;
                break;
            }
        }
        return existance;
    }

    float deg_to_rad(unsigned int deg){
        float rad = 0;
        rad = (deg*3.14159265)/180;
        return rad;
    }

    int rad_to_deg(double rad){
        float deg = 0;
        deg = (rad*180)/3.14159265;
        return deg;
    }

    Vector get_gradiant_at(int x, int y, int seed){
        const unsigned w = 8 * sizeof(unsigned);
        const unsigned s = w / 2; 
        unsigned a = abs(x), b = abs(y);
        a *= seed;
    
        b ^= a << s | a >> w - s;
        b *= 1911520717;
    
        a ^= b << s | b >> w - s;
        a *= 2048419325;

        float random = a * (3.14159265 / ~(~0u >> 1)); // in [0, 2*Pi]
        Vector vector;
        vector.x = cos(random);
        vector.y = sin(random);
    
        return vector;
    }

    float scalar_product(int ix, int iy, float x, float y, int seed) {
        // Get gradient from integer coordinates
        Vector gradient = get_gradiant_at(ix, iy, seed);
     
        // Compute the distance vector
        float dx = x - (float)ix;
        float dy = y - (float)iy;
     
        // Compute the dot-product
        return (dx * gradient.x + dy * gradient.y);
    }

    float interpolate(float a0, float a1, float w){
        return (a1 - a0) * (3.0 - w * 2.0) * w * w + a0;
    }

    Interval::Interval(){
        this->min = 0;
        this->max = 1;
    }

    Interval::Interval(int minimum, int maximum){
        if(minimum > maximum)
            minimum = 0;
        if(maximum < minimum){
            maximum = minimum;
            minimum = 0;
        }
        this->min = minimum;
        this->max = maximum;
    }

    int Interval::get_max(){
        return this->max;
    }

    int Interval::get_min(){
        return this->min;
    }

    void Interval::copy_to(Interval* interval){
        interval->min = this->min;
        interval->max = this->max;
    }

    bool Interval::has(float value){
        bool existance = false;
        if(this->min <= value && value <= this->max)
            existance = true;
        return existance;
    }

    Object::Object(std::string noise_name, float frequence, float amplitude, int octave, long long seed, int cell_size, sf::Vector2i grid_max_size){
        this->name = noise_name;
        this->frequence = frequence;
        this->amplitude = amplitude;
        this->octave = octave;
        this->seed = seed;
        this->cell_size = cell_size;
        this->grid_size = grid_max_size;
        this->pixels_color.clear();
    }

    Object::Object(std::string file_name, bool default_settings, std::fstream *file){
        if(!default_settings){
            if(!std::filesystem::exists(file_name)){
                file->close();
                file->open(file_name, std::ios::binary | std::ios::out);
                file->close();
            }
            else{
                file->close();
                file->open(file_name, std::ios::in | std::ios::binary);
                size_t noise_name_size = 0;
                file->read(reinterpret_cast<char*>(&noise_name_size), sizeof(size_t));

                std::string noise_name;
                noise_name.resize(noise_name_size);
                file->read(&noise_name[0], sizeof(char)*noise_name_size);
                this->name = noise_name;

                int seed = 0;
                file->read(reinterpret_cast<char*>(&seed), sizeof(int));
                this->seed = seed;

                float frequence = 0.0f;
                float amplitude = 0.0f;
                int octave = 1;
                int cell_size = 1;
                int grid_size_x = 1;
                int grid_size_y = 1;
                file->read(reinterpret_cast<char*>(&frequence), sizeof(float));
                file->read(reinterpret_cast<char*>(&amplitude), sizeof(float));
                file->read(reinterpret_cast<char*>(&octave), sizeof(int));
                file->read(reinterpret_cast<char*>(&cell_size), sizeof(int));
                file->read(reinterpret_cast<char*>(&grid_size_x), sizeof(int));
                file->read(reinterpret_cast<char*>(&grid_size_y), sizeof(int));
                this->frequence = frequence;
                this->amplitude = amplitude;
                this->octave = octave;
                this->cell_size = cell_size;
                this->grid_size.x = grid_size_x;
                this->grid_size.y = grid_size_x;

                size_t color_number = 1;
                file->read(reinterpret_cast<char*>(&color_number), sizeof(size_t));

                for(int i = 0; i < color_number; i++){
                    int min = 1;
                    int max = 2;
                    file->read(reinterpret_cast<char*>(&min), sizeof(int));
                    file->read(reinterpret_cast<char*>(&max), sizeof(int));
                    Interval interval(min, max);

                    char red = 0;
                    char green = 0;
                    char blue = 0;
                    char alpha = 0;
                    file->read(reinterpret_cast<char*>(&red), sizeof(char));
                    file->read(reinterpret_cast<char*>(&green), sizeof(char));
                    file->read(reinterpret_cast<char*>(&blue), sizeof(char));
                    file->read(reinterpret_cast<char*>(&alpha), sizeof(char));

                    this->configure_color(interval, sf::Color(red, green, blue, alpha));
                }
            }
        }
        else{
            this->name = "Noise";
            this->frequence = 0.1f;
            this->amplitude = 0.1f;
            this->octave = 1;
            this->seed = 1;
            this->cell_size = 20;
            this->grid_size = sf::Vector2i(500, 500);
            this->pixels_color.clear();
        }
    }

    void Object::set_name_to(std::string new_name){
        this->name = new_name;
    }

    void Object::set_frequence_to(float new_frequence){
        this->frequence = new_frequence;  
    }

    void Object::set_amplitude_to(float new_amplitude){
        this->amplitude = new_amplitude; 
    }

    void Object::set_octave_to(int new_octave){
        this->octave = new_octave;  
    }

    void Object::set_seed_to(int new_seed){
        this->seed = new_seed;  
    }

    void Object::set_cell_size_to(int new_cell_size){
        this->cell_size = new_cell_size;  
    }

    void Object::set_grid_size_to(sf::Vector2i new_grid_size){
        this->grid_size = new_grid_size;  
    }

    std::string Object::get_name(){
        return this->name;
    }

    float Object::get_frequence(){
        return this->frequence;  
    }

    float Object::get_amplitude(){
        return this->amplitude; 
    }

    int Object::get_octave(){
        return this->octave;  
    }

    long long Object::get_seed(){
        return this->seed;  
    }

    int Object::get_cell_size(){
        return this->cell_size;  
    }

    std::map<std::shared_ptr<Interval>, Pixel_color> Object::get_pixels_colors(){
        return this->pixels_color;
    }

    float Object::get_noise_at(float x, float y){
        Vector point;
        point.x = x;
        point.y = y;

        Vector cell, cell_corner;
        cell.x = (int)point.x;
        cell.y = (int)point.y;
        cell_corner.x = cell.x + 1;
        cell_corner.y = cell.y + 1;

        Vector distance;
        distance.x = point.x - (float)cell.x;
        distance.y = point.y - (float)cell.y;

        float dot_product_top_left, dot_product_top_right;
        dot_product_top_left = scalar_product(cell.x, cell.y, point.x, point.y, this->seed);
        dot_product_top_right = scalar_product(cell_corner.x, cell.y, point.x, point.y, this->seed);

        float dot_product_down_left, dot_product_down_right;
        dot_product_down_left = scalar_product(cell.x, cell_corner.y, point.x, point.y, this->seed);
        dot_product_down_right = scalar_product(cell_corner.x, cell_corner.y, point.x, point.y, this->seed);

        float vert_interp1, vert_interp2, horiz_interp;
        vert_interp1 = interpolate(dot_product_top_left, dot_product_top_right, distance.x);
        vert_interp2 = interpolate(dot_product_down_left, dot_product_down_right, distance.x);

        horiz_interp = interpolate(vert_interp1, vert_interp2, distance.y);
        return horiz_interp;
    }

    void Object::configure_color(Interval interval, sf::Color color){
        Pixel_color pixel_color;
        pixel_color.red = color.r;
        pixel_color.green = color.g;
        pixel_color.blue = color.b;
        pixel_color.alpha = color.a;
        this->pixels_color[std::make_shared<Interval>(interval)] = pixel_color;
    }

    sf::Color Object::adapt_color(int color){
        sf::Color color_result;
        sf::Color initial_color(color, color, color, 255);
        if(this->pixels_color.empty() == false){
            for(auto pixel_color : this->pixels_color){
                if(pixel_color.first->has(color)){
                    color_result.r = pixel_color.second.red;
                    color_result.g = pixel_color.second.green;
                    color_result.b = pixel_color.second.blue;
                    color_result.a = pixel_color.second.alpha;
                    break;
                }
            }
        }
        else{
            color_result = initial_color;
        }
        return color_result;
    }

    sf::Color Object::get_color_at(int x, int y){
        float value = 0;
        float freq = this->frequence;
        float amp = this->amplitude;
        for(int o = 0; o < this->octave; o++){
            value += this->get_noise_at(x*freq/this->cell_size, y*freq/this->cell_size)*amp;
            freq *= 2;
            amp /= 2;
        }

        if(value > 1.0)
            value = 1.0;
        if(value < -1.0)
            value = -1.0;

        int color_value = (int)(((value + 1.0f) * 0.5f) * 255);
        sf::Color color = this->adapt_color(color_value);
        
        return color;
    }

    void new_noise(std::string noise_name, float frequence, float amplitude, int octave, long long seed, int cell_size, sf::Vector2i grid_max_size){
        if(exists(noise_name) == false)
            all_noises.push_back(new Object(noise_name, frequence, amplitude, octave, seed, cell_size, grid_max_size));
    }

    Object* get(std::string name){
        Object* the_noise = nullptr;
        for(auto noise : all_noises){
            if(noise->get_name() == name){
                the_noise = &*noise;
                break;
            }
        }
        return the_noise;
    }

    void add(Object* new_noise){
        all_noises.push_back(new_noise);
    }

    void init(){
        Debugging::write("Perlin noises initialisation");
        if(all_noises.empty() == false){
            for(auto noise : all_noises){
                if(noise != nullptr){
                    delete noise;
                    noise = nullptr;
                }
            }
        }
        all_noises.clear();
        Debugging::write("Perlin noises initialisation terminated");
    }
    
    void remove(std::string name){
        for(auto noise : all_noises){
            if(noise->get_name() == name){
                delete noise;
                all_noises.remove(noise);
                break;
            }
        }
    }

    void clear(){
        for(auto& noise : all_noises){
            delete noise;
            noise = nullptr;
        }
        all_noises.clear();
    }

};