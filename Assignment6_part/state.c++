#include "state.h"

int theta_steps = 10, phi_steps = 10;
bool gouraud = 0, shade_back=0,visualize_grid=0,stats=0;
Vec3f whitecol(1, 1, 1), black(0, 0, 0),redcol(1,0,0),bluecol(0,0,1),purplecol(1,0,1),greencol(0,1,0),aquacol(0,1,1);
PhongMaterial white(whitecol),red(redcol),blue(bluecol),purple(purplecol),aqua(aquacol),green(greencol);