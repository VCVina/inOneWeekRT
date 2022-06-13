//
//  hittable.h
//  inOneWeekRT
//

#ifndef hittable_h
#define hittable_h

#include "ray.h"
#include "rtweekend.h"

class material;

struct hit_record {
    point3 p;//返回的落点
    vec3 normal;//返回的normal
    shared_ptr<material> mat_ptr;//返回的材质信息
    double t;//撞击时的时间
    bool front_face;//射线和法线是否在同一个方向
    
    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
            front_face = dot(r.direction(), outward_normal) < 0; //光线是否直击表面,如果在表面就是true,照射表面的话点乘<0
            normal = front_face ? outward_normal :-outward_normal;//设定法线,因为落点没有正反面,我们得到的全会是正面,因此要判断一下反面
    }
};

class hittable {
    public:
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
    //const = 0的意思是const:这个函数不能修改类的成员变量./// = 0是纯虚函数,目前没有任何实现与返回,由继承类定义
};


#endif /* hittable_h */
