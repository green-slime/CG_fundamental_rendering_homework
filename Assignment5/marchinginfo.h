#ifndef _MARCHINGINFO_H_
#define _MARCHINGINFO_H_

#include "vectors.h"

class MarchingInfo{
    public:
        MarchingInfo(){}
        ~MarchingInfo(){}
        void setNext(float x,float y,float z){
            t_next_x = x;
            t_next_y = y;
            t_next_z = z;
        }
        Vec3f getNext() { return Vec3f(t_next_x, t_next_y, t_next_z); }
        void setSign(int x,int y,int z){
            sign_x = x;
            sign_y = y;
            sign_z = z;
        }
        void setD(float x,float y,float z){
            d_tx = x;
            d_ty = y;
            d_tz = z;
        }
        Vec3f getD() { return Vec3f(d_tx, d_ty, d_tz); }
        void setPos(int _i,int _j,int _k){
            i = _i;
            j = _j;
            k = _k;
        }
        void seti(int _i) { i = _i; }
        int geti() { return i; }
        int getj() { return j; }
        int getk() { return k; }
        void setT(float _tnear, float _tfar) { tnear = _tnear, tfar= _tfar; }
        float getNear() { return tnear; }
        float getFar() { return tfar; }
        void setIntersect(bool flag) { intersect = flag; }
        bool getIntersect() { return intersect; }
        void nextCell();
        void setNormal(Vec3f n) { normal = n; }
        Vec3f getNormal() { return normal; }

    private:
        float t_next_x, t_next_y, t_next_z, d_tx, d_ty, d_tz;
        int i, j, k, sign_x, sign_y, sign_z;
        float tnear,tfar;
        Vec3f normal;
        bool intersect=true;
};

#endif