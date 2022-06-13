#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h"

struct hit_record;

class material {
    public:
        virtual bool scatter(//散射虚函数,每个材质自己定义去吧
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const = 0;
};

class lambertian: public material {
public:
    lambertian(const color& a): albedo(a){
        
    }
    virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override{
        auto scatter_direction = rec.normal + random_unit_vector();//法线加三维-1到1的随机单位化向量,但是没有定起点
        // 在材质中,如果我们的散射方向和法线方向想加刚好为0,会有不好的事情发生
        if (scatter_direction.near_zero())
            scatter_direction = rec.normal;//这种情况我们就让它沿着法线散射吧
        scattered = ray(rec.p, scatter_direction);//起点是p,终点是散射方向
        attenuation = albedo;//颜色是albedo
        return true;
    }
public:
    color albedo;
};

class metal : public material {
    public:
        metal(const color& a) : albedo(a) {}

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const override {
            vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
            scattered = ray(rec.p, reflected);
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);//返回出射方向和normal的点乘是否大于0?
        }

    public:
        color albedo;
};


#endif
