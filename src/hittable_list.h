#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable {
public:
    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) { add(object); }

    void clear() { objects.clear(); }
    void add(shared_ptr<hittable> object) { objects.push_back(object); }

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
    virtual bool boundingbox(aabb& out) const override;

public:
    std::vector<shared_ptr<hittable>> objects;
};

bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& object : objects) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

aabb surroundingbox(aabb& a, aabb& b) {
    point3 min_P(
        fmin(a.min().x(), b.min().x()),
        fmin(a.min().y(), b.min().y()),
        fmin(a.min().z(), b.min().z())
    );
    point3 max_p(
        fmax(a.max().x(), b.max().x()),
        fmax(a.max().y(), b.max().y()),
        fmax(a.max().z(), b.max().z())
    );
    return aabb(min_P, max_p);
}

bool hittable_list::boundingbox(aabb& out) const {
    if (objects.empty()) return false;
    aabb tempbox;
    bool first_box = true;
    for (auto& obj : objects)
    {
        if (!obj->boundingbox(tempbox)) return false;
        out = first_box ? tempbox : surroundingbox(out, tempbox);
        first_box = false;
    }
    return true;
}



#endif