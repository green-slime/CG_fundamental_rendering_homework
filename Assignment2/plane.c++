#include "plane.h"

float MIN=10e-8;

bool Plane::intersect(const Ray &r, Hit &h, float tmin){
    Vec3f R0=r.getOrigin(),Rd=r.getDirection();
    if(normal.Dot3(Rd)<MIN) return false;
    float t=-(d+normal.Dot3(R0))*1.0/normal.Dot3(Rd);
    if(t>tmin&&t<h.getT()){
        h.set(t,getcolor(),normal);
        return true;
    }
    return false;
}