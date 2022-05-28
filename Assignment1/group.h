#ifndef _GROUP_H_
#define _GROUP_H_

#include<object3d.h>

class Group: public Object3D{
public:
    Group(int ind){
        index=ind;
        objs = new Object3D*[index];
    }
    ~Group(){
        delete[] objs;
    }
    void addObject(int index, Object3D *obj);
    virtual bool intersect(const Ray &r, Hit &h, float tmin);

private:
    int index=0;
    Object3D** objs;
};

#endif