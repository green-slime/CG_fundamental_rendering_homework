#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "vectors.h"
#include "matrix.h"
#include "object3d.h"

class Transform : public Object3D{
public:
    Transform(Matrix &mm, Object3D *oo);
    ~Transform(){o=NULL;}
    virtual bool intersect(const Ray &r, Hit &h, float tmin);
    virtual void paint();
    virtual void insertIntoGrid(Grid *g, Matrix *m);

private:
    Matrix m;
    Object3D* o;
};

#endif