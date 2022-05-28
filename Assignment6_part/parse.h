#include <stdio.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/glut.h>

#include "scene_parser.h"
#include "camera.h"
#include "raytracer.h"
#include "image.h"
#include "hit.h"
#include "material.h"
#include "grid.h"
#include "matrix.h"

#include "vectors.h"
#include "glCanvas.h"
#include "state.h"

char *input_file = NULL;
int width = 100;
int height = 100;
char *output_file = NULL;
char *normal_file = NULL;
bool gui = 0;
SceneParser *scene = NULL;
int max_bounces = 0;
float cutoff_weight = 0;
bool shadows = 0;
int nx = 0, ny = 0, nz = 0;
Grid *grid = NULL;

void rendering(void){
    int sizepic=min(height,width);
    Image sceneimg(sizepic,sizepic);

    Camera *cam=scene->getCamera();
    float sizecam=cam->getsize();
    Vec3f black(0, 0, 0);
    Vec3f bg=scene->getBackgroundColor();
    PhongMaterial cosmos(bg,black,1.0,black,black,1);
    
    float interval=sizecam*1.0/sizepic;
    

    RayTracer tracer(scene, max_bounces, cutoff_weight,shadows,grid);

    for(int i=0;i<sizepic;i++){
        for(int j=0;j<sizepic;j++){
            Hit hit(INFINITY,&cosmos,black);
            Vec2f point(i*interval,j*interval);
            Ray ray=cam->generateRay(point);
            Vec3f col=tracer.traceRay(ray, 1e-5, 0, 1, 1, hit);
            sceneimg.SetPixel(i,j,col);
       
        }
    }
    if(stats==1) raystats.PrintStatistics();
    const char* filename="D:\\C++\\MIT_class\\Assignment6\\results\\";
    char path[100];
    if(output_file!=NULL){
        strcpy_s(path,filename);
        strcat_s(path,output_file);
        sceneimg.SaveTGA(path);
    }
}

void tracing(float x,float y){
    assert(x <= 1 && y <= 1);
    Camera *cam=scene->getCamera();
    float sizecam=cam->getsize();
    Vec3f black(0, 0, 0);
    Vec3f bg=scene->getBackgroundColor();
    PhongMaterial cosmos(bg,black,1.0,black,black,0);
    
    RayTracer tracer(scene, max_bounces, cutoff_weight,shadows,grid);
    Hit hit(INFINITY,&cosmos,black);
    Vec2f point(x * sizecam, y * sizecam);
    Ray ray=cam->generateRay(point);
    Vec3f col=tracer.traceRay(ray, 1e-5, 0, 1, 1, hit);

}

void parse(int argc, char* argv[]){
    
    // sample command lines:
    // raycast -input input.txt -size 100 100 -output output.tga
    // raycast -input input.txt -size 100 100 -depth 5.5 8.8 output.tga

    for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i],"-input")) {
        i++; assert (i < argc); 
        input_file = argv[i];
    } else if (!strcmp(argv[i],"-size")) {
        i++; assert (i < argc); 
        width = atoi(argv[i]);
        i++; assert (i < argc); 
        height = atoi(argv[i]);
    } else if (!strcmp(argv[i],"-output")) {
        i++; assert (i < argc); 
        output_file = argv[i];
    } else if (!strcmp(argv[i],"-gui")) {
        assert(i < argc);
        gui = 1;
    } else if (!strcmp(argv[i],"-grid")) {
        i++; assert (i < argc); 
        nx = atoi(argv[i]);
        i++; assert (i < argc); 
        ny = atoi(argv[i]);
        i++; assert (i < argc); 
        nz = atoi(argv[i]);
    } else if (!strcmp(argv[i],"-stats")) {
        assert (i < argc);
        stats = 1;
    } else if (!strcmp(argv[i],"-visualize_grid")) {
        assert (i < argc);
        visualize_grid = 1;
    }  else if (!strcmp(argv[i],"-shadows")) {
        assert (i < argc);
        shadows = 1;
    } else if (!strcmp(argv[i],"-bounces")) {
        i++; assert (i < argc); 
        max_bounces = atoi(argv[i]);
    } else if (!strcmp(argv[i],"-weight")) {
        i++; assert (i < argc); 
        cutoff_weight = atof(argv[i]);
    } else if (!strcmp(argv[i],"-shade_back")) {
        shade_back = 1;
    } else if (!strcmp(argv[i],"-tessellation")) {
        i++; assert (i < argc); 
        theta_steps = atoi(argv[i]);
        i++; assert (i < argc); 
        phi_steps = atoi(argv[i]);
    } else if (!strcmp(argv[i],"-gouraud")) {
        assert (i < argc);
        gouraud = 1;
    } else {
        printf ("whoops error with command line argument %d: '%s'\n",i,argv[i]);
        assert(0);
    }
    }
    SceneParser scene1(input_file);
    scene = &scene1;

    if(stats==1) raystats.Initialize(width, height, scene->getGroup()->getBoundingBox(), nx, ny, nz);

    if(nx==0)
        grid = NULL;
    else{
        //scene->getGroup()->updateBox();
        grid = new Grid(scene->getGroup()->getBoundingBox(), nx, ny, nz);
        scene->getGroup()->insertIntoGrid(grid, NULL); // 这一步更新了场景Group的boundingbox
        grid->setBoundingBox(scene->getGroup()->getBoundingBox());

        /*
        int x = grid->getx(), y = grid->gety(), z = grid->getz();
        for (int i = 0; i < x;i++){
            for (int j = 0; j < y;j++){
                for (int k = 0; k < z;k++){
                    printf("%d,%d,%d=%d\n", i, j, k, grid->getGrid(i, j, k));
                }
            }
        }
        */
    }
    if(gui){
        GLCanvas canv;
        canv.initialize(scene, rendering, tracing, grid, visualize_grid);
    }
    else{
        rendering();
    }
    
    
}