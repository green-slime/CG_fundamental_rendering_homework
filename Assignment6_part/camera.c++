#include "camera.h"
#include <assert.h>
#include "matrix.h"
#include <GL/gl.h>
#include <GL/glu.h>

Ray OrthographicCamera::generateRay(Vec2f point){  
    float x=point.x(),y=point.y();
    assert(x>=0&&x<size&&y>=0&&y<size);
    Vec3f ori=center-screen_up*(size/2.0)-horizontal*(size/2.0)+screen_up*y+horizontal*x;
    Ray ray(direction,ori);
    return ray;
}

Ray PerspectiveCamera::generateRay(Vec2f point){
    float x=point.x(),y=point.y();
    assert(x>=0&&x<imgsize&&y>=0&&y<imgsize);
    Vec3f ori=center-screen_up*(imgsize/2.0)-horizontal*(imgsize/2.0)+screen_up*y+horizontal*x+direction;
    Vec3f dir=ori-center;
    dir.Normalize();
    Ray ray(dir,ori);
    return ray;
}

void OrthographicCamera::glInit(int w, int h)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (w > h)
    glOrtho(-size/2.0, size/2.0, -size*(float)h/(float)w/2.0, size*(float)h/(float)w/2.0, 0.5, 40.0);
  else
    glOrtho(-size*(float)w/(float)h/2.0, size*(float)w/(float)h/2.0, -size/2.0, size/2.0, 0.5, 40.0);
}

// ====================================================================
// Place an orthographic camera within an OpenGL scene
// ====================================================================

void OrthographicCamera::glPlaceCamera(void)
{
  gluLookAt(center.x(), center.y(), center.z(),
            center.x()+direction.x(), center.y()+direction.y(), center.z()+direction.z(),
            up.x(), up.y(), up.z());
}

// ====================================================================
// dollyCamera, truckCamera, and RotateCamera
//
// Asumptions:
//  - up is really up (i.e., it hasn't been changed
//    to point to "screen up")
//  - up and direction are normalized
// Special considerations:
//  - If your constructor precomputes any vectors for
//    use in 'generateRay', you will likely to recompute those
//    values at athe end of the these three routines
// ====================================================================

// ====================================================================
// dollyCamera: Move camera along the direction vector
// ====================================================================

void OrthographicCamera::dollyCamera(float dist)
{
  center += direction*dist;

  // 其它不变

}

// ====================================================================
// truckCamera: Translate camera perpendicular to the direction vector
// ====================================================================

void OrthographicCamera::truckCamera(float dx, float dy)
{
  Vec3f horizontal;
  Vec3f::Cross3(horizontal, direction, up);
  horizontal.Normalize();

  Vec3f screenUp;
  Vec3f::Cross3(screenUp, horizontal, direction);

  center += horizontal*dx + screenUp*dy;

  // ===========================================
  // ASSIGNMENT 3: Fix any other affected values
  // ===========================================


}

// ====================================================================
// rotateCamera: Rotate around the up and horizontal vectors
// ====================================================================

void OrthographicCamera::rotateCamera(float rx, float ry)
{
  Vec3f _horizontal;
  Vec3f::Cross3(_horizontal, direction, up);
  _horizontal.Normalize();

  // Don't let the model flip upside-down (There is a singularity
  // at the poles when 'up' and 'direction' are aligned)
  float tiltAngle = acos(up.Dot3(direction));
  if (tiltAngle-ry > 3.13)
    ry = tiltAngle - 3.13;
  else if (tiltAngle-ry < 0.01)
    ry = tiltAngle - 0.01;

  Matrix rotMat = Matrix::MakeAxisRotation(up, rx);
  rotMat *= Matrix::MakeAxisRotation(_horizontal, ry);

  rotMat.Transform(center);
  rotMat.TransformDirection(direction);
  
  // ===========================================
  // ASSIGNMENT 3: Fix any other affected values
  // ===========================================
  rotMat.TransformDirection(screen_up);
  rotMat.TransformDirection(up);
  rotMat.TransformDirection(horizontal);
}

// ====================================================================
// ====================================================================

// ====================================================================
// Create a perspective camera with the appropriate dimensions that
// crops or stretches in the x-dimension as necessary
// ====================================================================

void PerspectiveCamera::glInit(int w, int h)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(angle*180.0/3.14159, (float)w/float(h), 0.5, 40.0);
}

// ====================================================================
// Place a perspective camera within an OpenGL scene
// ====================================================================

void PerspectiveCamera::glPlaceCamera(void)
{
  gluLookAt(center.x(), center.y(), center.z(),
            center.x()+direction.x(), center.y()+direction.y(), center.z()+direction.z(),
            up.x(), up.y(), up.z());
}

// ====================================================================
// dollyCamera, truckCamera, and RotateCamera
//
// Asumptions:
//  - up is really up (i.e., it hasn't been changed
//    to point to "screen up")
//  - up and direction are normalized
// Special considerations:
//  - If your constructor precomputes any vectors for
//    use in 'generateRay', you will likely to recompute those
//    values at athe end of the these three routines
// ====================================================================

// ====================================================================
// dollyCamera: Move camera along the direction vector
// ====================================================================

void PerspectiveCamera::dollyCamera(float dist)
{
  center += direction*dist;

  // ===========================================
  // ASSIGNMENT 3: Fix any other affected values
  // ===========================================


}

// ====================================================================
// truckCamera: Translate camera perpendicular to the direction vector
// ====================================================================

void PerspectiveCamera::truckCamera(float dx, float dy)
{
  Vec3f horizontal;
  Vec3f::Cross3(horizontal, direction, up);
  horizontal.Normalize();

  Vec3f screenUp;
  Vec3f::Cross3(screenUp, horizontal, direction);

  center += horizontal*dx + screenUp*dy;

  // ===========================================
  // ASSIGNMENT 3: Fix any other affected values
  // ===========================================


}

// ====================================================================
// rotateCamera: Rotate around the up and horizontal vectors
// ====================================================================

void PerspectiveCamera::rotateCamera(float rx, float ry)
{
  Vec3f _horizontal;
  Vec3f::Cross3(_horizontal, direction, up);
  _horizontal.Normalize();

  // Don't let the model flip upside-down (There is a singularity
  // at the poles when 'up' and 'direction' are aligned)
  float tiltAngle = acos(up.Dot3(direction));
  if (tiltAngle-ry > 3.13)
    ry = tiltAngle - 3.13;
  else if (tiltAngle-ry < 0.01)
    ry = tiltAngle - 0.01;

  Matrix rotMat = Matrix::MakeAxisRotation(up, rx);
  rotMat *= Matrix::MakeAxisRotation(_horizontal, ry);

  rotMat.Transform(center);
  rotMat.TransformDirection(direction);
  direction.Normalize();

  // ===========================================
  // ASSIGNMENT 3: Fix any other affected values
  // ===========================================
  rotMat.TransformDirection(up);
  rotMat.TransformDirection(screen_up);
  rotMat.TransformDirection(horizontal);

}

// ====================================================================