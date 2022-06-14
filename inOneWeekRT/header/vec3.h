//
//  vec3.h
//  inOneWeekRT
//

#ifndef vec3_h
#define vec3_h
#include <cmath>
#include <iostream>
#include "rtweekend.h"
using std::sqrt;

class vec3 {
    public:
        vec3() : e{0,0,0} {}
        vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

        double x() const { return e[0]; }
        double y() const { return e[1]; }
        double z() const { return e[2]; }

        vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
        double operator[](int i) const { return e[i]; }
        double& operator[](int i) { return e[i]; }

        vec3& operator+=(const vec3 &v) {
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            return *this;
        }

        vec3& operator*=(const double t) {
            e[0] *= t;
            e[1] *= t;
            e[2] *= t;
            return *this;
        }

        vec3& operator/=(const double t) {
            return *this *= 1/t;
        }

        double length() const {
            return sqrt(length_squared());
        }

        double length_squared() const {
            return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
        }
        bool near_zero() const {
            //在材质中,如果我们的散射方向和法线方向想加刚好为0,会有不好的事情发生
            const auto s = 1e-8;
            return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
        }
        inline static vec3 random() {
            return vec3(random_double(), random_double(), random_double());//返回三个clamp到1的随机数
        }

        inline static vec3 random(double min, double max) {//返回三个clamp到min max的随机数,为了方便取样吧
            return vec3(random_double(min,max), random_double(min,max), random_double(min,max));
        }

    public:
        double e[3];
    
    // vec3 Utility Functions
    //operator overload
    
};



inline std::ostream& operator<<(std::ostream &out, const vec3 &v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double t, const vec3 &v) {
    return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline vec3 operator*(const vec3 &v, double t) {
    return t * v;
}

inline vec3 operator/(vec3 v, double t) {
    return (1/t) * v;
}

inline double dot(const vec3 &u, const vec3 &v) {
    return u.e[0] * v.e[0]
         + u.e[1] * v.e[1]
         + u.e[2] * v.e[2];
}


inline vec3 cross(const vec3 &u, const vec3 &v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(vec3 v) {//归一化函数
    return v / v.length();
}
vec3 random_in_unit_sphere() {
    while (true) { //返回p = -1到1的随机数
        auto p = vec3::random(-1,1);
        if (p.length_squared() >= 1) continue;//这是为了防止浮点误差
        return p;
    }
}
vec3 random_unit_vector() {//单位化rius
    return unit_vector(random_in_unit_sphere());
}
vec3 random_in_hemisphere(const vec3& normal) {
    vec3 in_unit_sphere = random_in_unit_sphere();
    if (dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
        return in_unit_sphere;
    else
        return -in_unit_sphere;//和法线相乘如果是负数就真回来
}
vec3 reflect(const vec3& v, const vec3& n) {//镜面反射函数:R=I-2(I\dotN)N,I是入射光向量(不一定是单位向量),N是单位法线.
    return v - 2*dot(v,n)*n;
}
vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {//这里的uv输入的是unit_direction也就是入射单位方向向量
    auto cos_theta = fmin(dot(-uv, n), 1.0);//因此cos是入射和n的cos值
    vec3 r_out_perp =  etai_over_etat * (uv + cos_theta*n);//etai_over_etat就是折射率eta入射和eta出射的比值,为什么uv是R?
    vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;//c^2+s^2=1所以
    return r_out_perp + r_out_parallel;
}
// Type aliases for vec3
using point3 = vec3;   // 3D point alias
using color = vec3;    // RGB color

#endif /* vec3_h */
