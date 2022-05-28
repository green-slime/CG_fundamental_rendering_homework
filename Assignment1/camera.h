#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "ray.h"
#include "vectors.h"

class Camera{
public:
    Camera(){}
    ~Camera(){}
    virtual int getsize()=0;
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
    }
    ~OrthographicCamera();
    virtual int getsize(){return size;}
    virtual Ray generateRay(Vec2f point);

private:
    Vec3f center,direction,up,horizontal;
    int size;

};

#endif