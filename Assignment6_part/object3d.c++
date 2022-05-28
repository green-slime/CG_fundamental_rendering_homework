#include "object3d.h"
#include "matrix.h"
#include "grid.h"
#include <vector>

void Object3D::insertIntoGrid(Grid *g,Matrix *m){

    // 考虑box与grid的交
    assert(box != NULL);

    if(m!=NULL){
        Vec3f min = box->getMin(), max = box->getMax();
        vector<Vec3f> V(8);
        V[0] = min, V[6] = max;
        V[1] = Vec3f(max.x(), min.y(), min.z());
        V[2] = Vec3f(max.x(), max.y(), min.z());
        V[3] = Vec3f(min.x(), max.y(), min.z());
        V[4] = Vec3f(min.x(), min.y(), max.z());
        V[5] = Vec3f(max.x(), min.y(), max.z());
        V[7] = Vec3f(min.x(), max.y(), max.z());
        for (int i = 0; i < 8;i++){
            m->Transform(V[i]);
        }
        min = V[0], max = V[0];
        for (int i = 1; i < 8;i++){
            min = Vec3f::minVec(min, V[i]);
            max = Vec3f::maxVec(max, V[i]);
        }
        delete box;
        box = new BoundingBox(min, max);
    }

    int nx = g->getx(), ny = g->gety(), nz = g->getz();
    Vec3f min = g->getBoundingBox()->getMin(), max = g->getBoundingBox()->getMax();
    float lx = (max.x() - min.x()) / nx, ly = (max.y() - min.y()) / ny, lz = (max.z() - min.z()) / nz;
    Vec3f objmin = this->getBoundingBox()->getMin(), objmax = this->getBoundingBox()->getMax();
    for (int i = 0; i < nx; i++){
        for (int j = 0; j < ny; j++){
            for (int k = 0; k < nz; k++){
                Vec3f voxelmin(i * lx, j * ly, k * lz);
                voxelmin += min;
                if(voxelmin.z()>objmax.z()||voxelmin.z()+lz<objmin.z())
                    continue;
                if(voxelmin.y()>objmax.y()||voxelmin.y()+ly<objmin.y())
                    continue;
                if(voxelmin.x()>objmax.x()||voxelmin.x()+lx<objmin.x())
                    continue;
                g->setGrid(i, j, k, this);
                
            }
        }
    }
}