// state.h 用于保存全局变量

#ifndef _STATE_H_
#define _STATE_H_
#include "material.h"
#include "raytracing_stats.h"

extern int theta_steps, phi_steps;
extern bool gouraud,shade_back,visualize_grid,stats;
extern PhongMaterial white,purple,red,green,blue,aqua;
extern RayTracingStats raystats;

#endif