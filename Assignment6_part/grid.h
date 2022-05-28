#ifndef _GRID_H_
#define _GRID_H_

#include "object3d.h"
#include "marchinginfo.h"
#include "object3dvector.h"
#include <vector>

using namespace std;

class Grid: public Object3D{
public:
    Grid(BoundingBox *_bb, int _nx, int _ny, int _nz){
        nx = _nx;
        ny = _ny;
        nz = _nz;
        bb = new BoundingBox(_bb->getMin(),_bb->getMax());
        Object3DVector temp;
        grid.resize(nx);
        for (int i = 0; i < nx;i++){
            grid[i].resize(ny);
            for (int j = 0; j < ny;j++){
                grid[i][j].resize(nz);
                for (int k = 0; k < nz;k++)
                    grid[i][j][k] = temp;
            }
        }
    }
    ~Grid(){};
    int getx() { return nx; }
    int gety() { return ny; }
    int getz() { return nz; }
    BoundingBox *getBoundingBox() { return bb; }
    void setBox(BoundingBox *newbox) {
        delete bb;
        bb = new BoundingBox(newbox->getMin(), newbox->getMax()); 
    }
    virtual void paint();
    virtual bool intersect(const Ray &r, Hit &h, float tmin);
    virtual bool fastRayCast(const Ray &r, Hit &h, float tmin);
    void initializeRayMarch(MarchingInfo &mi,const Ray &r,float tmin) const;
    void setGrid(int i, int j, int k, Object3D* o) {
        assert(i>=0&&i<nx&&j>=0&&j<ny&&k>=0&&k<nz);
        grid[i][j][k].addObject(o);
    }
    int getGrid(int i, int j, int k) { return grid[i][j][k].getNumObjects(); }
    Object3D *getObj(int i, int j, int k, int ind) { 
        return grid[i][j][k].getObject(ind);
    }
    void insertinf(Object3D *o){
        infinite.addObject(o);
        infnum = infinite.getNumObjects();
    }

private:
    int nx, ny, nz, infnum=0;
    BoundingBox *bb;
    vector<vector<vector<Object3DVector>>> grid;
    Object3DVector infinite;
};

#endif