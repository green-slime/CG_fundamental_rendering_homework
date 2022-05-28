#include <stdio.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/glut.h>

#include "scene_parser.h"
#include "camera.h"
#include "image.h"
#include "hit.h"
#include "group.h"
#include "light.h"
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

void rendering(void){
    int sizepic=min(height,width);
    Image sceneimg(sizepic,sizepic);
    Vec3f bg=scene->getBackgroundColor();

    Vec3f black(0, 0, 0);
    PhongMaterial cosmos(bg,black,1.0);

    Camera *cam=scene->getCamera();
    float sizecam=cam->getsize();
    Group *group=scene->getGroup();
    float interval=sizecam*1.0/sizepic;

    int lightnum=scene->getNumLights();
    float r = 100.0;
    for(int i=0;i<sizepic;i++){
        for(int j=0;j<sizepic;j++){
            Vec2f point(i*interval,j*interval);
            Ray ray=cam->generateRay(point);
            Hit hit(INFINITY,&cosmos,black);
            Vec3f col;
            if(group->intersect(ray,hit,0.0)){
                Vec3f p=ray.pointAtParameter(hit.getT());
                Vec3f colp=hit.getMaterial()->getDiffuseColor();
                col=colp*scene->getAmbientLight();
                for(int k=0;k<lightnum;k++){
                    Vec3f dir,Lk;
                    Light* light=scene->getLight(k);
                    light->getIllumination(p,dir,Lk,r);
                    Vec3f specular = hit.getMaterial()->Shade(ray, hit, dir, Lk);
                    col+=colp*Lk*max(0.0f,dir.Dot3(hit.getNormal()));
                    col += specular*max(0.0f,dir.Dot3(hit.getNormal()));;
                    //col += specular;
                } 
            } // 不为背景
            else{
                col=bg*scene->getAmbientLight();
                for(int k=0;k<lightnum;k++){
                    Vec3f dir,Lk,p(1,1,1); // p whatever
                    Light* light=scene->getLight(k);
                    light->getIllumination(p,dir,Lk,r);
                    col+=bg*Lk;
                }
            }
            sceneimg.SetPixel(i,j,col);
       
        }
    }

    const char* filename="D:\\C++\\MIT_class\\Assignment3\\results\\";
    char path[100];
    if(output_file!=NULL){
        strcpy_s(path,filename);
        strcat_s(path,output_file);
        sceneimg.SaveTGA(path);
    }
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
        gui = 1;
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

    if(gui){
        GLCanvas canv;
        canv.initialize(scene, rendering);
    }

    
    
}