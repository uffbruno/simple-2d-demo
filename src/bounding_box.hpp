#ifndef BOUNDING_BOX_HPP
#define BOUNDING_BOX_HPP

#include <allegro5/allegro.h>

#include "vector2d.hpp" 

class bounding_box {
    public:
        bounding_box(float x, float y, float w, float h);
        ~bounding_box();
        
        float left() const   { return center.x - w/2; }
        float top() const    { return center.y - h/2; }
        float right() const  { return center.x + w/2; }
        float bottom() const { return center.y + h/2; }
        
        void set_x(float x) { center.x = x; }
        void set_y(float y) { center.y = y; }
        
        float get_x() { return center.x; }
        float get_y() { return center.y; }
		float get_w() { return w; }
		float get_h() { return h; }

        void draw() const;
        void update();
        void set_color(ALLEGRO_COLOR color) { this->color = color; }
        
        bool collide(const bounding_box* other, vector2d& normal) const;

    private:
        vector2d center;
		
        float w;
        float h;
      
        ALLEGRO_COLOR color;
        bounding_box(const bounding_box& other);
        bounding_box& operator=(const bounding_box& other);
};

#endif //BOUNDING_BOX_HPP