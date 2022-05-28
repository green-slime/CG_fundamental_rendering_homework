#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "vectors.h"
#include "object3d.h"

class Triangle: public Object3D{
public:
    Triangle(Vec3f &aa,Vec3f &bb,Vec3f &cc,PhongMaterial* m):Object3D(m){
        a=aa;
        b=bb;
        c=cc;
    }
    ~Triangle(){}
    virtual bool intersect(const Ray &r, Hit &h, float tmin);
    static float det(Vec3f x,Vec3f y,Vec3f z);
    void paint();

private:
    Vec3f a,b,c;
};

#endif