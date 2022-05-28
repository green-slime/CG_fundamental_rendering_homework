#define M_PI 3.141592654
#include <math.h>
#ifndef _GENERATOR_H_
#define _GENERATOR_H_

#include "vectors.h"
#include "particle.h"
#include "random.h"

#include <GL/gl.h>
#include <GL/glut.h>

class Generator{
    public:
        Generator(){}
        void SetColors(Vec3f _color, Vec3f _dead_color, float _color_randomness){
            color = _color;
            dead_color = _dead_color;
            color_randomness = _color_randomness;
        }
        void SetLifespan(float _lifespan, float _lifespan_randomness, int _desired_num_particles){
            lifespan = _lifespan;
            lifespan_randomness = _lifespan_randomness;
            desired_num_particles = _desired_num_particles;
        }
        void SetMass(float _mass, float _mass_randomness){
            mass = _mass;
            mass_randomness = _mass_randomness;
        }

        Vec3f getColor() { return color + rand.randomVector() * color_randomness; }
        Vec3f getDeadColor() { return dead_color; }
        float getLifespan() { return lifespan + lifespan_randomness*rand.next(); }
        float getMass() { return mass + mass_randomness*rand.next(); }

        virtual int numNewParticles(float current_time, float dt) const{
            return floor(desired_num_particles * dt / lifespan);
        }
        virtual Particle *Generate(float current_time, int i) = 0;

        virtual void Paint() const {}
        void Restart(){
            rand.reseed(0);
        }
    private:
        Vec3f color, dead_color;
        float lifespan, lifespan_randomness, mass, mass_randomness, color_randomness;
        int desired_num_particles;
        Random rand;
};

class HoseGenerator:public Generator{
    public: 
        HoseGenerator(Vec3f _position, float _position_randomness, Vec3f _velocity, float _velocity_randomness):Generator(){
            position = _position;
            position_randomness = _position_randomness;
            velocity = _velocity;
            velocity_randomness = _velocity_randomness;
        }
        virtual Particle* Generate(float current_time, int i){
            Vec3f pos = position + rand.randomVector() * position_randomness;
            Vec3f v = velocity + rand.randomVector() * velocity_randomness;
            Particle* p = new Particle(pos, v, getColor(), getDeadColor(), getMass(), getLifespan());
            return p;
        }
    private:
        Vec3f position, velocity;
        float position_randomness, velocity_randomness;
        Random rand;
};

class RingGenerator:public Generator{
    public: 
        RingGenerator(float _position_randomness, Vec3f _velocity, float _velocity_randomness):Generator(){
            position_randomness = _position_randomness;
            velocity = _velocity;
            velocity_randomness = _velocity_randomness;
        }
        virtual int numNewParticles(float current_time, float dt){
            int num = this->Generator::numNewParticles(current_time, dt);
            num *= (1 + current_time) * (1 + current_time);
            return num;
        }

        virtual Particle* Generate(float current_time, int i){
            float theta = 2 * M_PI * rand.next();
            Vec3f pos = Vec3f((1 + current_time) * cos(theta), 0, (1 + current_time) * sin(theta)) + rand.randomVector() * position_randomness;
            Vec3f v = velocity + rand.randomVector() * velocity_randomness;
            Particle* p = new Particle(pos, v, getColor(), getDeadColor(), getMass(), getLifespan());
            return p;
        }

        virtual void Paint() const{
            glBegin(GL_QUADS);
            glVertex3f(-20, -4, -20);
            glVertex3f(-20, -4, 20);
            glVertex3f(20, -4, 20);
            glVertex3f(20, -4, -20);
            glEnd();
        }
    private:
        Vec3f velocity;
        float position_randomness, velocity_randomness;
        Random rand;
};

#endif