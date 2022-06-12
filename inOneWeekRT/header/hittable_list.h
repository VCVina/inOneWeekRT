#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>

using std::shared_ptr;//智能指针和普通指针的用法是相似的，不同之处在于，智能指针可以在适当时机自动释放分配的内存
using std::make_shared;
//存储hittables的list
class hittable_list : public hittable {
    public:
        hittable_list() {}
        hittable_list(shared_ptr<hittable> object) { add(object); }//将hittables加入list

        void clear() { objects.clear(); }
        void add(shared_ptr<hittable> object) { objects.push_back(object); }//压vector呗

        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;

    public:
        std::vector<shared_ptr<hittable>> objects;
};

bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    hit_record temp_rec;//落点,normal,撞机时间
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& object : objects) {//获取objects里面的所有object
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

#endif
