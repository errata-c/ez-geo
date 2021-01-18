#pragma once
#include <glm/vec3.hpp>

namespace ez {
	template<typename T>
	struct Sphere
	{
		using vec3_t = glm::tvec3<T>;

		Sphere(T r = static_cast<T>(1), const vec3_t& o = vec3_t{static_cast<T>(0)})
			: radius(r)
			, position(o)
		{}

		~Sphere() = default;
		Sphere(const Sphere&) = default;
		Sphere(Sphere&&) noexcept = default;
		Sphere& operator=(const Sphere&) = default;
		Sphere& operator=(Sphere&&) noexcept = default;

		void translate(const vec3_t& off) {
			position += off;
		}
		void move(const vec3_t& loc) {
			position = loc;
		}

		const vec3_t& getLocation() const {
			return position;
		}
		void setLocation(const vec3_t& loc) {
			position = loc;
		}

		const vec3_t& getOrigin() const {
			return position;
		}
		void setOrigin(const vec3_t& loc) {
			position = loc;
		}

		void scaleBy(T amount) {
			radius *= amount;
		}
		void scaleFrom(const vec3_t& point, T amount) {

		}
		void setRadius(T len) {
			radius = len;
		}
		T getRadius() const {
			return radius;
		}

		T radius;
		vec3_t position;
	};
};