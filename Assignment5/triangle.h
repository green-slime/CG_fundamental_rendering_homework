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
        Vec3f min = Vec3f::minVec(aa, Vec3f::minVec(bb, cc));
        Vec3f max = Vec3f::maxVec(aa, Vec3f::maxVec(bb, cc));
        BoundingBox *_box = new BoundingBox(min, max);
        setBoundingBox(_box);
    }
    ~Triangle(){}
    virtual bool intersect(const Ray &r, Hit &h, float tmin);
    static float det(Vec3f x,Vec3f y,Vec3f z);
    void paint();
    virtual void insertIntoGrid(Grid *g, Matrix *m);

private:
    Vec3f a,b,c;
};

#endif