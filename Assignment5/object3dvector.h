#ifndef _OBJECT_3D_VECTOR_H_
#define _OBJECT_3D_VECTOR_H_

#include "assert.h"
#include <vector>
class Object3D;

typedef Object3D* Object3DPtr;

// ====================================================================
// ====================================================================

// Store an collection of Object3D.  The total number to be stored is
// not known initially.  Resize as necessary.

// ====================================================================
// ====================================================================

class Object3DVector {

public:

  // CONSTRUCTOR
  Object3DVector() {
    num_objects = 0;
    size = 10;
    objects.resize(size);
    for (int i = 0; i < size; i++)
      objects[i] = NULL;
  }

  // DESTRUCTOR
  ~Object3DVector() {
    // don't delete the objects, just the array to store the pointers
    
  }

  // ACCESSORS
  int getNumObjects() { return num_objects; }
  Object3D* getObject(int i) { 
    assert (i >= 0 && i < num_objects);
    assert (objects[i] != NULL);
    return objects[i];
  }

  // MODIFIERS
  void addObject(Object3D *o) {
    assert (o != NULL);
    if (size == num_objects) {
      objects.resize(2 * size, NULL);
      size *= 2;
    }
    objects[num_objects] = o;
    num_objects++;
  }

private:

  // REPRESENTATION
  std::vector<Object3D*> objects;
  int size;
  int num_objects;

};

// ====================================================================
// ====================================================================

#endif