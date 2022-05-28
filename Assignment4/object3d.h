#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#include "vectors.h"
#include "ray.h"
#include "hit.h"
#include "material.h"

class Object3D{

public:

    Object3D(){
        //m = new Material(Vec3f(0,0,0));
    }
    Object3D(PhongMaterial* mat){
        m = mat;
    }
    Object3D(const Object3D &obj){
        m=obj.m;
    }
    ~Object3D() {}
    PhongMaterial* getMaterial(){return m;}
    virtual bool intersect(const Ray &r, Hit &h, float tmin)=0;
    virtual void paint() = 0;

private:
    PhongMaterial *m;
};

#endif