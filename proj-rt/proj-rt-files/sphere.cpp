#include "sphere.h"
#include "ray.h"

// Determine if the ray intersects with the sphere
Hit Sphere::Intersection(const Ray& ray, int part) const
{
    //TODO;
	vec3 v = ray.endpoint - center; //e - c
	vec3 u = ray.direction;
	
	double a = dot(u, u);	//a = u^2
	double b = 2*dot(v, u);	// b = 2(e - c)u 
	double c = dot(v, v) - (radius*radius); // c = (e - c)(e - c) - r^2
	
	double det = b*b - 4*a*c; //determinant
	double t0, t1;
	
	if (det > 0) {
		t0 = (-b + sqrt(det)) / (2*a);
		t1 = (-b - sqrt(det)) / (2*a);
		
		if (t0 < t1 && t0 >= small_t) {
			return {this, t0, part};
		}
		else if (t1 < t0 && t1 >= small_t) { //can this be else {...} ?
			return {this, t1, part};
		}
	}
	else if (det == 0) {
		t0 = -b / (2*a);
		t1 = 0;
		
		if (t0 >= small_t){
			return {this, t0, part};
		}
	}
	else { //det < 0
		return {nullptr, 0, part};
	}
	
	return {nullptr, 0, part};
}

vec3 Sphere::Normal(const vec3& point, int part) const
{
    vec3 normal;
    //TODO; // compute the normal direction
	
    normal = (point - center) / radius;

	
    return normal;
}

Box Sphere::Bounding_Box(int part) const
{
    Box box;
    TODO; // calculate bounding box
    return box;
}
