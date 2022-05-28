#include "triangle.h"

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
    if(abs(detA)<=10e-8) return false;
    float beta=det(a-R0,a-c,Rd)*1.0/detA, gamma=det(a-b,a-R0,Rd)*1.0/detA;
    float t=det(a-b,a-c,a-R0)*1.0/detA; // PPT 公式给出的 t 的解
    if(beta+gamma<1&&beta>0&&gamma>0&&t>tmin&&t<h.getT()){
        Vec3f normal;
        normal.Cross3(normal,a-b,a-c);
        normal.Normalize();
        h.set(t,getcolor(),normal);
        return true;
    }
    return false;

}