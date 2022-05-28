#include "marchinginfo.h"

void MarchingInfo::nextCell(){
    float t_min_next = min(t_next_x, min(t_next_y, t_next_z));
    if(t_min_next==t_next_x){
        tnear = t_next_x;
        t_next_x += d_tx;
        i += sign_x;       
        normal = Vec3f(-sign_x, 0, 0);
    }
    else if(t_min_next==t_next_y){
        tnear = t_next_y;
        t_next_y += d_ty;
        j += sign_y;        
        normal = Vec3f(0, -sign_y, 0);
    }
    else{
        tnear = t_next_z;
        t_next_z += d_tz;
        k += sign_z;        
        normal = Vec3f(0, 0, -sign_z);
    } // 这部分法向量是 “进入本cell的面” 的法向量
}