#pragma once
#include <type_traits>
#include <ez/math/constants.hpp>
#include <ez/math/complex.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace ez {
	template<typename T, int N>
	class Ray {
	public:
		using vec_t = glm::vec<N, T>;

		Ray()
			: axis(static_cast<T>(0))
			, origin(static_cast<T>(0))
		{
			axis[0] = static_cast<T>(1);
		}

		Ray(const vec_t& ax, const vec_t& or)
			: axis(ax)
			, origin(or)
		{}

		~Ray() = default;
		Ray(const Ray&) = default;
		Ray(Ray&&) noexcept = default;
		Ray& operator=(const Ray&) = default;
		Ray& operator=(Ray&&) noexcept = default;
		
		vec_t eval(T t) const {
			return axis * t + origin;
		};

		void move(const vec_t& point) {
			origin = point;
		};
		void translate(const vec_t& offset) {
			origin += offset;
		};
		
		template<typename = std::enable_if_t<N == 2>>
		void rotate(T angle) {
			glm::tcomplex<T> rot = glm::polar(angle);
			Ray::rotate(rot);
		};
		template<typename = std::enable_if_t<N == 2>>
		void rotate(const glm::tcomplex<T>& rot) {
			axis = rot.rotate(axis);
		};

		template<typename = std::enable_if_t<N == 3>>
		void rotate(T angle, const vec_t& axis) {
			glm::quat rot = glm::angleAxis(angle, axis);
			Ray::rotate(rot);
		};
		template<typename = std::enable_if_t<N == 3>>
		void rotate(const glm::quat& rot) {
			axis = glm::rotate(rot, axis);
		};

		bool isFacing(const vec_t& p) const {
			T det = glm::dot(axis, p - origin);
			return det >= static_cast<T>(0);
		};

		void normalize() {
			axis = glm::normalize(axis);
		};

		// This method does not account for the start of the ray, therefore it may not perform as expected.
		T distanceFrom(const vec_t& p) const {
			vec_t op = origin - p;
			return glm::length(op - glm::dot(op, axis) * axis);
		};

		template<typename = std::enable_if_t<N == 3>>
		vec_t nearestPointTo(const Ray& r) const {
			vec_t n2 = glm::cross(r.getAxis(), glm::cross(axis, r.getAxis()));

			T denom = glm::dot(axis, n2);
			if (std::abs(denom) < ez::epsilon<T>()) {
				return origin;
			}
			else {
				T numer = glm::dot(r.getOrigin() - origin, n2);

				return eval(numer / denom);
			}
		};
		template<typename = std::enable_if_t<N == 3>>
		vec_t nearestPointFrom(const Ray& r) const {
			vec_t n1 = glm::cross(axis, glm::cross(r.getAxis(), axis));

			T denom = glm::dot(r.getOrigin(), n1);
			if (std::abs(denom) < ez::epsilon<T>()) {
				return r.getOrigin();
			}
			else {
				T numer = glm::dot(origin - r.getOrigin(), n1);

				return eval(numer / denom);
			}
		};

		template<typename = std::enable_if_t<N == 3>>
		vec_t nearestPointTo(const Ray& r, T& t) const {
			vec_t n2 = glm::cross(r.getAxis(), glm::cross(axis, r.getAxis()));

			T denom = glm::dot(axis, n2);
			if (std::abs(denom) < ez::epsilon<T>()) {
				t = static_cast<T>(0);
				return origin;
			}
			else {
				T numer = glm::dot(r.getOrigin() - origin, n2);
				t = numer / denom;
				return eval(t);
			}
		};
		template<typename = std::enable_if_t<N == 3>>
		vec_t nearestPointFrom(const Ray& r, T& t) const {
			vec_t n1 = glm::cross(axis, glm::cross(r.getAxis(), axis));

			T denom = glm::dot(r.getOrigin(), n1);
			if (std::abs(denom) < ez::epsilon<T>()) {
				t = static_cast<T>(0);
				return r.getOrigin();
			}
			else {
				T numer = glm::dot(origin - r.getOrigin(), n1);
				t = numer / denom;
				return eval(t);
			}
		};

		static Ray fromPoints(const vec_t& start, const vec_t& end) {
			return Ray{ glm::normalize(end - start), start };
		};

		vec_t axis, origin;
	};

	template<typename T>
	using Ray2 = Ray<T, 2>;

	template<typename T>
	using Ray3 = Ray<T, 3>;
};