#ifndef APPLICATION_H
#define APPLICATION_H

#include <vector>
#include <map>

#include "gl_texture.h"
#include "gl_viewer.h"
#include "obj.h"
#include "timer.h"

typedef std::map<std::string, gl_image_texture*>
    gl_image_texture_map;

class application : public gl_viewer
{
public:
    application();
    ~application();
    void init_event();
    void draw_event();
    void mouse_click_event(int button, int button_state, int x, int y);
    void mouse_move_event(int x, int y);
    void keyboard_event(unsigned char key, int x, int y);

private:
    bool raytrace;
    int rendmode;
    int npart;
    timer t;

    obj o;
    gl_image_texture_map texs;

    bool paused;
    float sim_t;
    bool draw_volcano;
    float h;
};



struct Particle {
 public:
    //variables
    float mass; //mass of the particle
    vec3 position; //position of the particle
    vec3 old_position; //old position
    vec3 velocity; //velocity of the particle
    vec3 force; //force applied on a particle
    vec3 color; //color
    float duration; //duration of particle


    //functions
    void Euler_Step(float h);
    void Reset_Forces();
    void Handle_collision(float damping, float coeff_restitution);
};


void Add_Particles(int);
float random(float, float);

#endif
