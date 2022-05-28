#include "integrator.h"

void EulerIntegrator::Update(Particle *particle, ForceField *forcefield, float t, float dt){
    Vec3f pos = particle->getPosition(), v = particle->getVelocity();
    particle->setPosition(pos + v * dt);
    particle->setVelocity(v + forcefield->getAcceleration(pos, particle->getMass(), t)*dt);
    particle->increaseAge(dt);
}

void MidpointIntegrator::Update(Particle *particle, ForceField *forcefield, float t, float dt){
    Vec3f pos = particle->getPosition(), v = particle->getVelocity();
    float m = particle->getMass();
    Vec3f pm = pos + v * dt * 0.5, vm = v + forcefield->getAcceleration(pos, m, t) * dt * 0.5;
    pos = pos + vm * dt;
    v = v + forcefield->getAcceleration(pm, m, t + dt / 2)*dt;
    particle->setPosition(pos), particle->setVelocity(v);
    particle->increaseAge(dt);
}