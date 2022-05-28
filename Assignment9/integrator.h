#ifndef _INTEGRATOR_H_
#define _INTEGRATOR_H_

#include "particle.h"
#include "forcefield.h"
#include "vectors.h"

class Integrator{
    public:
        Integrator(){}
        virtual void Update(Particle *particle, ForceField *forcefield, float t, float dt) = 0;
        virtual Vec3f getColor(){}
};

class EulerIntegrator:public Integrator{
    public:
        EulerIntegrator(){}
        virtual void Update(Particle *particle, ForceField *forcefield, float t, float dt);
        Vec3f getColor() { return Vec3f(1, 0, 1); }
};

class MidpointIntegrator:public Integrator{
    public:
        MidpointIntegrator(){}
        virtual void Update(Particle *particle, ForceField *forcefield, float t, float dt);
        Vec3f getColor() { return Vec3f(1, 1, 0); }
};

#endif