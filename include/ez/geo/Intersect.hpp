#pragma once
#include <glm/glm.hpp>
#include "Line.hpp"
#include "Ray.hpp"
#include "Rect.hpp"
#include "Sphere.hpp"
#include "AABB.hpp"
#include "Plane.hpp"

namespace ez {
	// Most intersect functions only make sense for floating point usage.
	template<typename T>
	bool intersect(const Line2<T> & l0, const Line2<T> & l1);
	template<typename T>
	bool intersect(const Line2<T>& l0, const Line2<T>& l1, glm::tvec2<T>& ret);
	template<typename T>
	bool intersect(const Line2<T>& l0, const Line2<T>& l1, T& t0);
	template<typename T>
	bool intersect(const Line2<T>& l0, const Line2<T>& l1, T& t0, T& t1);

	template<typename T>
	bool intersect(const Ray3<T> & r, const AABB3<T>& b);
	template<typename T>
	bool intersect(const Ray3<T>& r, const AABB3<T>& b, T& t);
	template<typename T>
	bool intersect(const Ray3<T>& r, const AABB3<T>& b, glm::tvec3<T> & hit);
	// ---
	template<typename T>
	bool intersect(const AABB3<T>& b, const Ray3<T>& r) {
		return intersect(r, b);
	}
	template<typename T>
	bool intersect(const AABB3<T>& b, const Ray3<T>& r, glm::tvec3<T>& hit) {
		return intersect(r, b, hit);
	}

	template<typename T>
	bool intersect(const Ray3<T>& r, const Plane3<T>&p);
	template<typename T>
	bool intersect(const Ray3<T>& r, const Plane3<T>&p, T& t);
	template<typename T>
	bool intersect(const Ray3<T>& r, const Plane3<T>&p, glm::tvec3<T>& hit);
	// ---
	template<typename T>
	bool intersect(const Plane3<float>& p, const Ray3<float>& r) {
		return intersect(r, p);
	}
	template<typename T>
	inline bool intersect(const Plane3<float>& p, const Ray3<float>& r, glm::vec3& hit) {
		return intersect(r, p, hit);
	}

	template<typename T>
	bool intersect(const Ray3<T>& r, const Sphere<T>&);
	template<typename T>
	bool intersect(const Ray3<T>& r, const Sphere<T>&, T& t);
	template<typename T>
	bool intersect(const Ray3<T>& r, const Sphere<T>&, glm::tvec3<T>& hit);

};