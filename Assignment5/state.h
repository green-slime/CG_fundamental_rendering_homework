// state.h 用于保存全局变量

#ifndef _STATE_H_
#define _STATE_H_
#include "material.h"

extern int theta_steps, phi_steps;
extern bool gouraud,shade_back,visualize_grid;
extern PhongMaterial white,purple,red,green,blue,aqua;

#endif