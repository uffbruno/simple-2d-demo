#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include "vector2d.hpp"
#include "sprite.hpp"

enum keycodes {
    KEY_UP,
    KEY_DOWN,
    KEY_LEFT,
    KEY_RIGHT
};

typedef enum keycodes keycodes;

class character {
    public:
        character(float pos_x, float pos_y, float speed_x, float speed_y, 
            const std::string& directory);
        virtual ~character();
        
        //initializes sprites, mandatory step. without it anything else will work.
        void init(); 
        
        void set_pos(float x, float y) { set_x(x); set_y(y); }
        void set_x(float x) { this->center.x = x; }
        void set_y(float y) { this->center.y = y; }
        void set_state(const unsigned int state);
        unsigned int get_state() const { return state; }
        bounding_box* get_bounding_box() const;
        virtual void print();
        
        void handle_input(const ALLEGRO_EVENT& ev);
        void update();
        void draw();

        virtual vector2d handle_collision(const character* other);
    
    protected:
        vector2d center;
        vector2d speed;
        sprite spr;
        
        bool loaded;
        unsigned int state;
        std::string directory; //directory that contains the frames and the anim_info.txt file.
        bool keys[4];
        
        bool load_frames();
        virtual unsigned int impl_update() = 0;
        virtual void impl_draw()           = 0;

    private:
        character(const character& other);
        character& operator=(const character& other);
};

#endif //CHARACTER_HPP