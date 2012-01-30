#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <vector>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include "bounding_box.hpp"

struct frame {
    bounding_box *bb;
    ALLEGRO_BITMAP *image;
    
    frame(): bb(NULL), image(NULL) {
    }
	
	frame(bounding_box *bb, ALLEGRO_BITMAP *image): bb(bb), image(image) {
	}
    
    frame(const frame& other): 
		bb(new bounding_box(other.bb->get_x(), other.bb->get_y(), other.bb->get_w(), other.bb->get_h())), 
        image(other.image) {
    }
    
    frame& operator=(const frame& other) {
        this->bb = new bounding_box(other.bb->get_x(), other.bb->get_y(), other.bb->get_w(), other.bb->get_h());
        this->image = other.image;
        
        return *this;
    }
	
	~frame() {
	}
};

typedef struct frame frame;

typedef std::vector<frame*> collection_frames;

class animation {
    public:
        animation(float speed);
        ~animation();
        
        void add_frame(frame *f);
        void set_speed(const float speed);
        float get_speed() const;
        void next();
        
		frame* get_frame();
		void set_frame(frame *f);
        
    private:
        float speed;
        float curr_speed;
        int current_frame; //time between frames.
        collection_frames frames;
        
        //can't use copy constructor
        animation(const animation& other);
        
        //can't use assignment operator
        animation& operator=(const animation& other);
};

#endif //ANIMATION_HPP