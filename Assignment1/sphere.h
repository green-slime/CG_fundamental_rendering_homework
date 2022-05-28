#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "vectors.h"
#include "object3d.h"

class Sphere : public Object3D{

public:
    Sphere(Vec3f cen, float rad, Vec3f col) : Object3D(col){  
        center=cen;
        radius=rad;
    }
    ~Sphere();
    virtual bool intersect(const Ray &r, Hit &h, float tmin);

private:
    Vec3f center;
    float radius;
    // Vec3f color;
};

#endif