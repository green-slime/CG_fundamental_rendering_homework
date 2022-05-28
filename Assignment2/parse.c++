#include <stdio.h>
#include <string.h>
#include "scene_parser.h"
#include "camera.h"
#include "image.h"
#include "hit.h"
#include "group.h"
#include "light.h"
#include "vectors.h"

void parse(int argc, char* argv[]){
    char *input_file = NULL;
    int width = 100;
    int height = 100;
    char *output_file = NULL;
    char *normal_file = NULL;

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
    } else if (!strcmp(argv[i],"-normal")) {
        i++; assert (i < argc); 
        normal_file = argv[i];
    } else {
        printf ("whoops error with command line argument %d: '%s'\n",i,argv[i]);
        assert(0);
    }
    }
    int sizepic=min(height,width);
    SceneParser scene(input_file);
    Image sceneimg(sizepic,sizepic),normalimg(sizepic,sizepic);

    Vec3f bg=scene.getBackgroundColor();
    
    Vec3f black(0,0,0);
    normalimg.SetAllPixels(black);

    Camera *cam=scene.getCamera();
    float sizecam=cam->getsize();
    Group *group=scene.getGroup();
    float interval=sizecam*1.0/sizepic;

    int lightnum=scene.getNumLights();

    for(int i=0;i<sizepic;i++){
        for(int j=0;j<sizepic;j++){
            Vec2f point(i*interval,j*interval);
            Ray ray=cam->generateRay(point);
            Hit hit(INFINITY,bg,black);
            Vec3f col;
            if(group->intersect(ray,hit,-INFINITY)){
                Vec3f p=ray.pointAtParameter(hit.getT());
                Vec3f colp=hit.getColor();
                col=colp*scene.getAmbientLight();
                for(int k=0;k<lightnum;k++){
                    Vec3f dir,Lk;
                    Light* light=scene.getLight(k);
                    light->getIllumination(p,dir,Lk);
                    col+=colp*Lk*max(0.0f,dir.Dot3(hit.getNormal()));
                } 
            } // 不为背景
            else{
                col=bg*scene.getAmbientLight();
                for(int k=0;k<lightnum;k++){
                    Vec3f dir,Lk,p(1,1,1); // p whatever
                    Light* light=scene.getLight(k);
                    light->getIllumination(p,dir,Lk);
                    col+=bg*Lk;
                }
            }
            Vec3f normal=hit.getNormal();
            Vec3f drawnormal(abs(normal.x()),abs(normal.y()),abs(normal.z()));
            normalimg.SetPixel(i,j,drawnormal);
            sceneimg.SetPixel(i,j,col);

            
        }
    }
    const char* filename="D:\\C++\\MIT_class\\Assignment2\\results\\";
    char path[100];
    if(output_file!=NULL){
        strcpy(path,filename);
        strcat(path,output_file);
        sceneimg.SaveTGA(path);
    }
    if(normal_file!=NULL){
        strcpy(path,filename);
        strcat(path,normal_file);
        normalimg.SaveTGA(path);
    }
    
}