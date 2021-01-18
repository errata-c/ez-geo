#pragma once
#include <glm/geometric.hpp>
#include <glm/vec3.hpp>

namespace ez {
	namespace Triangle {
		// Calculates the baryocentric coordinates of a point in the triange made by a->b->c
		template<typename T>
		glm::tvec3<T> toBarycentric(glm::tvec4<T> point, glm::tvec4<T> a, glm::tvec4<T> b, glm::tvec4<T> c) {
			glm::tvec4<T> v0 = b - a, v1 = c - a, v2 = point - a;
			T d00 = glm::dot(v0, v0);
			T d01 = glm::dot(v0, v1);
			T d11 = glm::dot(v1, v1);
			T d20 = glm::dot(v2, v0);
			T d21 = glm::dot(v2, v1);
			T denom = d00 * d11 - d01 * d01;

			glm::tvec3<T> ret;
			ret.y = (d11 * d20 - d01 * d21) / denom;
			ret.z = (d00 * d21 - d01 * d20) / denom;
			ret.x = 1.0f - ret.y - ret.z;

			return ret;
		}

		// Calculates the baryocentric coordinates of a point in the triange made by a->b->c
		template<typename T>
		glm::tvec3<T> toBarycentric(glm::tvec3<T> point, glm::tvec3<T> a, glm::tvec3<T> b, glm::tvec3<T> c) {
			glm::tvec3<T> v0 = b - a, v1 = c - a, v2 = point - a;
			T d00 = glm::dot(v0, v0);
			T d01 = glm::dot(v0, v1);
			T d11 = glm::dot(v1, v1);
			T d20 = glm::dot(v2, v0);
			T d21 = glm::dot(v2, v1);
			T denom = d00 * d11 - d01 * d01;

			glm::tvec3<T> ret;
			ret.y = (d11 * d20 - d01 * d21) / denom;
			ret.z = (d00 * d21 - d01 * d20) / denom;
			ret.x = 1.0f - ret.y - ret.z;

			return ret;
		}

		// Calculates the baryocentric coordinates of a point in the triange made by a->b->c
		template<typename T>
		glm::tvec3<T> toBarycentric(glm::tvec2<T> p, glm::tvec2<T> a, glm::tvec2<T> b, glm::tvec2<T> c) {
			glm::tvec2<T> v0 = b - a, v1 = c - a, v2 = p - a;
			T d00 = glm::dot(v0, v0);
			T d01 = glm::dot(v0, v1);
			T d11 = glm::dot(v1, v1);
			T d20 = glm::dot(v2, v0);
			T d21 = glm::dot(v2, v1);
			T denom = d00 * d11 - d01 * d01;

			glm::tvec3<T> ret;
			ret.y = (d11 * d20 - d01 * d21) / denom;
			ret.z = (d00 * d21 - d01 * d20) / denom;
			ret.x = 1.0f - ret.y - ret.z;

			return ret;
		}

		// Interpolates a vector from the triangle a->b->c using the barycentric coords provided.
		template<typename T>
		glm::tvec4<T> fromBarycentric(glm::tvec3<T> coords, glm::tvec4<T> a, glm::tvec4<T> b, glm::tvec4<T> c) {
			return a * coords.x + b * coords.y + c * coords.z;
		}

		// Interpolates a vector from the triangle a->b->c using the barycentric coords provided.
		template<typename T>
		glm::tvec3<T> fromBarycentric(glm::tvec3<T> coords, glm::tvec3<T> a, glm::tvec3<T> b, glm::tvec3<T> c) {
			return a * coords.x + b * coords.y + c * coords.z;
		}

		// Interpolates a vector from the triangle a->b->c using the barycentric coords provided.
		template<typename T>
		glm::tvec2<T> fromBarycentric(glm::tvec3<T> coords, glm::tvec2<T> a, glm::tvec2<T> b, glm::tvec2<T> c) {
			return a * coords.x + b * coords.y + c * coords.z;
		}
	}
};