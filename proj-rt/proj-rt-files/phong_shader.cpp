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
	
	//I_a = ambient intensity = object's ambient color * world's ambient color * world's ambient intensity
    vec3 I_a = color_ambient * world.ambient_color * world.ambient_intensity;
    vec3 I_d, I_s;
	double diffMax, specMax;


	Light *curr;
    
    
    for (unsigned int i = 0; i < world.lights.size(); ++i) {
        curr = world.lights.at(i);
        
        vec3 L = curr->position - intersection_point;

        //vec3 I_d = color_diffuse * Emitted_Light(L) * max(dot(n.L, 0);
        diffMax = std::max(dot(normal.normalized(), L.normalized()), 0.0);
    
       	I_d = I_d + color_diffuse * curr->Emitted_Light(L) * diffMax;
        
       	//vec3 I_s = color_specular * ray.Emitted_Light(L) * max(v.r, 0)^a;
        specMax = pow(std::max(dot((intersection_point - ray.endpoint).normalized(), (L - 2*dot(L, normal) * normal).normalized()), 0.0), specular_power);
       	
        I_s = I_s + color_specular * curr->Emitted_Light(L) * specMax;
    }
       
	//this looks wrong but it works i guess... pls work for rest of project

    //color = R_a * Emitted_Light(this->color_ambient) +
	//		  R_d * Emitted_Light(this->color_diffuse) + 
	//		  R_s * Emitted_Light(this->color_specular)
    color = I_a + I_d + I_s;

    return color;
}
