#pragma once
#include <ez/math/MathConstants.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace ez {
	template<typename T>
	class Ray3d {
	public:
		using vec_t = glm::tvec3<T>;

		Ray3d()
			: axis(static_cast<T>(1), static_cast<T>(0), static_cast<T>(0))
			, position(static_cast<T>(0))
		{}

		Ray3d(const vec_t& ax, const vec_t& or)
			: axis(ax)
			, position(or)
		{}

		~Ray3d() = default;
		Ray3d(const Ray3d&) = default;
		Ray3d(Ray3d&&) noexcept = default;
		Ray3d& operator=(const Ray3d&) = default;
		Ray3d& operator=(Ray3d&&) noexcept = default;
		
		vec_t eval(T t) const {
			return axis * t + position;
		}

		const vec_t& getDirection() const {
			return axis;
		}
		const vec_t& getAxis() const {
			return axis;
		}
		const vec_t& getOrigin() const {
			return position;
		}
		const vec_t& getPosition() const {
			return position;
		}

		void move(const vec_t& point) {
			position = point;
		}
		void translate(const vec_t& offset) {
			position += offset;
		}
		void rotate(T angle, const vec_t& axis) {
			glm::quat rot = glm::angleAxis(angle, axis);
			Ray3d::rotate(rot);
		}
		void rotate(const glm::quat& rot) {
			axis = glm::rotate(rot, axis);
		}

		bool isFacing(const vec_t& p) const {
			T det = glm::dot(axis, p - position);
			return det >= static_cast<T>(0);
		}

		void normalize() {
			axis = glm::normalize(axis);
		}

		// This method does not account for the start of the ray, therefore it may not perform as expected.
		T distanceFrom(const vec_t& p) const {
			vec_t op = position - p;
			return glm::length(op - glm::dot(op, axis) * axis);
		}

		vec_t nearestPointTo(const Ray3d& r) const {
			vec_t n2 = glm::cross(r.getAxis(), glm::cross(axis, r.getAxis()));

			T denom = glm::dot(axis, n2);
			if (std::abs(denom) < ez::epsilon<T>()) {
				return position;
			}
			else {
				T numer = glm::dot(r.getOrigin() - position, n2);

				return eval(numer / denom);
			}
		}
		vec_t nearestPointFrom(const Ray3d& r) const {
			vec_t n1 = glm::cross(axis, glm::cross(r.getAxis(), axis));

			T denom = glm::dot(r.getOrigin(), n1);
			if (std::abs(denom) < ez::epsilon<T>()) {
				return r.getOrigin();
			}
			else {
				T numer = glm::dot(position - r.getOrigin(), n1);

				return eval(numer / denom);
			}
		}

		vec_t nearestPointTo(const Ray3d& r, T& t) const {
			vec_t n2 = glm::cross(r.getAxis(), glm::cross(axis, r.getAxis()));

			T denom = glm::dot(axis, n2);
			if (std::abs(denom) < ez::epsilon<T>()) {
				t = static_cast<T>(0);
				return position;
			}
			else {
				T numer = glm::dot(r.getOrigin() - position, n2);
				t = numer / denom;
				return eval(t);
			}
		}
		vec_t nearestPointFrom(const Ray3d& r, T& t) const {
			vec_t n1 = glm::cross(axis, glm::cross(r.getAxis(), axis));

			T denom = glm::dot(r.getOrigin(), n1);
			if (std::abs(denom) < ez::epsilon<T>()) {
				t = static_cast<T>(0);
				return r.getOrigin();
			}
			else {
				T numer = glm::dot(position - r.getOrigin(), n1);
				t = numer / denom;
				return eval(t);
			}
		}

		static Ray3d fromPoints(const vec_t& start, const vec_t& end) {
			return Ray3d{ glm::normalize(end - start), start };
		}

		vec_t axis, position;
	};
};