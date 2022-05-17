#ifndef AABB_H
#define AABB_H

#include "rtweekend.h"

class aabb {
public:
	aabb() {}
	aabb(const point3& a, const point3& b) {
		minimum = a;
		maximun = b;
	}

	point3 min() const { return minimum; }
	point3 max() const { return maximun; }

	bool hit(const ray& r, double t_min, double t_max) const {
		/*for (int i = 0; i < 3; i++)
		{
			double direction = 1 / ray.direction()[i];
			double t0 = (minimum[i] - ray.origin()[i]) * direction;
			double t1 = (maximun[i] - ray.origin()[i]) * direction;
			if (direction < 0)
			{
				std::swap(t0, t1);
			}
			t_min = std::max(t_min, t0);
			t_max = std::min(t_max, t1);
			if (t_min >= t_max)
			{
				return false;
			}
		}
		return true;*/
		for (int a = 0; a < 3; a++) {
			auto t0 = fmin((minimum[a] - r.origin()[a]) / r.direction()[a],
				(maximun[a] - r.origin()[a]) / r.direction()[a]);
			auto t1 = fmax((minimum[a] - r.origin()[a]) / r.direction()[a],
				(maximun[a] - r.origin()[a]) / r.direction()[a]);
			t_min = fmax(t0, t_min);
			t_max = fmin(t1, t_max);
			if (t_max <= t_min)
				return false;
		}
		return true;
	}

	point3 minimum;
	point3 maximun;
};

#endif // AABB_H

