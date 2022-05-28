#include "grid.h"
#include "rayTree.h"
#include "state.h"
#include <GL/gl.h>

void Grid::paint(){
    Vec3f min = bb->getMin(), max = bb->getMax();
    float lx = (max - min).x() / nx, ly = (max - min).y() / ny, lz = (max - min).z() / nz;
    for (int i = 0; i < nx; i++){
        for (int j = 0; j < ny; j++){
            for (int k = 0; k < nz; k++){
                if(this->getGrid(i,j,k)>=1){
                    Vec3f min2 = min + Vec3f(i * lx, j * ly, k * lz);
                    // 接下来画六个面
                    //printf("Grid(%d,%d,%d)=%d\n", i, j, k, this->getGrid(i, j, k));                    
                    switch(this->getGrid(i,j,k)){
                        case 1:
                            glColor3f(1, 1, 1);
                            break;
                        case 2:
                            glColor3f(1, 0, 1);
                            break;
                        case 3:
                            glColor3f(0, 0, 1);
                            break;
                        case 4:
                            glColor3f(0, 1, 1);
                            break;
                        case 5:
                            glColor3f(0, 1, 0);
                            break;
                        default:
                            glColor3f(1, 0, 0);

                    }  
                    glBegin(GL_QUADS);                 
                    glNormal3f(0, -1, 0);
                    glVertex3f(min2.x(), min2.y(), min2.z());
                    glVertex3f(min2.x()+lx, min2.y(), min2.z());
                    glVertex3f(min2.x()+lx, min2.y(), min2.z()+lz);
                    glVertex3f(min2.x(), min2.y(), min2.z()+lz);

                    glNormal3f(1, 0, 0);
                    glVertex3f(min2.x()+lx, min2.y(), min2.z());
                    glVertex3f(min2.x()+lx, min2.y()+ly, min2.z());
                    glVertex3f(min2.x()+lx, min2.y()+ly, min2.z()+lz);
                    glVertex3f(min2.x()+lx, min2.y(), min2.z()+lz);

                    glNormal3f(0, 0, 1);
                    glVertex3f(min2.x(), min2.y(), min2.z()+lz);
                    glVertex3f(min2.x()+lx, min2.y(), min2.z()+lz);
                    glVertex3f(min2.x()+lx, min2.y()+ly, min2.z()+lz);
                    glVertex3f(min2.x(), min2.y()+ly, min2.z()+lz);

                    glNormal3f(-1, 0, 0);
                    glVertex3f(min2.x(), min2.y(), min2.z());
                    glVertex3f(min2.x(), min2.y(), min2.z()+lz);
                    glVertex3f(min2.x(), min2.y()+ly, min2.z()+lz);
                    glVertex3f(min2.x(), min2.y()+ly, min2.z());

                    glNormal3f(0, 1, 0);
                    glVertex3f(min2.x(), min2.y()+ly, min2.z());
                    glVertex3f(min2.x(), min2.y()+ly, min2.z()+lz);
                    glVertex3f(min2.x()+lx, min2.y()+ly, min2.z()+lz);
                    glVertex3f(min2.x()+lx, min2.y()+ly, min2.z());

                    glNormal3f(0, 0, -1);
                    glVertex3f(min2.x(), min2.y(), min2.z());
                    glVertex3f(min2.x()+lx, min2.y(), min2.z());
                    glVertex3f(min2.x()+lx, min2.y()+ly, min2.z());
                    glVertex3f(min2.x(), min2.y()+ly, min2.z());

                    glEnd();
                }
            }
        }
    }
}

void Grid::initializeRayMarch(MarchingInfo &mi,const Ray &r,float tmin) const{
    Vec3f ori = r.getOrigin(), dir = r.getDirection();
    Vec3f min = bb->getMin(), max = bb->getMax();
    int signx, signy, signz, i, j, k;
    float tnextx, tnexty, tnextz, dtx, dty, dtz, tnear, tfar;
    if(dir.x()>0)
        signx = 1;
    else
        signx = -1;
    if(dir.y()>0)
        signy = 1;
    else
        signy = -1;
    if(dir.z()>0)
        signz = 1;
    else
        signz = -1;
    float gridx = (max - min).x() / nx, gridy = (max - min).y() / ny, gridz = (max - min).z() / nz;
    dtx = abs(gridx / dir.x()), dty = abs(gridy / dir.y()), dtz = abs(gridz / dir.z());
    mi.setD(dtx, dty, dtz);
    mi.setSign(signx, signy, signz);

    //-------- ray-box intersection ---------
    Vec3f normal(-signx,0,0);
    tnear = (min.x() - ori.x()) / dir.x();
    tfar = (max.x() - ori.x()) / dir.x();
    if(tnear>tfar)
        swap(tnear, tfar);
    float tymin = (min.y() - ori.y()) / dir.y();
    float tymax = (max.y() - ori.y()) / dir.y();
    if(tymin>tymax)
        swap(tymin, tymax);
    if((tnear>tymax)||(tymin>tfar))
        mi.setIntersect(false);
    if(tymin>tnear){
        tnear = tymin;
        normal = Vec3f(0, -signy, 0); // 外部射入光线在表面的法向量
    }
    if(tymax<tfar)
        tfar = tymax;
    float tzmin = (min.z() - ori.z()) / dir.z();
    float tzmax = (max.z() - ori.z()) / dir.z();
    if(tzmin>tzmax)
        swap(tzmin, tzmax);
    if((tnear>tzmax)||(tzmin>tfar))
        mi.setIntersect(false);
    if(tzmin>tnear){
        tnear = tzmin;
        normal = Vec3f(0, 0, -signz);
    }
    if(tzmax<tfar)
        tfar = tzmax;
    // 全部赋值完毕

    if(tfar<tmin) // 盒子在眼睛后面
        mi.setIntersect(false);
    else if(tnear>tmin){ // 盒子在眼睛前
        mi.setT(tnear, tfar);
        mi.setNormal(normal);
    }
    else{ // 眼睛在盒子内
        tnear = 0;
        mi.setT(tnear, tfar);       
    }
    //---------------------------------------
    // 接下来确定其它初始量
    if(mi.getIntersect()){
        // 若有交
        float epsilon = 1e-8;
        Vec3f offset = r.pointAtParameter(tnear) - min;
        i = floor(offset.x() / gridx), j = floor(offset.y() / gridy), k = floor(offset.z() / gridz);
        if(i==nx)
            i -= 1;
        if(j==ny)
            j -= 1;
        if(k==nz)
            k -= 1;
        mi.setPos(i, j, k);
        int d_next_x = (signx == 1) ? ceil(offset.x() / gridx) : floor(offset.x() / gridx);
        int d_next_y = (signy == 1) ? ceil(offset.y() / gridy) : floor(offset.y() / gridy);
        int d_next_z = (signz == 1) ? ceil(offset.z() / gridz) : floor(offset.z() / gridz);
        tnextx = abs((d_next_x * gridx - offset.x()) / dir.x());
        if(tnextx<1e-8)
            tnextx = dtx;
        tnexty = abs((d_next_y * gridy - offset.y()) / dir.y());
        if(tnexty<1e-8)
            tnexty = dty;
        tnextz = abs((d_next_z * gridz - offset.z()) / dir.z());
        if(tnextz<1e-8)
            tnextz = dtz;
        mi.setNext(tnextx + tnear, tnexty + tnear, tnextz + tnear);
    }
}

