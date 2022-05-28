#ifndef _RAYTRACER_H_
#define _RAYTRACER_H_

#include "vectors.h"

class SceneParser;
class Ray;
class Hit;

class RayTracer{
    public:
        RayTracer(SceneParser *_s, int _max_bounces, float _cutoff_weight, bool _shadows){
            max_bounces = _max_bounces;
            cutoff_weight = _cutoff_weight;
            s = _s;
            shadows = _shadows;
        }
        ~RayTracer(){}
        Vec3f traceRay(Ray &ray, float tmin, int bounces, float weight, float indexOfRefraction, Hit &hit) const;
        static Vec3f mirrorDirection(const Vec3f &normal, const Vec3f &incoming);
        static bool transmittedDirection(const Vec3f &normal, const Vec3f &incoming, float index_i, float index_t, Vec3f &transmitted);

    private:
        int max_bounces;
        float cutoff_weight;
        SceneParser *s;
        bool shadows;
};

#endif