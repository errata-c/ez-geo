#include <ez/geo/Intersect.hpp>

#include <cmath>
#include <algorithm>
#include <limits>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <glm/gtx/quaternion.hpp>

#include <ez/math/constants.hpp>
#include <ez/math/poly.hpp>

namespace ez {
	template<typename T>
	bool intersect(const Line2<T>& l1, const Line2<T>& l2) {
		glm::tvec2<T> d1 = l1.end - l1.start;
		glm::tvec2<T> d2 = l2.end - l2.start;

		T det = (d2.y * d1.x - d2.x * d1.y);

		if (std::abs(det) < ez::epsilon<T>()) {
			return false;
		}
		else {
			glm::tvec2<T> diff = l2.start - l1.start;
			T numer = (diff.x * d1.y - diff.y * d1.x);

			T t2 = numer / det;
			T t1;
			if (t2 < static_cast<T>(0) || t2 > static_cast<T>(1)) {
				return false;
			}

			if (std::abs(d1.x) < ez::epsilon<T>()) {
				t1 = (diff.y + d2.y * t2) / d1.y;
			}
			else {
				t1 = (diff.x + d2.x * t2) / d1.x;
			}

			return (t1 < static_cast<T>(0) || t1 > static_cast<T>(1));
		}
	}
	// This is very strange syntax, googled how to precompile template functions.
	template bool intersect<float>(const Line2<float>&, const Line2<float>&);
	template bool intersect<double>(const Line2<double>&, const Line2<double>&);

	template<typename T>
	bool intersect(const Line2<T>& l1, const Line2<T>& l2, glm::tvec2<T>& ret) {
		glm::tvec2<T> d1 = l1.end - l1.start;
		glm::tvec2<T> d2 = l2.end - l2.start;

		T det = (d2.y * d1.x - d2.x * d1.y);

		if (std::abs(det) < ez::epsilon<T>()) {
			return false;
		}
		else {
			glm::tvec2<T> diff = l2.start - l1.start;
			T numer = (diff.x * d1.y - diff.y * d1.x);

			T t1;
			T t2 = numer / det;
			if (t2 < static_cast<T>(0) || t2 > static_cast<T>(1)) {
				return false;
			}

			if (std::abs(d1.x) < ez::epsilon<T>()) {
				t1 = (diff.y + d2.y * t2) / d1.y;
			}
			else {
				t1 = (diff.x + d2.x * t2) / d1.x;
			}

			if (t1 < static_cast<T>(0) || t1 > static_cast<T>(1)) {
				ret = d2 * t2 + l2.start;
				return true;
			}
			else {
				return false;
			}
		}
	}
	// Precompile templates
	template bool intersect<float>(const Line2<float>&, const Line2<float>&, glm::tvec2<float>& ret);
	template bool intersect<double>(const Line2<double>&, const Line2<double>&, glm::tvec2<double>& ret);

	template<typename T>
	bool intersect(const Line2<T>& l1, const Line2<T>& l2, T& t1) {
		glm::tvec2<T> d1 = l1.end - l1.start;
		glm::tvec2<T> d2 = l2.end - l2.start;

		T det = (d2.y * d1.x - d2.x * d1.y);

		if (std::abs(det) < ez::epsilon<T>()) {
			return false;
		}
		else {
			glm::tvec2<T> diff = l2.start - l1.start;
			T numer = (diff.x * d1.y - diff.y * d1.x);

			T t2 = numer / det;
			if (t2 < static_cast<T>(0) || t2 > static_cast<T>(1)) {
				return false;
			}

			if (std::abs(d1.x) < ez::epsilon<T>()) {
				t1 = (diff.y + d2.y * t2) / d1.y;
			}
			else {
				t1 = (diff.x + d2.x * t2) / d1.x;
			}

			return (t1 < static_cast<T>(0) || t1 > static_cast<T>(1));
		}
	}
	// Precompile templates
	template bool intersect<float>(const Line2<float>&, const Line2<float>&, float& t1);
	template bool intersect<double>(const Line2<double>&, const Line2<double>&, double& t2);

	template<typename T>
	bool intersect(const Line2<T>& l1, const Line2<T>& l2, T& t1, T& t2) {
		glm::tvec2<T> d1 = l1.end - l1.start;
		glm::tvec2<T> d2 = l2.end - l2.start;

		T det = (d2.y * d1.x - d2.x * d1.y);

		if (std::abs(det) < ez::epsilon<T>()) {
			return false;
		}
		else {
			glm::tvec2<T> diff = l2.start - l1.start;
			T numer = (diff.x * d1.y - diff.y * d1.x);

			T t2 = numer / det;
			if (t2 < static_cast<T>(0) || t2 > static_cast<T>(1)) {
				return false;
			}

			if (std::abs(d1.x) < ez::epsilon<T>()) {
				t1 = (diff.y + d2.y * t2) / d1.y;
			}
			else {
				t1 = (diff.x + d2.x * t2) / d1.x;
			}

			return (t1 < static_cast<T>(0) || t1 > static_cast<T>(1));
		}
	}
	// Precompile templates
	template bool intersect<float>(const Line2<float>&, const Line2<float>&, float&, float&);
	template bool intersect<double>(const Line2<double>&, const Line2<double>&, double&, double&);


