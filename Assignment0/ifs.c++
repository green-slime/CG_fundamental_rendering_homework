#include<stdio.h>
#include<vector>
#include<random>
#include<string>
#include<string.h>
#include "image.h"
#include "ifs.h"
using namespace std;

ifs::ifs(int argc,char *argv[]){
// Some sample code you might like to use for parsing 
// command line arguments and the input IFS files

// sample command line:
// ifs -input fern.txt -points 10000 -iters 10 -size 100 -output fern.tga

    char *input_file = NULL;
    int num_points = 10000;
    int num_iters = 10;
    int picsize = 100;
    char *output_file = NULL;

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i],"-input")) {
            i++; assert (i < argc); 
            input_file = argv[i];
        } else if (!strcmp(argv[i],"-points")) {
            i++; assert (i < argc); 
            num_points = atoi(argv[i]);
        } else if (!strcmp(argv[i],"-iters")) {
            i++; assert (i < argc); 
            num_iters = atoi(argv[i]);
        } else if (!strcmp(argv[i],"-size")) {
            i++; assert (i < argc); 
            picsize = atoi(argv[i]);
        } else if (!strcmp(argv[i],"-output")) {
            i++; assert (i < argc); 
            output_file = argv[i];
        } else {
            printf ("whoops error with command line argument %d: '%s'\n",i,argv[i]);
            assert(0);
        }
    }
    N=num_iters;
    size=picsize;
    points=num_points;
    
    // Some sample code you might like to use for
    // parsing the IFS transformation files

    // open the file
    FILE *input = fopen(input_file,"r");
    assert(input != NULL);

    // read the number of transforms
    int num_transforms; 
    fscanf(input,"%d",&num_transforms);
    n=num_transforms;
    probs.resize(n);
    trans.resize(n);

    // < DO SOMETHING WITH num_transforms >

    // read in the transforms
    for (int i = 0; i < num_transforms; i++) {
        float probability; 
        fscanf (input,"%f",&probability);
        if(i==0) probs[i]=probability;
        else probs[i]=probability+probs[i-1]; // 累计概率，递增
        Matrix m; 
        m.Read3x3(input);
        trans[i]=m;
        // < DO SOMETHING WITH probability and m >
    }
    // close the file
    fclose(input);
    pos.resize(points);
    std::default_random_engine e;
	std::uniform_real_distribution<double> u(0,size);   
	for(int i = 0; i < points; i++){
        int x=u(e), y=u(e);
        pos[i]=Vec3f(x,y,size);        
    }
    // 随机化完成 
    printf("Randomized!points=%d,size=%d\n",points,size);   
    pic = new Image(size,size);
    savepic("0");
    //pic.resize(size,size);
    
}

void ifs::savepic(const char *name){
    (*pic).SetAllPixels(Vec3f(255,255,255)); // 白色底
    
    //printf("Start!\n");
    printf("%d\n",size);
    for(int i=0;i<points;i++){
        //printf("%d,%d\n",int(pos[i].x()),int(pos[i].y()));
        int xx=int(pos[i].x());
        int yy=int(pos[i].y());        
        if(yy>=size) yy=size-1;
        else if(yy<0) yy=0;
        if(xx>=size) xx=size-1;
        else if(xx<0) xx=0;
        pos[i].Set(xx,yy,size);
        (*pic).SetPixel(int(pos[i].x()),int(pos[i].y()),Vec3f(0,0,0)); // 黑色点
    }   
    //printf("End!\n");
    char path[100];
    const char* tga=".tga";
    strcpy(path,filename);
    strcat(path,name);
    strcat(path,tga);
    (*pic).SaveTGA(path);
    printf(" ok!\n");
}

void ifs::transform(){
    std::default_random_engine e;
	std::uniform_real_distribution<double> u(0,1);
    for(int i=0;i<N;i++){
        for(int j=0;j<points;j++){
            double p=u(e);
            int k=0;
            for(k=0;k<n;k++){
                if(p<=probs[k]) break;
                 //probs[n-1]=1,能保证k不越界
            }
            //printf("p=%f,k=%d,j=%d\n",p,k,j);
            trans[k].Transform(pos[j]);
        }
        char b[100];
        std::sprintf(b,"%d",i+1);
        //printf("b=%s\n",b);
        savepic(b);
    }
}