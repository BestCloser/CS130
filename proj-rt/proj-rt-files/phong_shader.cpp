#include "light.h"
#include "phong_shader.h"
#include "ray.h"
#include "render_world.h"
#include "object.h"

vec3 Phong_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& normal,int recursion_depth) const
{
    vec3 color;
    
	/*
		Render_World& world_input,
        const vec3& color_ambient,
        const vec3& color_diffuse,
        const vec3& color_specular,
        double specular_power
	*/
	
    //I = I_a + I_d + I_s
    //  = R_a * L_a + R_d * L_d * max(n dot l, 0) + R_s * L_s * max(v dot r, 0)^a   <--
	//                            n dot l = cos(theta)          v dot r = cos(phi)  a is alpha
    //Emitted_Light(const vec3& vector_to_light)
    vec3 I_a = color_ambient * world.ambient_intensity * world.ambient_color;
    //vec3 I_d = {0,0,0};
    //vec3 I_s = {0,0,0};


    //color = R_a * Emitted_Light(this->color_ambient) +
	//		  R_d * Emitted_Light(this->color_diffuse)*max() + 
	//		  R_s * Emitted_Light(this->color_specular)
    color = I_a; // + I_d + I_s;

	//PLS WORK
	
	
    return color;
}
