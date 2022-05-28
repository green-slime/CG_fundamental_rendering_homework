#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#include "vectors.h"
#include "ray.h"
#include "hit.h"

class Object3D{

public:

    Object3D(){
        color=Vec3f(0,0,0);
    }
    Object3D(Vec3f col){
        color=col;
    }
    Object3D(const Object3D &obj){
        color=obj.color;
    }
    ~Object3D(){}
    Vec3f getcolor(){return color;}
    virtual bool intersect(const Ray &r, Hit &h, float tmin)=0;

private:

    Vec3f color;

};

#endif