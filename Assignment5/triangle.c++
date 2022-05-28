#include "triangle.h"
#include "material.h"
#include "matrix.h"
#include "grid.h"
#include <GL/gl.h>
#include <GL/glut.h>

float Triangle::det(Vec3f x,Vec3f y,Vec3f z){
    // x y z 为列向量构成矩阵
    float xx=x.x(),xy=x.y(),xz=x.z();
    float yx=y.x(),yy=y.y(),yz=y.z();
    float zx=z.x(),zy=z.y(),zz=z.z();
    float det=xx*(yy*zz-yz*zy)-xy*(yx*zz-yz*zx)+xz*(yx*zy-yy*zx);
    return det;
}
bool Triangle::intersect(const Ray &r, Hit &h, float tmin){
    Vec3f R0=r.getOrigin(),Rd=r.getDirection();
    float detA=det(a-b,a-c,Rd);
    if(abs(detA)<=1e-8) return false;
    float beta=det(a-R0,a-c,Rd)*1.0/detA, gamma=det(a-b,a-R0,Rd)*1.0/detA;
    float t=det(a-b,a-c,a-R0)*1.0/detA; // PPT 公式给出的 t 的解
    if(beta+gamma<1&&beta>0&&gamma>0&&t>tmin&&t<h.getT()){
        Vec3f normal;
        normal.Cross3(normal,a-c,a-b);
        normal.Normalize();
        h.set(t,getMaterial(),normal,r);
        return true;
    }
    return false;

}

void Triangle::paint(){
    getMaterial()->glSetMaterial();
    glBegin(GL_TRIANGLES);
    Vec3f normal;
    normal.Cross3(normal, a - c, a - b);
    normal.Normalize();
    glNormal3f(normal.x(), normal.y(), normal.z());
    glVertex3f(a.x(), a.y(), a.z());
    glVertex3f(b.x(), b.y(), b.z());
    glVertex3f(c.x(), c.y(), c.z());
    glEnd();
}

void Triangle::insertIntoGrid(Grid *g, Matrix *m){
    if(m==NULL)
        this->Object3D::insertIntoGrid(g, m);
    else{
        Vec3f aa = a, bb = b, cc = c;
        m->Transform(aa);
        m->Transform(bb);
        m->Transform(cc);
        int nx = g->getx(), ny = g->gety(), nz = g->getz();
        Vec3f min = g->getBoundingBox()->getMin(), max = g->getBoundingBox()->getMax();
        float lx = (max.x() - min.x()) / nx, ly = (max.y() - min.y()) / ny, lz = (max.z() - min.z()) / nz;
        Vec3f trimin=Vec3f::minVec(aa,Vec3f::minVec(bb,cc)), trimax = Vec3f::maxVec(aa,Vec3f::maxVec(bb,cc));
        BoundingBox *newbox = new BoundingBox(trimin, trimax);
        setBoundingBox(newbox);
        for (int i = 0; i < nx; i++){
            for (int j = 0; j < ny; j++){
                for (int k = 0; k < nz; k++){
                    Vec3f voxelmin(i * lx, j * ly, k * lz);
                    voxelmin += min;
                    if(voxelmin.z()>trimax.z()||voxelmin.z()+lz<trimin.z())
                        continue;
                    if(voxelmin.y()>trimax.y()||voxelmin.y()+ly<trimin.y())
                        continue;
                    if(voxelmin.x()>trimax.x()||voxelmin.x()+lx<trimin.x())
                        continue;
                    g->setGrid(i, j, k, this);
                    
                }
            }
        }
    }
    
}