#include "plane.h"
#include "ray.h"
#include <cfloat>
#include <limits>

// Intersect with the half space defined by the plane.  The plane's normal
// points outside.  If the ray starts on the "inside" side of the plane, be sure
// to record a hit with t=0 as the first entry in hits.
Hit Plane::Intersection(const Ray& ray, int part) const
{
    //TODO;
	double t;
	
	vec3 v = x1 - ray.endpoint;
	vec3 u = ray.direction;
	
	float denom = dot(u, normal);
	t = dot(v, normal) / denom;
	if (denom >= small_t) {
   		return {this, t, part};
		
        //Hit = {
		//const Object* object; // object that was intersected
		//double dist; // distance along ray to intersection location
		//int part; // which part was intersected (eg, for meshes)
		//}
	}
	else {
		return {nullptr, 0, part};
	}
	
//    return {0,0,0};
}

vec3 Plane::Normal(const vec3& point, int part) const
{
    return normal;
}

// There is not a good answer for the bounding box of an infinite object.
// The safe thing to do is to return a box that contains everything.
Box Plane::Bounding_Box(int part) const
{
    Box b;
    b.hi.fill(std::numeric_limits<double>::max());
    b.lo=-b.hi;
    return b;
}