bool Grid::intersect(const Ray &r, Hit &h, float tmin){
    MarchingInfo mi;
    initializeRayMarch(mi, r, tmin);
    if(mi.getIntersect()==0)
        return false;
    else{
        Vec3f max = bb->getMax(), min = bb->getMin();
        float gridx = (max - min).x() / nx, gridy = (max - min).y() / ny, gridz = (max - min).z() / nz;
        Vec3f x(gridx, 0, 0), y(0, gridy, 0), z(0, 0, gridz);   
        int i = mi.geti(), j = mi.getj(), k = mi.getk();         
        while(mi.getNear()<mi.getFar()&&i<nx&&i>=0&&j<ny&&j>=0&&k<nz&&k>=0){
                       
            Vec3f minvox(i * gridx, j * gridy, k * gridz);
            minvox += min;

            PhongMaterial *m = &white;
            RayTree::AddHitCellFace(minvox, minvox + x, minvox + x + z, minvox + z, Vec3f(0, -1, 0), m);
            RayTree::AddHitCellFace(minvox + x, minvox + x + y, minvox + x + y + z, minvox + x + z, Vec3f(1, 0, 0), m);
            RayTree::AddHitCellFace(minvox + y, minvox + x + y, minvox + x + y + z, minvox + y + z, Vec3f(0, 1, 0), m);
            RayTree::AddHitCellFace(minvox, minvox + y, minvox + z + y, minvox + z, Vec3f(-1, 0, 0), m);
            RayTree::AddHitCellFace(minvox + z, minvox + x + z, minvox + x + y + z, minvox + y + z, Vec3f(0, 0, 1), m);
            RayTree::AddHitCellFace(minvox, minvox + x, minvox + x + y, minvox + y, Vec3f(0, 0, -1), m);

            if(this->getGrid(i,j,k)>=1){
                PhongMaterial* voxmat;
                switch(this->getGrid(i,j,k)){
                    case 1:
                        voxmat = &white;
                        break;
                    case 2:
                        voxmat = &purple;
                        break;
                    default:
                        voxmat = &red;
                }
                h.set(mi.getNear(), voxmat, mi.getNormal(), r);
                return true;
            }

            mi.nextCell();
            i = mi.geti(), j = mi.getj(), k = mi.getk();

            Vec3f normal = mi.getNormal();
            minvox = Vec3f(i * gridx, j * gridy, k * gridz) + min;
            if(normal==Vec3f(0,-1,0))
                RayTree::AddEnteredFace(minvox, minvox + x, minvox + x + z, minvox + z, Vec3f(0, -1, 0), m);
            else if(normal==Vec3f(1,0,0))
                RayTree::AddEnteredFace(minvox + x, minvox + x + y, minvox + x + y + z, minvox + x + z, Vec3f(1, 0, 0), m);
            else if(normal==Vec3f(0,1,0))
                RayTree::AddEnteredFace(minvox + y, minvox + x + y, minvox + x + y + z, minvox + y + z, Vec3f(0, 1, 0), m);
            else if(normal==Vec3f(-1,0,0))
                RayTree::AddEnteredFace(minvox, minvox + y, minvox + z + y, minvox + z, Vec3f(-1, 0, 0), m);
            else if(normal==Vec3f(0,0,1))
                RayTree::AddEnteredFace(minvox + z, minvox + x + z, minvox + x + y + z, minvox + y + z, Vec3f(0, 0, 1), m);
            else
                RayTree::AddEnteredFace(minvox, minvox + x, minvox + x + y, minvox + y, Vec3f(0, 0, -1), m);
        }
        return false;
    }
}
