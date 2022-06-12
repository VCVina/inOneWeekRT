//
//  ray.h
//  inOneWeekRT
//

#ifndef ray_h
#define ray_h
#include "vec3.h"

class ray {
    public:
        ray() {}
        ray(const point3& origin, const vec3& direction)//光线的起点和方向
            : orig(origin), dir(direction)
        {}

        point3 origin() const  { return orig; }//返回vec3起点
        vec3 direction() const { return dir; }//返回vec3方向向量

        point3 at(double t) const {
            return orig + t*dir;
        }
    public:
        point3 orig;
        vec3 dir;
};

#endif /* ray_h */
