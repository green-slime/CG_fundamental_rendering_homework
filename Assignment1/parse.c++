#include <stdio.h>
#include <string.h>
#include "scene_parser.h"
#include "camera.h"
#include "image.h"
#include "hit.h"
#include "group.h"

void parse(int argc, char* argv[]){
    char *input_file = NULL;
    int width = 100;
    int height = 100;
    char *output_file = NULL;
    float depth_min = 0;
    float depth_max = 1;
    char *depth_file = NULL;

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
    } else if (!strcmp(argv[i],"-depth")) {
        i++; assert (i < argc); 
        depth_min = atof(argv[i]);
        i++; assert (i < argc); 
        depth_max = atof(argv[i]);
        i++; assert (i < argc); 
        depth_file = argv[i];
    } else {
        printf ("whoops error with command line argument %d: '%s'\n",i,argv[i]);
        assert(0);
    }
    }
    int sizepic=min(height,width);
    SceneParser scene(input_file);
    Image sceneimg(sizepic,sizepic),depthimg(sizepic,sizepic);
    Vec3f bg=scene.getBackgroundColor();
    sceneimg.SetAllPixels(bg);
    Vec3f black(0,0,0);
    depthimg.SetAllPixels(black);

    Camera *cam=scene.getCamera();
    int sizecam=cam->getsize();
    Group *group=scene.getGroup();
    float interval=sizecam*1.0/sizepic;

    for(int i=0;i<sizepic;i++){
        for(int j=0;j<sizepic;j++){
            Vec2f point(i*interval,j*interval);
            Ray ray=cam->generateRay(point);
            Hit hit(INFINITY,bg);
            group->intersect(ray,hit,-INFINITY);
            Vec3f col=hit.getColor();
            sceneimg.SetPixel(i,j,col);

            float t=hit.getT();
            if(t>=depth_min&&t<=depth_max){
                float range=depth_max-depth_min;
                float gray=(depth_max-t)*1.0/range;
                Vec3f col(gray,gray,gray);
                depthimg.SetPixel(i,j,col);
            }
            else depthimg.SetPixel(i,j,black);
        }
    }
    const char* filename="D:\\C++\\MIT_class\\Assignment1\\results\\";
    char path[100];
    strcpy(path,filename);
    strcat(path,output_file);
    sceneimg.SaveTGA(path);
    strcpy(path,filename);
    strcat(path,depth_file);
    depthimg.SaveTGA(path);
}