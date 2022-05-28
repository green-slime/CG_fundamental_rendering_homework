#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "ray.h"
#include "vectors.h"
#include <math.h>

class Camera{
public:
    Camera(){}
    ~Camera(){}
    virtual float getsize()=0;
    virtual Ray generateRay(Vec2f point)=0;
};


class OrthographicCamera : public Camera{
public:
    OrthographicCamera(Vec3f cen,Vec3f dir,Vec3f upp,int sizee){
        center=cen;
        direction=dir;
        direction.Normalize();
        up=upp;
        up-=direction*(up.Dot3(direction));
        up.Normalize();
        size=sizee;
        horizontal.Cross3(horizontal,direction,up); // 模长必为1
        direction=dir; // 不作单位化
    }
    ~OrthographicCamera();
    virtual float getsize(){return size;}
    virtual Ray generateRay(Vec2f point);

private:
    Vec3f center,direction,up,horizontal;
    int size;

};

class PerspectiveCamera: public Camera{
public:
    PerspectiveCamera(Vec3f &cen,Vec3f &dir,Vec3f &upp, float ang){
        center=cen;
        direction=dir;
        direction.Normalize();
        up=upp;
        up-=direction*(up.Dot3(direction));
        up.Normalize();
        angle=ang;
        horizontal.Cross3(horizontal,direction,up); // 模长必为1
        direction=dir; // 不作单位化
        float l=direction.Length(); //设center到视平面距离为direction模长
        imgsize=l*sin(angle/2.0)/sqrt(pow(cos(angle/2.0),2)/2.0-0.25);
        // 反解出视平面大小
    }
    ~PerspectiveCamera(){}
    virtual Ray generateRay(Vec2f point);
    virtual float getsize(){return imgsize;}
private:
    Vec3f center,direction,up,horizontal;
    float angle,imgsize;
};

#endif