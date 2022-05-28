#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "vectors.h"
#include "ray.h"
#include "hit.h"

// ====================================================================
// ====================================================================

// You will extend this class in later assignments

class Material {

public:

  // CONSTRUCTORS & DESTRUCTOR
  Material(const Vec3f &d_color) { diffuseColor = d_color; }
  ~Material() {}

  // ACCESSORS
  virtual Vec3f getDiffuseColor() const { return diffuseColor; }
  virtual Vec3f Shade
    (const Ray &ray, const Hit &hit, const Vec3f &dirToLight, 
     const Vec3f &lightColor) const = 0;
  virtual void glSetMaterial(void) const = 0;

protected:

  // REPRESENTATION
  Vec3f diffuseColor;
  
};

class PhongMaterial : public Material{
public:
  PhongMaterial(const Vec3f &diffuseColor,const Vec3f &_specularColor, float _exponent,const Vec3f &_reflectiveColor, const Vec3f &_transparentColor, float _indexOfRefraction):Material(diffuseColor){
    specularColor = _specularColor;
    exponent = _exponent;
    reflectiveColor = _reflectiveColor;
    transparentColor = _transparentColor;
    indexOfRefraction = _indexOfRefraction;
  }
  PhongMaterial(const Vec3f &diffuseColor):Material(diffuseColor){
    Vec3f black(0, 0, 0);
    specularColor = black;
    exponent = 1;
    reflectiveColor = black;
    transparentColor = black;
    indexOfRefraction = 1;
  }
  ~PhongMaterial(){}
  virtual Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight,const Vec3f &lightColor) const;
  Vec3f getSpecularColor() const{ return specularColor; }
  Vec3f getReflectiveColor() const { return reflectiveColor; }
  Vec3f getTransparentColor() const { return transparentColor; }
  float getIndex() const { return indexOfRefraction; }
  virtual void glSetMaterial(void) const;

private:
    Vec3f specularColor,reflectiveColor,transparentColor;
    float exponent,indexOfRefraction;
};

#endif