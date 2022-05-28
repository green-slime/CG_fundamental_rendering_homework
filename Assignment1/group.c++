#include "group.h"

void Group::addObject(int ind, Object3D *obj){
    objs[ind]=obj;
}

bool Group::intersect(const Ray &r, Hit &h, float tmin){
    bool final=false;
    for(int i=0;i<index;i++){
        if(objs[i]->intersect(r,h,tmin)) final=true;
    }
    return final;
}