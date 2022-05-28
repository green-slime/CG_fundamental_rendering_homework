#ifndef _PLANE_H_
#define _PLANE_H_

#include "vectors.h"
#include "object3d.h"

class Plane : public Object3D{
public:
    Plane(Vec3f &nor,float dis,PhongMaterial* m):Object3D(m){
        normal=nor;
        normal.Normalize();
        d=dis;
        setBoundingBox(NULL);
    }
    ~Plane(){}
    virtual bool intersect(const Ray &r, Hit &h, float tmin);
    void paint();

private:
    Vec3f normal;
    float d;
};

#endif