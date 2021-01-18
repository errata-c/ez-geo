#pragma once
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace ez {
	template<typename T>
	struct Plane {
		using vec_t = glm::tvec3<T>;

		Plane()
			: normal{static_cast<T>(1), static_cast<T>(0), static_cast<T>(0)}
			, position{static_cast<T>(0)}
		{}
		Plane(const vec_t& norm, const vec_t& orig)
			: normal(norm)
			, position(orig)
		{}

		void normalize() {
			normal = glm::normalize(normal);
		}

		void move(const vec_t& point) {
			position = point;
		}
		void translate(const vec_t& offset) {
			position += offset;
		}
		void rotate(T angle, const vec_t& axis) {
			glm::quat rot = glm::angleAxis(angle, axis);
			Plane::rotate(rot);
		}
		void rotate(const glm::quat& rot) {
			normal = glm::rotate(rot, normal);
		}

		bool isFacing(const vec_t& p) const {
			T det = glm::dot(normal, p - position);
			return det >= 0.f;
		}

		T distanceFrom(const vec_t& p) const {
			vec_t B = p - position;
			return glm::dot(normal, B);
		}

		vec_t normal, position;
	};
};