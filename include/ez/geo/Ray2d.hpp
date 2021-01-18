#pragma once
#include <glm/vec2.hpp>
#include <ez/math/MathConstants.h>
#include <ez/math/Complex.h>

struct Ray2d {
	using T = float;
	using vec2_t = glm::tvec2<T>;

	Ray2d()
		: axis{ static_cast<T>(1), static_cast<T>(0) }
		, position{ static_cast<T>(0) }
	{}
	Ray2d(const vec2_t& ax, const vec2_t& or )
		: axis{ ax }
		, position{ or }
	{}

	~Ray2d() = default;
	Ray2d(const Ray2d&) = default;
	Ray2d(Ray2d&&) noexcept = default;
	Ray2d& operator=(const Ray2d&) = default;
	Ray2d& operator=(Ray2d&&) noexcept = default;

	vec2_t eval(T t) const {
		return axis * t + position;
	}

	const vec2_t& getDirection() const {
		return axis;
	}
	const vec2_t& getAxis() const {
		return axis;
	}
	const vec2_t& getOrigin() const {
		return position;
	}
	const vec2_t& getPosition3d() const {
		return position;
	}

	void move(const vec2_t& point) {
		position = point;
	}
	void translate(const vec2_t& offset) {
		position += offset;
	}

	void rotate(T angle) {
		Complex<T> rot = Complex<T>::fromPolar(angle);
		Ray2d::rotate(rot);
	}
	void rotate(const Complex<T>& rot) {
		axis = rot.rotate(axis);
	}

	bool isFacing(const vec2_t& p) const {
		T det = glm::dot(axis, p - position);
		return det >= static_cast<T>(0);
	}

	// This method does not account for the start of the ray, therefore it may not perform as expected.
	T distanceFrom(const vec2_t& p) const {
		vec2_t op = position - p;
		return glm::length(op - glm::dot(op, axis) * axis);
	}

	void normalize() {
		axis = glm::normalize(axis);
	}

	static Ray2d fromPoints(const vec2_t& start, const vec2_t& end) {
		return Ray2d{ glm::normalize(end - start), start };
	}

	vec2_t axis, position;
};