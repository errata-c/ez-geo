#pragma once
#include <glm/glm.hpp>
#include "Line2d.hpp"
#include "Line3d.hpp"
#include "Ray3d.hpp"
#include "Rect.hpp"
#include "Sphere.hpp"
#include "AABB2d.hpp"
#include "AABB3d.hpp"
#include "Plane.hpp"

namespace ez {
	// Some intersect functions only make sense for floating point usage.
	template<typename T>
	bool intersect(const Line2d<T> & l0, const Line2d<T> & l1);
	template<typename T>
	bool intersect(const Line2d<T>& l0, const Line2d<T>& l1, glm::tvec2<T>& ret);
	template<typename T>
	bool intersect(const Line2d<T>& l0, const Line2d<T>& l1, T& t0);
	template<typename T>
	bool intersect(const Line2d<T>& l0, const Line2d<T>& l1, T& t0, T& t1);

	template<typename T>
	bool intersect(const Ray3d<T> & r, const AABB3d<T>& b);
	template<typename T>
	bool intersect(const Ray3d<T>& r, const AABB3d<T>& b, T& t);
	template<typename T>
	bool intersect(const Ray3d<T>& r, const AABB3d<T>& b, glm::tvec3<T> & hit);
	// ---
	template<typename T>
	bool intersect(const AABB3d<T>& b, const Ray3d<T>& r) {
		return intersect(r, b);
	}
	template<typename T>
	bool intersect(const AABB3d<T>& b, const Ray3d<T>& r, glm::tvec3<T>& hit) {
		return intersect(r, b, hit);
	}

	template<typename T>
	bool intersect(const Ray3d<T>& r, const Plane<T>&p);
	template<typename T>
	bool intersect(const Ray3d<T>& r, const Plane<T>&p, T& t);
	template<typename T>
	bool intersect(const Ray3d<T>& r, const Plane<T>&p, glm::tvec3<T>& hit);
	// ---
	template<typename T>
	bool intersect(const Plane<float>& p, const Ray3d<float>& r) {
		return intersect(r, p);
	}
	template<typename T>
	inline bool intersect(const Plane<float>& p, const Ray3d<float>& r, glm::vec3& hit) {
		return intersect(r, p, hit);
	}

	template<typename T>
	bool intersect(const Ray3d<T>& r, const Sphere<T>&);
	template<typename T>
	bool intersect(const Ray3d<T>& r, const Sphere<T>&, T& t);
	template<typename T>
	bool intersect(const Ray3d<T>& r, const Sphere<T>&, glm::tvec3<T>& hit);

};