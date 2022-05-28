#ifndef _IFS_H_
#define _IFS_H_

#include <stdio.h>
#include <vector>
#include "image.h"
#include "matrix.h"


class ifs{

public:
    Image* pic=NULL;
    ifs(int argc,char *argv[]);
    ~ifs(){
        std::vector<double>().swap(probs);
        std::vector<Matrix>().swap(trans);       
        std::vector<Vec3f>().swap(pos);
    }
    void transform();
    void savepic(const char* name);

private:
    int n; //变换个数
    int N; // 迭代次数
    int size; //图像大小
    int points; //点个数
    //const char* filename="D:\\C++\\test\\";
    const char* filename="D:\\C++\\MIT_class\\Assignment0\\results\\";
    std::vector<double> probs;
    std::vector<Matrix> trans;
    std::vector<Vec3f> pos; // 储存各点位置   
};

#endif