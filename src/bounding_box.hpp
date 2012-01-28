#ifndef BOUNDING_BOX_HPP
#define BOUNDING_BOX_HPP

#include <allegro5/allegro.h>
enum keycodes {
    KEY_UP,
    KEY_DOWN,
    KEY_LEFT,
    KEY_RIGHT
};

typedef enum keycodes keycodes;

enum bb_state {
    STATE_INAIR,
    STATE_STANDING
};

typedef enum bb_state bb_state;

struct vector_2d {
    float x;
    float y;
    vector_2d(float x, float y): x(x), y(y) {
    }
};

typedef struct vector_2d vector_2d;

class bounding_box {
    public:
        bounding_box(float x, float y, float w, float h, float speedx, float speedy);
        ~bounding_box();
        
        float left() const   { return center.x - w/2; }
        float top() const    { return center.y - h/2; }
        float right() const  { return center.x + w/2; }
        float bottom() const { return center.y + h/2; }

        void draw();
        void update();
        void handle_input(ALLEGRO_EVENT& ev);
        void set_color(ALLEGRO_COLOR color) { this->color = color; }
        
        void handle_collide(const bounding_box& other);
    
    private:
        vector_2d center;
        vector_2d speed;

        float w;
        float h;
 
        bool keys[4];
        
        ALLEGRO_COLOR color;
        bb_state state;
        
        bool collide(const bounding_box& other, vector_2d& normal) const;
        bounding_box(const bounding_box& other);
        bounding_box& operator=(const bounding_box& other);
};

#endif //BOUNDING_BOX_HPP