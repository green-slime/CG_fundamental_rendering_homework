#include "sphere.h"
#include "vectors.h"
#include "ray.h"
#include "hit.h"
#include <math.h>
#include <assert.h>

bool Sphere::intersect(const Ray &r, Hit &h, float tmin){
    Vec3f R0=r.getOrigin()-center;
    float b=2*R0.Dot3(r.getDirection());
    float c=R0.Dot3(R0)-radius*radius;
    if(b*b-4*c<=0) return false;
    float d=sqrt(b*b-4*c),t;
    if(-b-d>0)  t=(-b-d)/2.0;
    else if(-b+d>0) t=(-b+d)/2.0;
    else return false;
    if(t<tmin) return false;
    //Vec3f Q=r.pointAtParameter(t);
    //Q.Normalize();
    if(t<h.getT()) h.set(t,getcolor());
    return true;
}