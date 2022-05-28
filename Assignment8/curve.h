#ifndef _CURVE_H_
#define _CURVE_H_

#include "spline.h"
#include <vector>

class Curve:public Spline{
    public:
        Curve(int _vernum) {
            vernum = _vernum;
            controlPoints.resize(vernum);
        }
        void set(int i,Vec3f v){
            controlPoints[i] = v;
        }

        virtual int getNumVertices() { return vernum; }
        virtual Vec3f getVertex(int i) { return controlPoints[i]; }

        virtual void moveControlPoint(int selectedPoint, float x, float y) { 
            controlPoints[selectedPoint] = Vec3f(x, y, 0); 
        }
        void insertVertex(int i,Vec3f newVer){
            controlPoints.insert(controlPoints.begin() + i, newVer);
        }
        void deleteVertex(int i){
            controlPoints.erase(controlPoints.begin() + i);
        }
        void changeNum(int i) { vernum += i; }
        void setCurve(int i, Vec3f point) { curvePoints[i] = point; }
        Vec3f getCurve(int i) { return curvePoints[i]; }
        void reserveCurve(int n) { curvePoints.resize(n); }
        int sizeCurve() { return curvePoints.size(); }

    private:
        int vernum;
        std::vector<Vec3f> controlPoints,curvePoints;
};

class BezierCurve:public Curve{
    public:
        BezierCurve(int _vernum) : Curve(_vernum){}

        void Paint4p(ArgParser *args, int ind); // 画出controlPoints[ind]到[ind+3]的内容
        virtual void Paint(ArgParser *args);
        virtual void OutputBezier(FILE *file);
        virtual void OutputBSpline(FILE *file);
        virtual void addControlPoint(int selectedPoint, float x, float y){
            printf("BezierCurve cannot add Points.\n");
        }
        virtual void deleteControlPoint(int selectedPoint){
            printf("BezierCurve cannot delete Points.\n");
        }

    private:

};

class BSplineCurve:public Curve{
    public:
        BSplineCurve(int _vernum) : Curve(_vernum){}

        void Paint4p(ArgParser *args, int ind); // 画出controlPoints[ind]到[ind+3]的内容
        virtual void Paint(ArgParser *args);
        virtual void OutputBezier(FILE *file);
        virtual void OutputBSpline(FILE *file);

        virtual void addControlPoint(int selectedPoint, float x, float y){
            changeNum(1);
            insertVertex(selectedPoint, Vec3f(x, y, 0));
        }
        virtual void deleteControlPoint(int selectedPoint){
            if(getNumVertices()==4)
                printf("at least 4 points.\n");
            else{
                changeNum(-1);
                deleteVertex(selectedPoint);
            }
        }

    private:
};
#endif