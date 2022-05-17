#ifndef BVH_H
#define BVH_H

bool box_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axis);
bool box_x_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b);
bool box_y_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b);
bool box_z_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b);

#include "rtweekend.h"
#include "hittable.h"
#include "hittable_list.h"
#include <algorithm>




class bvh_node : public hittable {
public:
	bvh_node() {}
	bvh_node(const hittable_list& list):bvh_node(list.objects, 0, list.objects.size()) {}
	bvh_node(const std::vector<shared_ptr<hittable>>& src_objects, size_t start, size_t end) {
		size_t object_span = end - start;
		int axis = random_int(0, 2);
		auto objects = src_objects;
		auto comparator = axis == 0 ? box_x_compare : axis == 1 ? box_y_compare : box_z_compare;
		if (object_span == 1)
		{
			left = right = objects[start];
		}else if (object_span == 2) {
			if (box_compare(objects[start], objects[start + 1], axis))
			{
				left = objects[start];
				right = objects[start + 1];
			}
			else {
				left = objects[start + 1];
				right = objects[start];
			}
		}
		else {
			std::sort( objects.begin() + start, objects.begin() + end, comparator);
			auto mid = start + object_span / 2;
			left = make_shared<bvh_node>(objects, start, mid);
			right = make_shared<bvh_node>(objects, mid, end);
		}
		aabb box_left, box_right;
		box = surroundingbox(box_left, box_right);
	}
	virtual bool hit(const ray& ray, double t_min, double t_max, hit_record& rec) const override;
	virtual bool boundingbox(aabb& out) const override;

public:
	shared_ptr<hittable> left;
	shared_ptr<hittable> right;
	aabb box;
};

bool bvh_node:: boundingbox(aabb& out) const {
	out = box;
	return true;
}

bool bvh_node::hit(const ray& ray, double t_min, double t_max, hit_record& rec) const {
	if (!box.hit(ray, t_min, t_max)) return false;
	auto hitleft = left->hit(ray, t_min, t_max, rec);
	auto hitright = right->hit(ray, t_min, hitleft? rec.t: t_max, rec);
	return hitleft || hitright;
}

inline bool box_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axis) {
	aabb box_a;
	aabb box_b;
	return box_a.min().e[axis] < box_b.min().e[axis];
}

bool box_x_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
	return box_compare(a, b, 0);
}

bool box_y_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
	return box_compare(a, b, 1);
}

bool box_z_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
	return box_compare(a, b, 2);
}

#endif // !BVH_H
