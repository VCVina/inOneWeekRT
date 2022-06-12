//
//  sphere.h
//  inOneWeekRT
//

#ifndef sphere_h
#define sphere_h

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
    public:
        sphere() {}
        sphere(point3 cen, double r) : center(cen), radius(r) {};

        virtual bool hit(//虚函数获得定义
            const ray& r, double t_min, double t_max, hit_record& rec) const override;//override表示重写父类的虚函数,见下文

    public:
        point3 center;
        double radius;
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {//在这里重写
    vec3 oc = r.origin() - center;//sphere中心指向摄像机的向量
    auto a = r.direction().length_squared();//这三个都是碰撞检测的那三个判别式项
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius*radius;

    auto discriminant = half_b*half_b - a*c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    //引入tmin和tmax的目的是为了规避光照直接招光源的bug,检查两个根是否都在t区间内部,先从小的开始,因为小的如果对那直接就是落点了.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t); //返回一个坐标:orig + rec.t*dir  rec.t就是root = (-half_b - sqrtd) / a , 碰撞点的根(解)!
    vec3 outward_normal = (rec.p - center) / radius; //ourward_normal永远是落点位置-center/半径
    rec.set_face_normal(r, outward_normal);

    return true;
}

#endif /* sphere_h */
