#define _USE_MATH_DEFINES
#include "sphere.h"
#include "vectors.h"
#include "ray.h"
#include "hit.h"
#include "state.h"
#include "grid.h"

#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <GL/gl.h>

bool Sphere::intersect(const Ray &r, Hit &h, float tmin){
    Vec3f R0=r.getOrigin()-center;
    float a=r.getDirection().Dot3(r.getDirection());
    float b=2*R0.Dot3(r.getDirection());
    float c=R0.Dot3(R0)-radius*radius;
    if(b*b-4*a*c<0) return false;
    float d=sqrt(b*b-4*a*c),t;
    if(-b-d>1e-5)  t=(-b-d)/2.0/a;
    else if(-b+d>1e-5) t=(-b+d)/2.0/a;
    else return false;
    if(t<tmin) return false;
    if(t<h.getT()){
        Vec3f Q=r.pointAtParameter(t)-center;
        Q.Normalize();
        h.set(t,getMaterial(),Q,r);
        if(stats==1) raystats.IncrementNumIntersections();
        return true;
    }
    return false;
}

void Sphere::paint(){
    getMaterial()->glSetMaterial();
    if(gouraud==0){
        glBegin(GL_QUADS);
        double dphi = M_PI / phi_steps, dthe = 2 * M_PI / theta_steps;
        for (double iPhi = -M_PI / 2.0; iPhi < M_PI / 2.0;iPhi+=dphi){
            for (double iTheta = 0; iTheta < 2 * M_PI;iTheta+=dthe){       
                glNormal3f(cos(iPhi+dphi/2) * cos(iTheta+dthe/2), cos(iPhi+dphi/2) * sin(iTheta+dthe/2), sin(iPhi+dphi/2));
                double x = center.x(), y = center.y(), z = center.z(), r=radius;
                glVertex3f(x + r * cos(iPhi) * cos(iTheta), y + r * cos(iPhi) * sin(iTheta), z + r * sin(iPhi));
                glVertex3f(x + r * cos(iPhi+dphi) * cos(iTheta), y + r * cos(iPhi+dphi) * sin(iTheta), z + r * sin(iPhi+dphi));
                glVertex3f(x + r * cos(iPhi+dphi) * cos(iTheta+dthe), y + r * cos(iPhi+dphi) * sin(iTheta+dthe), z + r * sin(iPhi+dphi));
                glVertex3f(x + r * cos(iPhi) * cos(iTheta+dthe), y + r * cos(iPhi) * sin(iTheta+dthe), z + r * sin(iPhi));
            }
        }
        glEnd();
    }
    else{
        glBegin(GL_QUADS);
        double dphi = M_PI / phi_steps, dthe = 2 * M_PI / theta_steps;
        for (double iPhi = -M_PI / 2.0; iPhi < M_PI / 2.0;iPhi+=dphi){
            for (double iTheta = 0; iTheta < 2 * M_PI;iTheta+=dthe){
                glNormal3f(cos(iPhi) * cos(iTheta), cos(iPhi) * sin(iTheta), sin(iPhi));
                double x = center.x(), y = center.y(), z = center.z(), r=radius;
                glVertex3f(x + r * cos(iPhi) * cos(iTheta), y + r * cos(iPhi) * sin(iTheta), z + r * sin(iPhi));
                glNormal3f(cos(iPhi+dphi) * cos(iTheta), cos(iPhi+dphi) * sin(iTheta), sin(iPhi+dphi));
                glVertex3f(x + r * cos(iPhi+dphi) * cos(iTheta), y + r * cos(iPhi+dphi) * sin(iTheta), z + r * sin(iPhi+dphi)); 
                glNormal3f(cos(iPhi+dphi) * cos(iTheta+dthe), cos(iPhi+dphi) * sin(iTheta+dthe), sin(iPhi+dphi));
                glVertex3f(x + r * cos(iPhi+dphi) * cos(iTheta+dthe), y + r * cos(iPhi+dphi) * sin(iTheta+dthe), z + r * sin(iPhi+dphi));
                glNormal3f(cos(iPhi) * cos(iTheta+dthe), cos(iPhi) * sin(iTheta+dthe), sin(iPhi));
                glVertex3f(x + r * cos(iPhi) * cos(iTheta+dthe), y + r * cos(iPhi) * sin(iTheta+dthe), z + r * sin(iPhi));
            }
        }
        glEnd();
    }
}

void Sphere::insertIntoGrid(Grid *g, Matrix *m){
    if(m==NULL){
        int nx = g->getx(), ny = g->gety(), nz = g->getz();
        Vec3f min = g->getBoundingBox()->getMin(), max = g->getBoundingBox()->getMax();
        float lx = (max.x() - min.x()) / nx, ly = (max.y() - min.y()) / ny, lz = (max.z() - min.z()) / nz;
        for (int i = 0; i < nx; i++){
            for (int j = 0; j < ny; j++){
                for (int k = 0; k < nz; k++){
                    Vec3f point = min + Vec3f((i + 0.5) * lx, (j + 0.5) * ly, (k + 0.5) * lz);
                    if((point-center).Length()<radius+sqrt(lx*lx+ly*ly+lz*lz)/2){
                        g->setGrid(i, j, k, this);
                    }           
                        
                }
            }
        }
    }
    else{
        this->Object3D::insertIntoGrid(g, m);
    }
}