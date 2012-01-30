#include <iostream>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "map2d.hpp"
#include "char_2d.hpp"

const int SCREEN_W = 500;
const int SCREEN_H = 500;

double calc_fps() {
	static double old_time = 0.0;
	double new_time = al_get_time();
    double delta = new_time - old_time;
    double fps = 1.0/(delta);
    old_time = new_time;
	return fps;
}

int main() {
    al_init();
	al_init_font_addon();
	al_init_ttf_addon();
    al_install_keyboard();
	al_init_primitives_addon();
    al_init_image_addon();
    
    //acquire allegro resources
    ALLEGRO_DISPLAY *display = al_create_display(SCREEN_W, SCREEN_H);
	ALLEGRO_FONT *fpsfont = al_load_ttf_font("resources//fonts//pirulen.ttf", 12, 0);
    ALLEGRO_TIMER *timer = al_create_timer(1.0/60);
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    
    bool exit = false; //exit game
    bool redraw = false; //redraw screen

    al_start_timer(timer);
    
    character *char2d = new char_2d(60, 40, 2.0, 0.0, "resources//mariobros");
    char2d->init();
    
    map2d mymap("resources//map.txt");
    
    double fps = 0.0;
    while (!exit) {
        do {
			ALLEGRO_EVENT event;
			al_wait_for_event(event_queue, &event);
        
			//if user closes window
			if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
				exit = true;
			}
        
			if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
				//pressing ESC also exits the loop.
				if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
					exit = true;
				}
			}

			char2d->handle_input(event);
        
			if (event.type == ALLEGRO_EVENT_TIMER) {
				mymap.check_collision_with(char2d);
				char2d->update();
             
				redraw = true;
				fps = calc_fps();
			}
        } while (!al_is_event_queue_empty(event_queue));
        
		if (redraw) {
            al_clear_to_color(al_map_rgb(0,0,0));
            
            char2d->draw();
			//char2d->print();
            mymap.draw();
			
			al_draw_textf(fpsfont, al_map_rgb(255,255,255), 0, SCREEN_H - 12, 0, "FPS = %.0f", fps);
            
            al_flip_display();
            redraw = false;
        }
    }
    
	delete char2d;
	
    //release allegro resources
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
}