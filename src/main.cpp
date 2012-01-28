#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "bounding_box.hpp"

int main() {
    al_init();
    al_install_keyboard();
    al_init_primitives_addon();
    
    //acquire allegro resources
    ALLEGRO_DISPLAY *display = al_create_display(640,480);
    ALLEGRO_TIMER *timer = al_create_timer(1.0/60);
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    
    bool exit = false; //exit game
    bool redraw = false; //redraw screen

    al_start_timer(timer);
    
    bounding_box bb(300, 0, 30, 30, 2, 0);
    bb.set_color(al_map_rgb(255,255,255));
    
    bounding_box bb2(300, 300, 30, 30, 0, 0);
    bb2.set_color(al_map_rgb(255, 255, 0));
        
    while (!exit) {
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
        
        bb.handle_input(event);
        
        if (event.type == ALLEGRO_EVENT_TIMER) {
            bb.update();
            bb.handle_collide(bb2);
            
            redraw = true;
        }
        
        if (redraw && al_is_event_queue_empty(event_queue)) {
            al_clear_to_color(al_map_rgb(0,0,0));
            bb.draw();
            bb2.draw(); 
            al_flip_display();
            redraw = false;
        }
    }
    
    //release allegro resources
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
}