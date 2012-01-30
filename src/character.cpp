#include "character.hpp"
#include <sstream>
#include <fstream>
#include <iostream>

character::character(float pos_x, float pos_y, float speed_x, float speed_y, 
    const std::string& directory): center(pos_x, pos_y), 
                                   speed(speed_x, speed_y), 
                                   spr(), 
                                   loaded(false), 
                                   state(0), 
                                   directory(directory.c_str()) {
    
    keys[KEY_LEFT]  = false; 
    keys[KEY_RIGHT] = false;
    keys[KEY_UP]    = false;
    keys[KEY_DOWN]  = false;
}

character::~character() {
}

void character::init() {
    if (!loaded) {
        loaded = load_frames();
    }
}

bool character::load_frames() {
    std::ostringstream oss;
    oss << directory << "//anim_info.txt";

    std::ifstream file(oss.str().c_str());
  
    if (!file) {
        std::cout << "error opening " << oss.str().c_str() << std::endl;
        return false;
    }
    
    std::string line;
    animation *anim = NULL;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#' || line[0] == '\n') {
            continue;
        } else if (line[0] == '.') {
            if (anim != NULL) {
                spr.add_animation(anim);
            }
            
            anim = NULL;
        } else {
            std::ostringstream oss_frame;        
            oss_frame << directory << "//" << line;

            if (anim == NULL) {
                anim = new animation(8.0);
            }

            std::cout << "opening " << oss_frame.str().c_str() << std::endl;
            ALLEGRO_BITMAP *image = al_load_bitmap(oss_frame.str().c_str());
            if (image == NULL) {
                std::cout << "error loading frame" << std::endl;
            } else {
				bounding_box *bb = new bounding_box(center.x, center.y, 
									al_get_bitmap_width(image), 
									al_get_bitmap_height(image));
				bb->set_color(al_map_rgb(255,255,255));
                frame *f = new frame(bb, image);
                anim->add_frame(f);
            }
        }
    }
    
    file.close();
    return true;
}

bounding_box *character::get_bounding_box() const {
    animation *anim = spr.get_animation();
    bounding_box *bb = NULL;
	
	if (anim != NULL) {
        frame *f = anim->get_frame();
        bb = f->bb;
    }
    
    return bb;
}

vector2d character::handle_collision(const character* other) {
    if (other == NULL) return vector2d(0,0);
    
    vector2d normal(0,0);
    animation *a1 = this->spr.get_animation();
    animation *a2 = other->spr.get_animation();
    
    if (!a1 || !a2) return vector2d(0,0);
	
	bounding_box *bb1 = this->get_bounding_box();
	bounding_box *bb2 = other->get_bounding_box();
    
	if (!bb1 || !bb2) return vector2d(0,0);
	
    if (bb1->collide(bb2, normal)) {
        //push this box out of the other one
        this->center.x += normal.x;
        this->center.y += normal.y;
        
		//update bounding box
		bb1->set_x(this->center.x);
        bb1->set_y(this->center.y);
    }
    
    return normal;
}

void character::set_state(const unsigned int state) {
    unsigned int num_animations = spr.get_num_animations();
    if (state > num_animations) {
        this->state = 0;
    }
    else {
        this->state = state;
    }
}

void character::handle_input(const ALLEGRO_EVENT& ev) {
    if (ev.type == ALLEGRO_EVENT_KEY_DOWN || ev.type == ALLEGRO_EVENT_KEY_UP) {
        switch(ev.keyboard.keycode) {
            case ALLEGRO_KEY_UP:
                keys[KEY_UP] = (ev.type == ALLEGRO_EVENT_KEY_DOWN);
                break;
            case ALLEGRO_KEY_DOWN:
                keys[KEY_DOWN] = (ev.type == ALLEGRO_EVENT_KEY_DOWN);
                break;
            case ALLEGRO_KEY_LEFT:
                keys[KEY_LEFT] = (ev.type == ALLEGRO_EVENT_KEY_DOWN);
                break;
            case ALLEGRO_KEY_RIGHT:
                keys[KEY_RIGHT] = (ev.type == ALLEGRO_EVENT_KEY_DOWN);
                break;
        }
    }
}

void character::update() {
    if (loaded) {
        state = impl_update();
        
        //animates the character
        spr.set_animation(state);
        spr.run_animation();
        
		bounding_box *bb = get_bounding_box();
		
		if (!bb) return;
        bb->set_x(center.x);
        bb->set_y(center.y);
    }
}

void character::print() {
    bounding_box *bb = get_bounding_box();
	if (!bb) return;
	
    std::cout << "center.x = " << center.x << std::endl;
    std::cout << "center.y = " << center.y << std::endl;
    std::cout << "f.bb.center.x = " << bb->get_x() << std::endl;
    std::cout << "f.bb.center.y = " << bb->get_y() << std::endl;
}

void character::draw() {
    if (loaded) {
        impl_draw();
    }
}