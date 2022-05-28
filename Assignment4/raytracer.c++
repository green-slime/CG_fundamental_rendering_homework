#include "raytracer.h"
#include "ray.h"
#include "hit.h"
#include "group.h"
#include "light.h"
#include "material.h"
#include "scene_parser.h"
#include "rayTree.h"

Vec3f RayTracer::mirrorDirection(const Vec3f &normal, const Vec3f &incoming){
    return incoming - normal * 2 * (incoming.Dot3(normal));
}

bool RayTracer::transmittedDirection(const Vec3f &normal, const Vec3f &incoming, float index_i, float index_t, Vec3f &transmitted){
    float ny = index_i / index_t;
    float squarepart = 1 - pow(ny, 2) * (1 - pow(normal.Dot3(incoming), 2));
    if(squarepart<0)
        return false; // 全反射
    else{
        transmitted = normal * (ny *(-1.0)* normal.Dot3(incoming) - sqrt(squarepart)) + incoming * ny;
        transmitted.Normalize();
        return true;
    }
}

Vec3f RayTracer::traceRay(Ray &ray, float tmin, int bounces, float weight, float indexOfRefraction, Hit &hit) const{   
    if(bounces<=max_bounces&&weight>cutoff_weight){
        Vec3f bg=s->getBackgroundColor();  
        Group *group=s->getGroup();
        int lightnum=s->getNumLights();
        float r = 10.0;
        float epsilon = 1e-5;
        Vec3f black(0, 0, 0);
        PhongMaterial cosmos(bg,black,1.0,black,black,1);
        const Hit hitori(INFINITY,&cosmos,black);

        Vec3f col;
        if(group->intersect(ray,hit,epsilon)){
            Vec3f p=ray.pointAtParameter(hit.getT());

            if(bounces==0) RayTree::SetMainSegment(ray, 0, hit.getT());

            Vec3f colp=hit.getMaterial()->getDiffuseColor();
            col=colp*s->getAmbientLight();
            for(int k=0;k<lightnum;k++){
                Vec3f dir,Lk;
                Light* light=s->getLight(k);           
                light->getIllumination(p,dir,Lk,r);
                     
                if(shadows){
                    Hit hit2(INFINITY,&cosmos,black);
                    Ray ray2(dir,p);
                    bool shad = group->intersect(ray2, hit2, epsilon);
                    RayTree::AddShadowSegment(ray2, 0, min(r,hit2.getT()));
                    if(shad&&r>hit2.getT()) // 有交的情况下,若灯在交点之后
                        continue;
                        // 跳过这盏灯的shade
                }
                col+=colp*Lk*max(0.0f,dir.Dot3(hit.getNormal()));              
                Vec3f specular = hit.getMaterial()->Shade(ray, hit, dir, Lk);  
                col += specular*max(0.0f,dir.Dot3(hit.getNormal()));;
                //col += specular;
            }
            Vec3f reflectcol = hit.getMaterial()->getReflectiveColor();
            if(reflectcol>Vec3f(0,0,0)){
                bounces++;
                weight *= reflectcol.Length();
                Ray reflect(mirrorDirection(hit.getNormal(), ray.getDirection()),p);
                if(bounces<=max_bounces&&weight>cutoff_weight){
                    Hit hittemp(INFINITY,&cosmos,black);
                    group->intersect(reflect, hittemp, epsilon);
                    RayTree::AddReflectedSegment(reflect, epsilon, hittemp.getT());
                }
                Hit hittmp = hitori;
                col += traceRay(reflect, epsilon, bounces, weight,indexOfRefraction, hittmp)*reflectcol;
            }
            Vec3f transcol = hit.getMaterial()->getTransparentColor();
            if(transcol>Vec3f(0,0,0)){
                if(hit.getNormal().Dot3(ray.getDirection())<0){
                    // 外射向内
                    float index = hit.getMaterial()->getIndex();
                    Vec3f transdir;
                    if(transmittedDirection(hit.getNormal(), ray.getDirection(), indexOfRefraction, index, transdir)){
                        Ray ray3(transdir, p);
                        Hit hittemp(INFINITY,&cosmos,black);
                        group->intersect(ray3, hittemp, epsilon);
                        RayTree::AddTransmittedSegment(ray3, epsilon, hittemp.getT());
                        hittemp = hitori;
                        col+=traceRay(ray3, epsilon, bounces, weight, index, hittemp)*transcol;
                    }
                    
                }
                else if(hit.getNormal().Dot3(ray.getDirection())>0){
                    // 内射向外
                    Vec3f transdir;
                    if(transmittedDirection(hit.getNormal()*(-1.0f), ray.getDirection(), indexOfRefraction, 1, transdir)){
                        Ray ray3(transdir, p);
                        Hit hittemp(INFINITY,&cosmos,black);
                        group->intersect(ray3, hittemp, epsilon);
                        RayTree::AddTransmittedSegment(ray3, 0, hittemp.getT());
                        hittemp = hitori;
                        col+=traceRay(ray3, epsilon, bounces, weight, 1, hittemp)*transcol;
                    }
                }
            }

        } // 不为背景
        else{
            col=bg*s->getAmbientLight();
            for(int k=0;k<lightnum;k++){
                Vec3f dir,Lk,p(1,1,1); // p whatever
                Light* light=s->getLight(k);
                light->getIllumination(p,dir,Lk,r);
                col+=bg*Lk;
            }
        }
        return col;
    }
    else
        return Vec3f(0, 0, 0);
}