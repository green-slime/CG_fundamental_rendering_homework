#include "transform.h"
#include "matrix.h"

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
        h.set(h.getT(),h.getColor(),normal);
        return true;
    }
    return false;
}