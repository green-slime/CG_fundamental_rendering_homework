#include "curve.h"
#include "matrix.h"
#include <GL/gl.h>
#include <math.h>
#include <string>

float Bbezier[16] = {-1, 3, -3, 1, 3, -6, 3, 0, -3, 3, 0, 0, 1, 0, 0, 0};
float Bbspline[16] = {-1, 3, -3, 1, 3, -6, 3, 0, -3, 0, 3, 0, 1, 4, 1, 0};
Matrix BBezier, BBSpline;

void BezierCurve::Paint4p(ArgParser *arg,int ind){
    int n = arg->curve_tessellation;
    float dt = 1.0 / n;
    glPointSize(5);
    glBegin(GL_POINTS);
    glColor3f(1, 0, 0);

    for (int i = ind; i < ind+4;i++){
        // 先画顶点
        glVertex3f(getVertex(i).x(), getVertex(i).y(), getVertex(i).z());
    }
    glEnd();
    glBegin(GL_LINES);   
    glColor3f(0, 0, 1);
    for (int i = ind; i < ind+3;i++){
        // 画控制线
        glVertex3f(getVertex(i).x(), getVertex(i).y(), getVertex(i).z());
        glVertex3f(getVertex(i+1).x(), getVertex(i+1).y(), getVertex(i+1).z());
    }
    glEnd();

    for (int i = 0; i <= n;i++){
        float t = i * dt;
        Vec3f Qt = getVertex(ind) * pow((1 - t), 3) + getVertex(ind+1) * 3 * t * pow((1 - t), 2) + getVertex(ind+2) * 3 * pow(t, 2) * (1 - t) + getVertex(ind+3) * pow(t, 3);
        setCurve(i + ind / 3 * (n + 1), Qt);
    }
    glLineWidth(3);
    glBegin(GL_LINES);
    
    glColor3f(0, 1, 0);
    for (int i = 0; i < n;i++){
        int j = i + ind / 3 * (n + 1);
        glVertex3f(getCurve(j).x(), getCurve(j).y(), getCurve(j).z());
        glVertex3f(getCurve(j+1).x(), getCurve(j+1).y(), getCurve(j+1).z());
    }
    glEnd();
}

void BezierCurve::Paint(ArgParser *arg){
    assert((getNumVertices() - 1) % 3 == 0);
    int n = (getNumVertices() - 1) / 3;
    reserveCurve(n * (arg->curve_tessellation + 1));
    for (int k = 0; k < n;k++)
        Paint4p(arg, 3 * k);
}


void BSplineCurve::Paint4p(ArgParser *arg,int ind){
    int n = arg->curve_tessellation;
    float dt = 1.0 / n;
    glPointSize(5);
    glBegin(GL_POINTS);
    glColor3f(1, 0, 0);

    for (int i = ind; i < ind+4;i++){
        // 先画顶点
        glVertex3f(getVertex(i).x(), getVertex(i).y(), getVertex(i).z());
    }
    glEnd();
    glBegin(GL_LINES);   
    glColor3f(0, 0, 1);
    for (int i = ind; i < ind+3;i++){
        // 画控制线
        glVertex3f(getVertex(i).x(), getVertex(i).y(), getVertex(i).z());
        glVertex3f(getVertex(i+1).x(), getVertex(i+1).y(), getVertex(i+1).z());
    }
    glEnd();


    for (int i = 0; i <= n;i++){
        float t = i * dt;
        Vec3f Qt = getVertex(ind) * (pow((1 - t), 3) / 6.0) + getVertex(ind+1) * ((3 * pow(t, 3) - 6 * pow(t, 2) + 4) / 6.0) + getVertex(ind+2) * ((-3 * pow(t, 3) + 3 * pow(t, 2) + 3 * t + 1) / 6.0) + getVertex(ind+3) * (pow(t, 3) / 6.0);
        setCurve(i + ind * (n + 1), Qt);
    }
    glLineWidth(3);
    glBegin(GL_LINES);
    
    glColor3f(0, 1, 0);
    for (int i = 0; i < n;i++){
        int j = i + ind * (n + 1);
        glVertex3f(getCurve(j).x(), getCurve(j).y(), getCurve(j).z());
        glVertex3f(getCurve(j+1).x(), getCurve(j+1).y(), getCurve(j+1).z());
    }
    glEnd();
}

void BSplineCurve::Paint(ArgParser *arg){
    int n = getNumVertices() - 3;
    reserveCurve(n * (arg->curve_tessellation + 1));
    for (int k = 0; k < n;k++)
        Paint4p(arg, k);
}


void BezierCurve::OutputBezier(FILE *file){
    fprintf(file,"bezier\n");
    fprintf(file,"num_vertices %d\n",getNumVertices());
    for (int i = 0; i < getNumVertices();i++){
        float x = getVertex(i).x(), y = getVertex(i).y(), z = getVertex(i).z();
        fprintf(file,"%f %f %f\n",x,y,z);
    }
}

void BezierCurve::OutputBSpline(FILE *file){
    // 先计算控制点的值
    BBezier.Set16(Bbezier);
    BBSpline.Set16(Bbspline);
    BBSpline *= (1.0 / 6);

    Matrix GBezier, GBSpline;
    for (int y = 0; y < 4;y++){
        Vec3f point = getVertex(y);
        GBezier.Set(y, 0, point.x());
        GBezier.Set(y, 1, point.y());
        GBezier.Set(y, 2, point.z());
        GBezier.Set(y, 3, 0);
    }
    BBSpline.Inverse(BBSpline);
    
    GBSpline = GBezier * BBezier * BBSpline;   
    

    fprintf(file, "bspline\n");
    fprintf(file,"num_vertices %d\n",getNumVertices());
    for (int i = 0; i < 4;i++){
        float x = GBSpline.Get(i,0), y = GBSpline.Get(i,1), z = GBSpline.Get(i,2);
        fprintf(file,"%f %f %f\n",x,y,z);
    }
}

void BSplineCurve::OutputBSpline(FILE *file){
    fprintf(file,"bspline\n");
    fprintf(file,"num_vertices %d\n",getNumVertices());
    for (int i = 0; i < getNumVertices();i++){
        float x = getVertex(i).x(), y = getVertex(i).y(), z = getVertex(i).z();
        fprintf(file,"%f %f %f\n",x,y,z);
    }
}

void BSplineCurve::OutputBezier(FILE *file){
    // 先计算控制点的值
    BBezier.Set16(Bbezier);
    BBSpline.Set16(Bbspline);
    BBSpline *= (1.0 / 6);

    Matrix GBezier, GBSpline;
    for (int y = 0; y < 4;y++){
        Vec3f point = getVertex(y);
        GBSpline.Set(y, 0, point.x());
        GBSpline.Set(y, 1, point.y());
        GBSpline.Set(y, 2, point.z());
        GBSpline.Set(y, 3, 0);
    }
    BBezier.Inverse(BBezier);
    GBezier = GBSpline * BBSpline * BBezier;   

    fprintf(file, "bezier\n");
    fprintf(file,"num_vertices %d\n",getNumVertices());
    for (int i = 0; i < 4;i++){
        float x = GBezier.Get(i,0), y = GBezier.Get(i,1), z = GBezier.Get(i,2);
        fprintf(file,"%f %f %f\n",x,y,z);
    }
}