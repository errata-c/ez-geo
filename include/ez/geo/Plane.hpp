#pragma once
#include <type_traits>
#include <glm/vec3.hpp>
#include <ez/math/complex.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace ez {
	// Generic N-dimensional plane. Rotation only supported for 2 and 3 dimensions.
	template<typename T, int N>
	struct Plane {
		static_assert(N > 1, "1 Dimensional planes are not supported.");
		static_assert(N < 5, "Vector types with more than 4 coorinates are not supported.");
		using vec_t = glm::vec<N, T>;

		Plane() noexcept
			: normal{ static_cast<T>(0) }
			, origin{ static_cast<T>(0) }
		{
			normal[0] = static_cast<T>(1);
		};
		Plane(const vec_t& norm, const vec_t& orig) noexcept
			: normal(norm)
			, origin(orig)
		{};

		void normalize() noexcept {
			normal = glm::normalize(normal);
		}

		void move(const vec_t& point) noexcept {
			origin = point;
		};
		void translate(const vec_t& offset) noexcept {
			origin += offset;
		};

		template<typename = std::enable_if_t<N == 3>>
		void rotate(T angle, const vec_t& axis) noexcept {
			glm::tquat<T> rot = glm::angleAxis<T>(angle, axis);
			Plane::rotate(rot);
		};
		template<typename = std::enable_if_t<N == 3>>
		void rotate(const glm::tquat<T>& rot) noexcept {
			normal = glm::rotate<T>(rot, normal);
		};

		template<typename = std::enable_if_t<N == 2>>
		void rotate(T angle) noexcept {
			glm::tcomplex<T> rot = glm::polar(angle);
			Plane::rotate(rot);
		};
		template<typename = std::enable_if_t<N == 2>>
		void rotate(const glm::tcomplex<T>& rot) noexcept {
			normal = rot.rotate(normal);
		};

		bool isFacing(const vec_t& p) const {
			T det = glm::dot(normal, p - origin);
			return det >= 0.f;
		};

		// Dont know if I like this...
		T distanceFrom(const vec_t& p) const {
			vec_t B = p - origin;
			return glm::dot(normal, B);
		};

		vec_t normal, origin;
	};

	template<typename T>
	using Plane2 = Plane<T, 2>;

	template<typename T>
	using Plane3 = Plane<T, 3>;
};