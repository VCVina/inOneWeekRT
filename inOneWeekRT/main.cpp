#ifndef COLOR_H
#define COLOR_H

#include "rtweekend.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"

#include "ray.h"
#include "camera.h"
#include "vec3.h"

#include <iostream>
#include <fstream>
using namespace std;

color ray_color(const ray& r, const hittable& world, int depth/*最大递归次数*/) {
    if (depth <= 0)
            return color(0,0,0);
    hit_record rec;
    if (world.hit(r, 0.001, infinity, rec)) {//之所以是0.001不是0是因为我们想忽略因为浮点导致非常接进0但实际上并不是反射的hit
        point3 target = rec.p + rec.normal +  random_in_unit_sphere();//计算那个单位圆内随机点的位置,但这个是一个绝对位置.记住加定义域vec3::
        return 0.8 * ray_color(ray(rec.p, target - rec.p), world,depth-1);//每次反弹只吸收0.5的能量,其实还太保守了
    }
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*color(0.5, 0.5, 0.5) + t*color(0.5, 0.7, 1.0);
}

double hit_sphere(const point3& center, double radius, const ray& r){
    vec3 oc = r.origin() - center;//光起点-圆中心(A-C)
    auto a = r.direction().length_squared();//这三个都是公式的分别每个项,r.direction()就是b from判别式,length_sq就是自乘.
    auto half_b = dot(oc,r.direction());//b = 2h
    auto c = oc.length_squared() - radius*radius;
    auto discriminant = half_b*half_b - a*c;//判别式
    return discriminant>0?((- half_b - sqrt(discriminant) ) / a):-1.0;
}

color ray_color(const ray& r){
    auto t = hit_sphere(point3(0,0,-1), 0.5, r);
    if (t > 0.0) {
        vec3 N = unit_vector(r.at(t) - vec3(0,0,-1));//创建对应的法线(接触点-中心)
        return 0.5*color(N.x()+1, N.y()+1, N.z()+1);//+1的目的是因为让值是正数
    }
    
    //background
    vec3 unit_direction = unit_vector(r.direction());
    t = 0.5*(unit_direction.y() + 1.0);//线性插值中的t
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);//线性插值,StartValue -> EndValue
//    return color(1,1,1);
}

int main() {
    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    ofstream outImage;
    outImage.open("image.ppm");
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);//w除以wh比
    const int samples_per_pixel = 30;//spp次数
    const int max_depth = 50;//最大最大的递归次数
    
    // World
    hittable_list world;
    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));

    // Camera
    camera cam;
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;//这东西是焦距，和焦点距离还不一样
    
    auto origin = point3(0, 0, 0);//摄像机坐标
    auto horizontal = vec3(viewport_width, 0, 0);//vieport的横向宽度以向量表示
    auto vertical = vec3(0, viewport_height, 0);//vieport的纵向高度以向量表示
    auto lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);//见note
    //注意这个公式的逻辑，别忘记朝向我们才是正，所以这样没问题
    // Render
    outImage << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) {//逐像素1
        cerr << "\rScanlines remaining: " << j << ' ' << flush;
        for (int i = 0; i < image_width; ++i) {//逐像素2
            color pixel_color(0, 0, 0);
                for (int s = 0; s < samples_per_pixel; ++s) {
                    auto u = (i + random_double()) / (image_width-1);//横向的比例u坐标,为什么要-1?为了让u能够在最后==1(i最大值为h-1))
                    auto v = (j + random_double()) / (image_height-1);
                    ray r = cam.get_ray(u, v);
                    pixel_color += ray_color(r, world, max_depth);
                }
            write_color(outImage, pixel_color, samples_per_pixel);
        }
    }
    outImage.close();
    cerr << "\nDone.\n";
    return 0;
}

 #endif