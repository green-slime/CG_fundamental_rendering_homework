#include "surface.h"
#include "triangle_mesh.h"
#include <GL/gl.h>
#define M_PI 3.141592654

void SurfaceOfRevolution::Paint(ArgParser *args){
    c->Paint(args);
    int vernum = c->sizeCurve(), revonum = args->revolution_tessellation;
    float dtheta = 2 * M_PI / revonum;
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < revonum;i++){
        float theta1 = i * dtheta, theta2 = (i + 1) * dtheta;
        for (int j = 0; j < vernum-1;j++){           
            Vec3f normal;
            float x1 = c->getCurve(j).x(), y1 = c->getCurve(j).y();
            float x2 = c->getCurve(j + 1).x(), y2 = c->getCurve(j + 1).y();
            Vec3f a1(x1 * cos(theta1), y1, x1 * sin(theta1)), b1(x2 * cos(theta1), y2, x2 * sin(theta1)), a2(x1 * cos(theta2), y1, x1 * sin(theta2)), b2(x2 * cos(theta2), y2, x2 * sin(theta2));
            normal.Cross3(normal, a1 - b1, a1 - a2);
            normal.Normalize();
            glNormal3f(normal.x(), normal.y(), normal.z());
            glVertex3f(a1.x(), a1.y(), a1.z());
            glVertex3f(b1.x(), b1.y(), b1.z());
            glVertex3f(a2.x(), a2.y(), a2.z());

            normal.Cross3(normal, b1 - b2, b1 - a2);
            normal.Normalize();
            glNormal3f(normal.x(), normal.y(), normal.z());
            glVertex3f(b2.x(), b2.y(), b2.z());
            glVertex3f(b1.x(), b1.y(), b1.z());
            glVertex3f(a2.x(), a2.y(), a2.z());
            
        }
    }
    glEnd();
}

TriangleMesh* SurfaceOfRevolution::OutputTriangles(ArgParser *args){
    c->Paint(args);
    int vernum = c->sizeCurve(), revonum = args->revolution_tessellation;
    int k = args->curve_tessellation + 1;
    int n = vernum / k;
    TriangleNet *mesh = new TriangleNet(0,0);
    float dtheta = 2 * M_PI / revonum;
    for (int patch = 0; patch < n;patch++){
        TriangleNet m(k - 1, revonum);
        for (int i = 0; i <= revonum;i++){
            float theta = i * dtheta;
            for (int j = 0; j < k;j++){
                int ind = j + patch * k;
                float x = c->getCurve(ind).x(), y = c->getCurve(ind).y();
                Vec3f v(x * cos(theta), y, x * sin(theta));
                m.SetVertex(j, i, v);
            }
        }
        mesh->Merge(m);
    }
    return mesh;
}

Vec3f BezierPatch::Bezier4p(Vec3f p1, Vec3f p2, Vec3f p3, Vec3f p4, float t){
    return p1 * pow(1 - t, 3) + p2 * 3 * t * pow(1 - t, 2) + p3 * 3 * pow(t, 2) * (1 - t) + p4 * pow(t, 3);
}

void BezierPatch::Paint(ArgParser *arg){
    int snum = arg->curve_tessellation, tnum = arg->patch_tessellation;
    float ds = 1.0 / snum, dt = 1.0 / tnum;
    vector<vector<Vec3f>> Qst(tnum + 1, vector<Vec3f>(snum + 1));
    for (int i = 0; i <= tnum;i++){
        float t = i * dt;
        for (int j = 0; j <= snum;j++){
            float s = j * ds;
            Vec3f p1 = Bezier4p(p[0], p[1], p[2], p[3], s), p2 = Bezier4p(p[4], p[5], p[6], p[7], s), p3 = Bezier4p(p[8], p[9], p[10], p[11], s), p4 = Bezier4p(p[12], p[13], p[14], p[15], s);
            Qst[i][j] = Bezier4p(p1, p2, p3, p4, t);
        }
    }
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < tnum;i++){
        for (int j = 0; j < snum;j++){
            Vec3f a1 = Qst[i][j], a2 = Qst[i][j + 1], b1 = Qst[i + 1][j], b2 = Qst[i + 1][j + 1];
            Vec3f normal;
            normal.Cross3(normal, a1 - a2, a1 - b1);
            normal.Normalize();
            glNormal3f(normal.x(), normal.y(), normal.z());
            glVertex3f(a1.x(), a1.y(), a1.z());
            glVertex3f(a2.x(), a2.y(), a2.z());
            glVertex3f(b1.x(), b1.y(), b1.z());

            normal.Cross3(normal, b2 - b1, b2 - a2);
            normal.Normalize();
            glNormal3f(normal.x(), normal.y(), normal.z());
            glVertex3f(b2.x(), b2.y(), b2.z());
            glVertex3f(b1.x(), b1.y(), b1.z());
            glVertex3f(a2.x(), a2.y(), a2.z());
        }
    }
    glEnd();
}

TriangleMesh* BezierPatch::OutputTriangles(ArgParser *args){
    int snum = args->curve_tessellation, tnum = args->patch_tessellation;
    float ds = 1.0 / snum, dt = 1.0 / tnum;
    vector<vector<Vec3f>> Qst(tnum + 1, vector<Vec3f>(snum + 1));
    for (int i = 0; i <= tnum;i++){
        float t = i * dt;
        for (int j = 0; j <= snum;j++){
            float s = j * ds;
            Vec3f p1 = Bezier4p(p[0], p[1], p[2], p[3], s), p2 = Bezier4p(p[4], p[5], p[6], p[7], s), p3 = Bezier4p(p[8], p[9], p[10], p[11], s), p4 = Bezier4p(p[12], p[13], p[14], p[15], s);
            Qst[i][j] = Bezier4p(p1, p2, p3, p4, t);
        }
    }

    TriangleNet *mesh = new TriangleNet(tnum, snum);
    for (int i = 0; i <= tnum;i++){
        for (int j = 0; j <= snum;j++){
            mesh->SetVertex(i, j, Qst[i][j]);
        }
    }
    return mesh;
}