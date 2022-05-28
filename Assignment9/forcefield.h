#ifndef _FORCEFIELD_H_
#define _FORCEFIELD_H_

#include "vectors.h"

class ForceField{
    public:
        ForceField(){}
        virtual Vec3f getAcceleration(const Vec3f &position, float mass, float t) const = 0;
};

class GravityForceField: public ForceField{
    public:
        GravityForceField(Vec3f _gravity){
            gravity = _gravity;
        }
        virtual Vec3f getAcceleration(const Vec3f &position, float mass, float t) const{
            return gravity;
        }

    private:
        Vec3f gravity;
};

class ConstantForceField: public ForceField{
    public:
        ConstantForceField(Vec3f _force){
            force = _force;
        }
        virtual Vec3f getAcceleration(const Vec3f &position, float mass, float t) const{
            return force*(1.0/mass);
        }

    private:
        Vec3f force;
};

class RadialForceField: public ForceField{
    public:
        RadialForceField(float _magnitude){
            magnitude = _magnitude;
        }
        virtual Vec3f getAcceleration(const Vec3f &position, float mass, float t) const{
            return position * (-1.0 / mass) * magnitude;
        } // 先单位化再乘距离等于没做单位化

    private:
        float magnitude;
};

class VerticalForceField: public ForceField{
    public:
        VerticalForceField(float _magnitude){
            magnitude = _magnitude;
        }
        virtual Vec3f getAcceleration(const Vec3f &position, float mass, float t) const{
            return Vec3f(0, -1, 0) * (position.y() / mass) * magnitude;
        }

    private:
        float magnitude;
};

#endif