	template<typename T>
	bool intersect(const Ray3<T>& r, const AABB3<T>& b) {
		T tmin = -std::numeric_limits<T>::max(), tmax = std::numeric_limits<T>::max();

		glm::tvec3<T> inverseAxis = static_cast<T>(1) / r.axis;

		T t1 = (b.min[0] - r.origin[0]) * inverseAxis[0];
		T t2 = (b.max[0] - r.origin[0]) * inverseAxis[0];

		tmin = std::max(tmin, std::min(t1, t2));
		tmax = std::min(tmax, std::max(t1, t2));

		t1 = (b.min[1] - r.origin[1]) * inverseAxis[1];
		t2 = (b.max[1] - r.origin[1]) * inverseAxis[1];

		tmin = std::max(tmin, std::min(t1, t2));
		tmax = std::min(tmax, std::max(t1, t2));

		t1 = (b.min[2] - r.origin[2]) * inverseAxis[2];
		t2 = (b.max[2] - r.origin[2]) * inverseAxis[2];

		tmin = std::max(tmin, std::min(t1, t2));
		tmax = std::min(tmax, std::max(t1, t2));

		return tmax > std::max(tmin, static_cast<T>(0));
	}
	// Precompile templates
	template bool intersect<float>(const Ray3<float>&, const AABB3<float>&);
	template bool intersect<double>(const Ray3<double>&, const AABB3<double>&);

	template<typename T>
	bool intersect(const Ray3<T>& r, const AABB3<T>& b, T& t) {
		T tmin = -std::numeric_limits<T>::max(), tmax = std::numeric_limits<T>::max();

		glm::tvec3<T> inverseAxis = static_cast<T>(1) / r.axis;

		T t1 = (b.min[0] - r.origin[0]) * inverseAxis[0];
		T t2 = (b.max[0] - r.origin[0]) * inverseAxis[0];

		tmin = std::max(tmin, std::min(t1, t2));
		tmax = std::min(tmax, std::max(t1, t2));

		t1 = (b.min[1] - r.origin[1]) * inverseAxis[1];
		t2 = (b.max[1] - r.origin[1]) * inverseAxis[1];

		tmin = std::max(tmin, std::min(t1, t2));
		tmax = std::min(tmax, std::max(t1, t2));

		t1 = (b.min[2] - r.origin[2]) * inverseAxis[2];
		t2 = (b.max[2] - r.origin[2]) * inverseAxis[2];

		tmin = std::max(tmin, std::min(t1, t2));
		tmax = std::min(tmax, std::max(t1, t2));
		
		if (tmax > tmin) {
			if (tmin < 0.0) {
				t = tmax;
			}
			else {
				t = tmin;
			}
			return true;
		}
		return false;
	}
	// Precompile templates
	template bool intersect<float>(const Ray3<float>& r, const AABB3<float>& b, float& t);
	template bool intersect<double>(const Ray3<double>& r, const AABB3<double>& b, double& t);

	template<typename T>
	bool intersect(const Ray3<T>& r, const AABB3<T>& b, glm::tvec3<T>& hit) {
		T tmin = -std::numeric_limits<T>::max(), tmax = std::numeric_limits<T>::max();

		glm::tvec3<T> inverseAxis = static_cast<T>(1) / r.axis;

		T t1 = (b.min[0] - r.origin[0]) * inverseAxis[0];
		T t2 = (b.max[0] - r.origin[0]) * inverseAxis[0];

		tmin = std::max(tmin, std::min(t1, t2));
		tmax = std::min(tmax, std::max(t1, t2));

		t1 = (b.min[1] - r.origin[1]) * inverseAxis[1];
		t2 = (b.max[1] - r.origin[1]) * inverseAxis[1];

		tmin = std::max(tmin, std::min(t1, t2));
		tmax = std::min(tmax, std::max(t1, t2));

		t1 = (b.min[2] - r.origin[2]) * inverseAxis[2];
		t2 = (b.max[2] - r.origin[2]) * inverseAxis[2];

		tmin = std::max(tmin, std::min(t1, t2));
		tmax = std::min(tmax, std::max(t1, t2));

		if (tmax > tmin) {
			if (tmin < 0.0) {
				hit = r.eval(tmax);
			}
			else {
				hit = r.eval(tmin);
			}
			return true;
		}
		return false;
	}
	// Precompile templates
	template bool intersect<float>(const Ray3<float>& r, const AABB3<float>& b, glm::tvec3<float>& t);
	template bool intersect<double>(const Ray3<double>& r, const AABB3<double>& b, glm::tvec3<double>& t);


