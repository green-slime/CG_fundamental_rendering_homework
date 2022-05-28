#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "vectors.h"
#include "object3d.h"

class Sphere : public Object3D{

public:
    Sphere(Vec3f cen, float rad, PhongMaterial* m) : Object3D(m){  
        center=cen;
        radius=rad;
        BoundingBox *_box = new BoundingBox(center - Vec3f(1, 1, 1) * radius, center + Vec3f(1, 1, 1) * radius);
        setBoundingBox(_box);
    }
    ~Sphere();
    virtual bool intersect(const Ray &r, Hit &h, float tmin);
    void paint();
    virtual void insertIntoGrid(Grid *g, Matrix *m);

private:
    Vec3f center;
    float radius;
    // Vec3f color;
};

#endif