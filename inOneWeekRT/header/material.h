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
        metal(const color& a, double f) : albedo(a), fuzz(f<1?f:1) {}//冒号表示后面是初始化列表,输入a为a,f为fuzz(如果f<1就是f,如果大于1就是1)
    
        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const override {
            vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
//            scattered = ray(rec.p, reflected);这是镜面反射,我们可以加入一个控制反射磨砂感的参数
            scattered = ray(rec.p, reflected+fuzz*random_in_unit_sphere());//让反射方向出现一点变化
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);//返回出射方向和normal的点乘是否大于0?
        }

    public:
        color albedo;
        double fuzz;
};

class dielectric : public material {//玻璃等电介质
    public:
        dielectric(double index_of_refraction) : ir(index_of_refraction) {}

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const override {
            attenuation = color(1.0, 1.0, 1.0);
            double refraction_ratio = rec.front_face ? (1.0/ir) : ir; //就是etai_over_etat,内外折射率的比值

            vec3 unit_direction = unit_vector(r_in.direction());
            vec3 refracted = refract(unit_direction, rec.normal, refraction_ratio);

            scattered = ray(rec.p, refracted);
            return true;
        }

    public:
        double ir; // Index of Refraction
};
#endif
