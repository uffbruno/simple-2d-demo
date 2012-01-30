#include <iostream>
#include "char_2d.hpp"

char_2d::char_2d(float x, float y, float sx, float sy, 
    const std::string& directory): character(x, y, sx, sy, directory), 
                                   gravity_force(0.2), 
                                   jumping_force(7), 
                                   vertical_speed_limit(8) {

	
	state = CHAR2D_JUMPING_RIGHT;
	
	// for debugging purposes
	statenames[CHAR2D_STILL_LEFT]    = "CHAR2D_STILL_LEFT";
	statenames[CHAR2D_STILL_RIGHT]   = "CHAR2D_STILL_RIGHT";
	statenames[CHAR2D_WALKING_LEFT]  = "CHAR2D_WALKING_LEFT";
	statenames[CHAR2D_WALKING_RIGHT] = "CHAR2D_WALKING_RIGHT";
	statenames[CHAR2D_JUMPING_LEFT]  = "CHAR2D_JUMPING_LEFT";
	statenames[CHAR2D_JUMPING_RIGHT] = "CHAR2D_JUMPING_RIGHT";
}

char_2d::~char_2d() {
}

unsigned int char_2d::impl_update() {
    //gravity action
	speed.y += gravity_force;
	
	//limit the falling speed
    if (speed.y > vertical_speed_limit) {
        speed.y = vertical_speed_limit;
    }
	
	center.y += speed.y;

	//up arrow is for jumping
	if (keys[KEY_UP]) {
        //if our character is not jumping
		if (state != CHAR2D_JUMPING_LEFT && state != CHAR2D_JUMPING_RIGHT) {
            
			//apply a jumping force, opposite to gravity
			//the gravity will take care of pulling our character 
			speed.y = -jumping_force;
            
			//determine direction character is facing
			if (state == CHAR2D_WALKING_LEFT || state == CHAR2D_STILL_LEFT) {
                state = CHAR2D_JUMPING_LEFT;
            } 
			else if (state == CHAR2D_WALKING_RIGHT || state == CHAR2D_STILL_RIGHT) {
                state = CHAR2D_JUMPING_RIGHT;
            }
        }
    }
    
	//move left. even if in air.
    if (keys[KEY_LEFT]) {
        center.x -= speed.x;
		
		//if on ground, change state to walking state
        if (state != CHAR2D_JUMPING_LEFT && state != CHAR2D_JUMPING_RIGHT) {
			state = CHAR2D_WALKING_LEFT;
		} else {
			state = CHAR2D_JUMPING_LEFT;
		}
	}
	
	//same for moving right
    else if (keys[KEY_RIGHT]) {
        center.x += speed.x;
        if (state != CHAR2D_JUMPING_LEFT && state != CHAR2D_JUMPING_RIGHT) {
			state = CHAR2D_WALKING_RIGHT;
		} else {
			state = CHAR2D_JUMPING_RIGHT;
		}
    } else { 
        //if no keys are pressed, character is still, and facing direction
        //he / she was originally moving
        if (state == CHAR2D_WALKING_LEFT) state = CHAR2D_STILL_LEFT;
        else if (state == CHAR2D_WALKING_RIGHT) state = CHAR2D_STILL_RIGHT;
    }
   
    return state;
}

void char_2d::print() {
	std::cout << "current state = " << statenames[state] << std::endl;
}

void char_2d::impl_draw() {
    animation *anim = spr.get_animation();
	if (!anim) return;
	
	frame *f = anim->get_frame();
	if (!f) return;
	
	int w = al_get_bitmap_width(f->image);
	int h = al_get_bitmap_height(f->image);

    al_draw_bitmap(f->image, center.x - w/2, center.y - h/2, 0);
}

vector2d char_2d::handle_collision(const character* other) {
	vector2d normal = character::handle_collision(other);

	//some handling for when the character falls (collision from above)
	if (normal.y < 0) {
		//reset y speed so gravity increases when the character falls again
		if (speed.y >0) speed.y = 0;
		
		//update state
		if (state == CHAR2D_JUMPING_LEFT) state = CHAR2D_STILL_LEFT;
		else if (state == CHAR2D_JUMPING_RIGHT) state = CHAR2D_STILL_RIGHT;
	}
	
	return normal;
}