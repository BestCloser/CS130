#include "sphere.h"
#include "ray.h"

// Determine if the ray intersects with the sphere
Hit Sphere::Intersection(const Ray& ray, int part) const
{
    //TODO;
	vec3 v = ray.endpoint - center; //e - c
	vec3 u = ray.direction;
	Hit h = {0, 0, 0};
	
	double a = dot(u, u);	//(e - c)
	double b = 2*dot(v, u);	// 2(e - c)u 
	double c = dot(v, v) - pow(radius, 2); // (x - c) dot (x - c) = r^2
	
	double det = pow(b, 2) - 4*a*c; //determinant
	double t0 = 0;
	double t1 = 0;
	
	if (det > 0) {
		t0 = (-b + sqrt(det)) / (2*a);
		t1 = (-b - sqrt(det)) / (2*a);
	}
	else if (det == 0) {
		t0 = -b / (2*a);
		t1 = 0;
	}
	else { //det < 0
		t0 = 0;
		t1 = 0;
	}
	
	if (t0 < small_t) {
		t0 = 0;
	}
	if (t1 < small_t) {
		t1 = 0;
	}
	
	if (t1 == 0) {
		if (t0 == 0) {
			h.object = nullptr;
			h.dist = -1;
			h.part = part;
		}
		else {
			h.object = this;
			h.dist = t0;
			h.part = part;
		}
	}
	else { //t1 != 0 //2 intersections, FIXME
		if (t1 < t0) {
			h.object = this;
			h.dist = t1;
			h.part = part;
		}
		else { //t0 < t1
			h.object = this;
			h.dist = t0;
			h.part = part;
		}
	}
	return h;
    //return {0,0,0};
}

vec3 Sphere::Normal(const vec3& point, int part) const
{
    vec3 normal;
    TODO; // compute the normal direction
    return normal;
}

Box Sphere::Bounding_Box(int part) const
{
    Box box;
    TODO; // calculate bounding box
    return box;
}
