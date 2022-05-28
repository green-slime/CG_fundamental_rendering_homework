#include "camera.h"
#include <assert.h>

Ray OrthographicCamera::generateRay(Vec2f point){  
    float x=point.x(),y=point.y();
    assert(x>=0&&x<size&&y>=0&&y<size);
    Vec3f ori=center-up*(size/2.0)-horizontal*(size/2.0)+up*y+horizontal*x;
    Ray ray(direction,ori);
    return ray;
}

Ray PerspectiveCamera::generateRay(Vec2f point){
    float x=point.x(),y=point.y();
    assert(x>=0&&x<imgsize&&y>=0&&y<imgsize);
    Vec3f ori=center-up*(imgsize/2.0)-horizontal*(imgsize/2.0)+up*y+horizontal*x+direction;
    Vec3f dir=ori-center; // 不作单位化
    Ray ray(dir,ori);
    return ray;
}