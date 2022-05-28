#include "transform.h"
#include "matrix.h"
#include "vectors.h"

#include <GL/gl.h>
#include <GL/glut.h>

Transform::Transform(Matrix &mm, Object3D *oo){
    m=mm;
    o=oo;
    BoundingBox *b1 = oo->getBoundingBox();
    assert(b1 != NULL);
    Vec3f min = b1->getMin(), max = b1->getMax();
    Vec3f all[8];
    all[0] = min;
    all[1] = max;
    all[2] = Vec3f(min.x(), min.y(), max.z());
    all[3] = Vec3f(min.x(), max.y(), min.z());
    all[4] = Vec3f(min.x(), max.y(), max.z());
    all[5] = Vec3f(max.x(), min.y(), min.z());
    all[6] = Vec3f(max.x(), min.y(), max.z());
    all[7] = Vec3f(max.x(), max.y(), min.z());
    for (int i = 0; i < 8; i++){
        m.Transform(all[i]);        
    }
    min = all[0], max = all[1];
    for (int i = 0; i < 8; i++){
        min = Vec3f::minVec(min, all[i]);
        max = Vec3f::maxVec(max, all[i]);
    }
    BoundingBox *_box = new BoundingBox(min, max);
    setBoundingBox(_box);
}

bool Transform::intersect(const Ray &r, Hit &h, float tmin){
    Matrix m_i=m;
    m_i.Inverse(m_i);
    Vec3f R0=r.getOrigin(),Rd=r.getDirection();
    m_i.Transform(R0);
    m_i.TransformDirection(Rd);
    Ray ray(Rd,R0);
    // 接下来在物体坐标系OS下计算h
    if(o->intersect(ray,h,tmin)){
        Vec3f normal=h.getNormal();
        // direction未归一，t不变，normal进行变换
        m_i.Transpose(m_i);
        m_i.TransformDirection(normal);
        normal.Normalize();
        h.set(h.getT(),h.getMaterial(),normal,r);
        return true;
    }
    return false;
}

void Transform::paint(){
    glPushMatrix();
    GLfloat *glMatrix = m.glGet();
    glMultMatrixf(glMatrix);
    delete[] glMatrix;
    o->paint();
    glPopMatrix();
}

void Transform::insertIntoGrid(Grid *g,Matrix *mat){
    if(mat==NULL)
        o->insertIntoGrid(g, &m);
    else{
        Matrix matnew = (*mat) * m;
        o->insertIntoGrid(g, &matnew);
    }
}