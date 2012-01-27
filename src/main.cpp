#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

enum keycodes {
    KEY_UP,
    KEY_DOWN,
    KEY_LEFT,
    KEY_RIGHT
};

typedef enum keycodes keycodes;

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
        
        bool collide(const bounding_box& other, vector_2d& normal) const;
        bounding_box(const bounding_box& other);
        bounding_box& operator=(const bounding_box& other);
};

bounding_box::bounding_box(float x, float y, float w, float h, float speedx, float speedy):
    center(x, y), speed(speedx ,speedy), w(w), h(h), color() {
    keys[KEY_LEFT]  = false;
    keys[KEY_RIGHT] = false;
    keys[KEY_UP]    = false;
    keys[KEY_DOWN]  = false;
}

bounding_box::~bounding_box() {
}

void bounding_box::draw() {
    al_draw_rectangle(left(), top(), right(), bottom(), color, 0);
}

void bounding_box::update() {
    //determine character state based on pressed keys.
    //if any key is pressed, character is walking on correspondent direction
    
    if (keys[KEY_UP]) {
        center.y -= speed.y;
    } else if (keys[KEY_DOWN]) {
        center.y += speed.y;
    } else if (keys[KEY_LEFT]) {
        center.x -= speed.x;
    } else if (keys[KEY_RIGHT]) {
        center.x += speed.x;
    } 
}

void bounding_box::handle_collide(const bounding_box& other) {
    vector_2d normal(0,0);
    if (this->collide(other, normal)) {
        this->center.x += normal.x;
        this->center.y += normal.y;
    }
}

/*-----------------------------------------------------------------------------------------------

I have adapted this algorithm from http://forums.elysianshadows.com/viewtopic.php?f=13&t=5767
(thanks to the user N64vSNES). I'm still struggling to understand the check on the if that's 
indicated with the [NO UNDERSTAND!!!] but i guess this works fine. :-)
All comments on this algorithm were made by N64vSNES on his thread post (link above).

-------------------------------------------------------------------------------------------------*/
bool bounding_box::collide(const bounding_box& other, vector_2d& normal) const {
    // The distance between the two objects
    vector_2d distance(0,0);
   
    // The absDistance between the objects
    vector_2d abs_distance(0,0);

    // Calculate the distance between A and B
    distance.x = other.center.x - this->center.x;
    distance.y = other.center.y - this->center.y;

    // Combine both rectangles and half the returned value
    float xadd = (other.w + this->w) / 2.0f;
    float yadd = (other.h + this->h) / 2.0f;

    // Get absolute value of distance vector.
    abs_distance.x = (distance.x < 0.0f) ? -distance.x : distance.x;
    abs_distance.y = (distance.y < 0.0f) ? -distance.y : distance.y;

    //[NO UNDERSTAND!!!]
    /*If the absDistance X is less than X add and the absDistance is less thank YAdd
    then it dosen't take a genius to figure out they arn't colliding so return false*/
    if( ! ( ( abs_distance.x < xadd ) && ( abs_distance.y < yadd ) ) ) {   
        return false;
    }
    //[END OF NO UNDERSTAND!!!]

    /*Get the magnitute by the overlap of the two rectangles*/
    float xmag = xadd - abs_distance.x;
    float ymag = yadd - abs_distance.y;

    /*Determine what axis we need to act on based on the overlap*/
    if(xmag < ymag) {
      normal.x = (distance.x > 0) ? -xmag : xmag;
    }
    else if (xmag > ymag) {
      normal.y = (distance.y > 0) ? -ymag : ymag;
   }
   
   // If we reached this point then we now know there was a collision
   return true;

    // if (this->right() < other.left()) return false;
    // if (this->left() > other.right()) return false;
    // if (this->bottom() < other.top()) return false;
    // if (this->top() > other.bottom()) return false;
    // return true;
}

void bounding_box::handle_input(ALLEGRO_EVENT& ev) {
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
    
    bounding_box bb(200, 200, 30, 30, 2, 2);
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