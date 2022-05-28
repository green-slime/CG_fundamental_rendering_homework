#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "vectors.h"
#include "object3d.h"

class Triangle: public Object3D{
public:
    Triangle(Vec3f &aa,Vec3f &bb,Vec3f &cc,Vec3f &color):Object3D(color){
        a=aa;
        b=bb;
        c=cc;
    }
    ~Triangle(){}
    virtual bool intersect(const Ray &r, Hit &h, float tmin);
    static float det(Vec3f x,Vec3f y,Vec3f z);
private:
    Vec3f a,b,c;
};

#endif