	template<typename T>
	bool intersect(const Ray3<T>& r, const Plane3<T>& p) {
		T numer = glm::dot(p.origin, p.normal) - glm::dot(p.normal, r.origin);
		T denom = glm::dot(r.axis, p.normal);

		if (std::abs(denom) > ez::epsilon<T>()) {
			T t = numer / denom;
			return t >= static_cast<T>(0);
		}
		return false;
	}
	// Precompile templates
	template bool intersect<float>(const Ray3<float>& r, const Plane3<float>& p);
	template bool intersect<double>(const Ray3<double>& r, const Plane3<double>& p);


	template<typename T>
	bool intersect(const Ray3<T>& r, const Plane3<T>&, T& t) {
		T numer = glm::dot(p.origin, p.normal) - glm::dot(p.normal, r.origin);
		T denom = glm::dot(r.axis, p.normal);

		if (std::abs(denom) > ez::epsilon<T>()) {
			t = numer / denom;
			return t >= static_cast<T>(0);
		}
		return false;
	}

	template<typename T>
	bool intersect(const Ray3<T>& r, const Plane3<T>&p, glm::tvec3<T>& hit) {
		T numer = glm::dot(p.origin, p.normal) - glm::dot(p.normal, r.origin);
		T denom = glm::dot(r.axis, p.normal);

		if (std::abs(denom) > ez::epsilon<T>()) {
			T t = numer / denom;
			if (t >= static_cast<T>(0)) {
				hit = r.eval(t);
				return true;
			}
			return false;
		}
		return false;
	}
	template bool intersect<float>(const Ray3<float>& r, const Plane3<float>& p, glm::tvec3<float>& hit);
	template bool intersect<double>(const Ray3<double>& r, const Plane3<double>& p, glm::tvec3<double>& hit);

	template<typename T>
	bool intersect(const Ray3<T>& r, const Sphere<T>& s) {
		glm::tvec3<T> L = r.origin - s.origin;
		T a = glm::dot(r.axis, r.axis);
		T b = static_cast<T>(2) * glm::dot(r.axis, L);
		T c = glm::dot(L, L) - s.radius * s.radius;

		T D = (b * b) + ((-static_cast<T>(4)) * a * c);

		return D >= static_cast<T>(0);
	}
	template bool intersect<float>(const Ray3<float>& r, const Sphere<float>& s);
	template bool intersect<double>(const Ray3<double>& r, const Sphere<double>& s);

	template<typename T>
	bool intersect(const Ray3<T>& r, const Sphere<T>& s, T& t) {
		glm::tvec3<T> L = r.origin - s.origin;
		T a = glm::dot(r.axis, r.axis);
		T b = static_cast<T>(2) * glm::dot(r.axis, L);
		T c = glm::dot(L, L) - s.radius * s.radius;

		T roots[2];
		int count = ez::poly::solveQuadratic<T>(a, b, c, roots);

		if (count == 2) {
			T closest = std::min(roots[0], roots[1]);
			if (closest < static_cast<T>(0)) {
				closest = roots[1];
			}
			if (closest < static_cast<T>(0)) {
				return false;
			}
			t = closest;
			return true;
		}
		else if (count == 1 && roots[0] >= static_cast<T>(0)) {
			t = roots[0];
			return true;
		}

		return false;
	}
	template bool intersect<float>(const Ray3<float>& r, const Sphere<float>& s, float& t);
	template bool intersect<double>(const Ray3<double>& r, const Sphere<double>& s, double& t);

	template<typename T>
	bool intersect(const Ray3<T>& r, const Sphere<T>& s, glm::tvec3<T>& hit) {
		glm::tvec3<T> L = r.origin - s.origin;
		T a = glm::dot(r.axis, r.axis);
		T b = static_cast<T>(2) * glm::dot(r.axis, L);
		T c = glm::dot(L, L) - s.radius * s.radius;

		T roots[2];
		int count = ez::poly::solveQuadratic<T>(a, b, c, roots);

		if (count == 2) {
			T closest = std::min(roots[0], roots[1]);
			if (closest < static_cast<T>(0)) {
				closest = roots[1];
			}
			if (closest < static_cast<T>(0)) {
				return false;
			}
			hit = r.eval(closest);
			return true;
		}
		else if (count == 1 && roots[0] >= static_cast<T>(0)) {
			hit = r.eval(roots[0]);
			return true;
		}

		return false;
	}
	template bool intersect<float>(const Ray3<float>& r, const Sphere<float>& s, glm::tvec3<float>& hit);
	template bool intersect<double>(const Ray3<double>& r, const Sphere<double>& s, glm::tvec3<double>& hit);
};