#include <iostream>
#include <cstdlib>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "../../sglib/src/camera.hpp"
#include "../../sglib/src/map2d.hpp"
#include "../../sglib/src/char_2d.hpp"

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
    ALLEGRO_FONT *debugfont = al_load_ttf_font("resources//fonts//pirulen.ttf", 10, 0);
    ALLEGRO_TIMER *timer = al_create_timer(1.0/60);
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    
    map2d mymap;
    if (!mymap.load_map("resources//maps//map.ini")) {
        std::cout << "error loading map" << std::endl;
        std::exit(1);
    }

    camera cam;
    cam.set_size(SCREEN_W, SCREEN_H);
    
    char_2d *char2d = new char_2d(10, 20, 2.0, 0.0, "resources//mariobros");
    char2d->init();
    cam.set_focused_character(char2d);    
    
    mymap.set_always_redraw(true);
    mymap.set_map_visible_size(cam.get_width(), cam.get_height());    
    mymap.add_character(char2d);

    double fps = 0.0;
    bool exit = false; //exit game
    bool redraw = false; //redraw screen
    
    al_start_timer(timer);
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
				char2d->update();
                mymap.check_collision_with(char2d);
                cam.update();
                
                vector2d camerapos = cam.get_position();
                vector2d camerafocus(0,0);

                //update camera position and focus (respecting the x and y returned by mymap::set_map_pos)
                mymap.set_map_pos(camerapos.x, camerapos.y);
                camerafocus.x = camerapos.x + SCREEN_W/2;
                camerafocus.y = camerapos.y + SCREEN_H/2;
                
                cam.set_position(camerapos.x, camerapos.y);
                cam.set_focus(camerafocus.x, camerafocus.y);

				redraw = true;
				fps = calc_fps();
			}
        } while (!al_is_event_queue_empty(event_queue));
        
		if (redraw) {
            al_clear_to_color(al_map_rgb(0,0,0));
            
            mymap.draw();

			al_draw_textf(debugfont, al_map_rgb(255,255,255), 0, 0, 0, "char2d.x = %.0f", char2d->get_x());
            al_draw_textf(debugfont, al_map_rgb(255,255,255), 0, 12, 0, "char2d.y = %.0f", char2d->get_y());
            al_draw_textf(debugfont, al_map_rgb(255,255,255), 0, 24, 0, "cam.x = %.0f", cam.get_position().x);
            al_draw_textf(debugfont, al_map_rgb(255,255,255), 0, 36, 0, "cam.y = %.0f", cam.get_position().y);
            al_draw_textf(debugfont, al_map_rgb(255,255,255), 0, 48, 0, "cam.fx = %.0f", cam.get_focus().x);
            al_draw_textf(debugfont, al_map_rgb(255,255,255), 0, 60, 0, "cam.fy = %.0f", cam.get_focus().y);
            al_draw_textf(debugfont, al_map_rgb(255,255,255), 0, 72, 0, "mapx = %.0f", mymap.get_map_posx());
            al_draw_textf(debugfont, al_map_rgb(255,255,255), 0, 84, 0, "mapy = %.0f", mymap.get_map_posy());
            
            al_draw_textf(fpsfont, al_map_rgb(255,255,255), 0, SCREEN_H - 12, 0, "FPS = %.0f", fps);
            
            al_flip_display();
            redraw = false;
        }
    }
    
	delete char2d;
	
    //release allegro resources
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_font(fpsfont);
    al_destroy_font(debugfont);
    al_destroy_event_queue(event_queue);
}