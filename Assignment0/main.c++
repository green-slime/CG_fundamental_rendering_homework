#include<stdio.h>
#include"ifs.h"
#include"image.h"

int main(int arg,char *argv[]){
    ifs test(arg,argv);
    test.transform();
    printf("success.\n");
}