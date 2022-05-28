#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#include "vectors.h"
#include "ray.h"
#include "hit.h"
#include "material.h"
#include "matrix.h"
#include "boundingbox.h"

class Grid;

class Object3D{

public:

    Object3D(){
        //m = new Material(Vec3f(0,0,0));
        trans.SetToIdentity();
    }
    Object3D(PhongMaterial* mat){
        m = mat;
        trans.SetToIdentity();
    }
    ~Object3D() { delete box; }
    PhongMaterial* getMaterial(){return m;}
    BoundingBox *getBoundingBox() { return box; }
    void setBoundingBox(BoundingBox *_box) {  
        if(_box!=NULL) box = new BoundingBox(_box->getMin(),_box->getMax());
    }
    virtual bool intersect(const Ray &r, Hit &h, float tmin)=0;
    virtual void paint() = 0;
    virtual void insertIntoGrid(Grid *g, Matrix *m);
    virtual void setMat(Matrix m) { trans = m; }
    Matrix getMat() { return trans; }

private:
    PhongMaterial *m;
    BoundingBox *box=NULL;
    Matrix trans;
};

#endif