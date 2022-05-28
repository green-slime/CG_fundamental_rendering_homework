#ifndef _SURFACE_H_
#define _SURFACE_H_

#include "spline.h"
#include "curve.h"
#include "triangle_mesh.h"

class Surface: public Spline{
public:
    Surface() {}
};

class SurfaceOfRevolution: public Surface{
    public:
        SurfaceOfRevolution(Curve *_c) { c = _c; }
        virtual void Paint(ArgParser *args);
        virtual TriangleMesh *OutputTriangles(ArgParser *args);
        virtual void OutputBezier(FILE *file){
            fprintf(file, "surface_of_revolution\n");
            c->OutputBezier(file);
        }
        virtual void OutputBSpline(FILE *file){
            fprintf(file, "surface_of_revolution\n");
            c->OutputBSpline(file);
        }

    private:
        Curve *c;
};

class BezierPatch: public Surface{
    public:
        BezierPatch() { p.resize(16); }
        void set(int i, Vec3f v) { p[i] = v; }
        virtual void Paint(ArgParser *args);
        static Vec3f Bezier4p(Vec3f p1, Vec3f p2, Vec3f p3, Vec3f p4, float t);
        virtual TriangleMesh *OutputTriangles(ArgParser *args);

    private:
        vector<Vec3f> p;
};
#endif