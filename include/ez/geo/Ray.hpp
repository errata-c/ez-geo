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
		using vec_t = typename glm::vec<N, T>;

		Ray() noexcept
		{
			axis = vec_t{T(0)};
			origin = vec_t{T(0)};
			axis[0] = static_cast<T>(1);
		}

		Ray(const vec_t& ax, const vec_t& og) noexcept
		{
			axis = ax;
			origin = og;
		}

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

		bool isFacing(const vec_t& p) const {
			T det = glm::dot(axis, p - origin);
			return det >= static_cast<T>(0);
		};

		void normalize() {
			axis = glm::normalize(axis);
		};

		vec_t nearestPointTo(const Ray<T, N>& r, T& t) const {
			vec_t n2 = glm::cross(r.axis, glm::cross(axis, r.axis));

			T denom = glm::dot(axis, n2);
			if (std::abs(denom) < ez::epsilon<T>()) {
				t = static_cast<T>(0);
				return origin;
			}
			else {
				T numer = glm::dot(r.origin - origin, n2);
				t = numer / denom;
				return eval(t);
			}
		};
		vec_t nearestPointFrom(const Ray<T, N>& r, T& t) const {
			vec_t n1 = glm::cross(axis, glm::cross(r.axis, axis));
			
			T denom = glm::dot(r.origin, n1);
			if (std::abs(denom) < ez::epsilon<T>()) {
				t = static_cast<T>(0);
				return r.origin;
			}
			else {
				T numer = glm::dot(origin - r.origin, n1);
				t = numer / denom;
				return eval(t);
			}
		};

		// This method does not account for the start of the ray, therefore it may not perform as expected.
		T distanceFrom(const vec_t& p) const {
			vec_t op = origin - p;
			return glm::length(op - glm::dot(op, axis) * axis);
		};

		static Ray<T, N> fromPoints(const vec_t& start, const vec_t& end) {
			return Ray<T, N>{ glm::normalize(end - start), start };
		};

		template<int K = N, typename = std::enable_if_t<(K == 2)>>
		void rotate(T angle) {
			glm::tcomplex<T> rot = glm::polar(angle);
			rotate(rot);
		};
		template<int K = N, typename = std::enable_if_t<(K == 2)>>
		void rotate(const glm::tcomplex<T>& rot) {
			axis = rot.rotate(axis);
		};

		template<int K = N, typename = std::enable_if_t<(K == 3)>>
		void rotate(T angle, const vec_t& axis) {
			glm::quat rot = glm::angleAxis(angle, axis);
			rotate(rot);
		};
		template<int K = N, typename = std::enable_if_t<(K == 3)>>
		void rotate(const glm::quat& rot) {
			axis = glm::rotate(rot, axis);
		};

		vec_t axis, origin;
	};

	template<typename T>
	using Ray2 = Ray<T, 2>;

	template<typename T>
	using Ray3 = Ray<T, 3>;
};