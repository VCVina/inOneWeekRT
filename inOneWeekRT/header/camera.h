//
//  camera.h
//  inOneWeekRT
//

#ifndef camera_h
#define camera_h

#include "rtweekend.h"

class camera {
    public:
        camera() {
            auto aspect_ratio = 16.0 / 9.0;
            auto viewport_height = 2.0;
            auto viewport_width = aspect_ratio * viewport_height;
            auto focal_length = 1.0;//摄像机参数

            origin = point3(0, 0, 0);//设定原点
            horizontal = vec3(viewport_width, 0.0, 0.0);//横向宽度
            vertical = vec3(0.0, viewport_height, 0.0);
            lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);//左下角的位置
        }

        ray get_ray(double u, double v) const {
            return ray(origin, lower_left_corner + u*horizontal + v*vertical - origin);
        }//返回以origin为起点(u,v)坐标为方向的光线

    private:
        point3 origin;
        point3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
};


#endif /* camera_h */
