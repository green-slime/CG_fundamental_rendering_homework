#ifndef _PLANE_H_
#define _PLANE_H_

#include "vectors.h"
#include "object3d.h"

class Plane : public Object3D{
public:
    Plane(Vec3f &nor,float dis,Vec3f &col):Object3D(col){
        normal=nor;
        normal.Normalize();
        d=dis;
    }
    ~Plane(){}
    virtual bool intersect(const Ray &r, Hit &h, float tmin);
private:
    Vec3f normal;
    float d;
};

#endif