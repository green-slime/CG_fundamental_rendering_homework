#include "group.h"

Group::Group(int ind){
    index=ind;
    objs = new Object3D*[index];
    
}

void Group::addObject(int ind, Object3D *obj){
    objs[ind]=obj;
    if(obj->getBoundingBox()!=NULL){
        BoundingBox *box = getBoundingBox(),*objbox=obj->getBoundingBox();
        if(box==NULL)
            setBoundingBox(objbox);
        else{
            Vec3f min = Vec3f::minVec(box->getMin(), objbox->getMin()), max = Vec3f::maxVec(box->getMax(), objbox->getMax());
            BoundingBox *newbox = new BoundingBox(min, max);
            setBoundingBox(newbox);
            delete newbox;
        }    
    }    
}

bool Group::intersect(const Ray &r, Hit &h, float tmin){
    bool final=false;
    for(int i=0;i<index;i++){
        if(objs[i]->intersect(r,h,tmin)) final=true;
    }
    return final;
}

void Group::paint(){
    for (int i = 0; i < index; i ++){
        objs[i]->paint();
    }
}

void Group::updateBox(){
    BoundingBox *box = getBoundingBox();
    assert( box!= NULL);
    Vec3f min = box->getMin(), max = box->getMax();
    for (int i = 0; i < index; i++){
        if(objs[i]->getBoundingBox()!=NULL){
            min = Vec3f::minVec(min, objs[i]->getBoundingBox()->getMin());
            max = Vec3f::maxVec(max, objs[i]->getBoundingBox()->getMax());
        }        
    }
    BoundingBox *_box = new BoundingBox(min, max);
    setBoundingBox(_box);
    delete _box;
}


void Group::insertIntoGrid(Grid *g, Matrix *m){
    for (int i = 0; i < index;i++){
        objs[i]->insertIntoGrid(g, m);
    }
    updateBox();
}