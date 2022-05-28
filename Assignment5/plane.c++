#include "plane.h"
#include <GL/gl.h>
#include <GL/glut.h>

float MIN=10e-8;

bool Plane::intersect(const Ray &r, Hit &h, float tmin){
    Vec3f R0=r.getOrigin(),Rd=r.getDirection();
    if(abs(normal.Dot3(Rd))<MIN) return false;
    //float t=-(d+normal.Dot3(R0))*1.0/normal.Dot3(Rd);
    float t=(d-normal.Dot3(R0))*1.0/normal.Dot3(Rd);
    if(t>tmin&&t<h.getT()){
        h.set(t,getMaterial(),normal,r);
        return true;
    }
    return false;
}

void Plane::paint(){
    Vec3f v(1, 0, 0);
    if((normal-v).Length()<1e-5){
        v = Vec3f(0, 1, 0);
    }
    Vec3f b1,b2;
    b1.Cross3(b1, v, normal);
    b1.Normalize();
    b2.Cross3(b2, normal, b1); // 模长为1
    float big = 20.0;
    Vec3f cen = normal * d;
    Vec3f zero(0, 0, 0);
    Vec3f ru = cen + (b1 + b2) * big, rd = cen + (b1 - b2) * big, lu = cen + (b2 - b1) * big, ld = cen + (zero- b1 - b2) * big;
    getMaterial()->glSetMaterial();
    glBegin(GL_QUADS);
    glNormal3f(normal.x(), normal.y(), normal.z());
    glVertex3f(ru.x(), ru.y(), ru.z());
    glVertex3f(rd.x(), rd.y(), rd.z());  
    glVertex3f(ld.x(), ld.y(), ld.z());
    glVertex3f(lu.x(), lu.y(), lu.z());
    glEnd();
}