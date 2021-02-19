#pragma once
#include <ez/math/MathConstants.hpp>
#include <ez/math/Complex.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/vec3.hpp>

namespace ez {
	namespace geo {
		template<typename T, int N>
		glm::vec<N, T> worldX() noexcept {
			glm::vec<N, T> res{ T(0) };
			res[0] = T(1);
			return res;
		};
		template<typename T, int N>
		glm::vec<N, T> worldY() noexcept {
			glm::vec<N, T> res{ T(0) };
			res[1] = T(1);
			return res;
		};
		template<typename T, int N>
		glm::vec<N, T> worldZ() noexcept {
			static_assert(N == 3, "Need at least 3 dimensions to have a z coord.");
			return glm::vec<N, T>{T(0), T(0), T(1)};
		};

		// In these functions, the first few reference values are the return values.

		template<typename T>
		glm::tquat<T> makeRotation(const glm::tvec3<T>& from, const glm::tvec3<T>& to) noexcept {
			using vec_t = glm::vec<3, T>;
			using quat_t = glm::tquat<T>;

			quat_t rot;
			T det = glm::dot(v1, v2);
			if (det >= eps1) {
				rot = quat_t{};
			}
			else if (det <= -eps1) {
				rot = glm::angleAxis(ez::pi<T>(), vec3_t{ static_cast<T>(1), static_cast<T>(0), static_cast<T>(0) });
			}
			else {
				T s = std::sqrt((static_cast<T>(1) + det) * static_cast<T>(2));
				T invs = static_cast<T>(1) / s;
				vec3_t c = glm::cross(v1, v2);
				rot = {
					s / static_cast<T>(2),
					c * invs
				};
				rot = glm::normalize(rot);
			}
			return rot;
		};
		template<typename T>
		ez::Complex<T> makeRotation(const glm::tvec2<T>& from, const glm::tvec2<T>& to) noexcept {
			ez::Complex<T> r0{ from.x, from.y }, r1{ to.x, to.y };
			return r0.reverseRotate(r1);
		};

		template<typename T>
		void rotate(glm::tvec3<T>& val, const glm::tvec3<T>& axis, T angle) noexcept {
			glm::tquat<T> rot = glm::angleAxis(angle, axis);
			return rotate(val, rot);
		};
		template<typename T>
		void rotate(glm::tvec3<T>& val, const glm::tquat<T>& rot) noexcept {
			val = glm::rotate(val, rot);
		};

		template<typename T>
		void rotate(glm::tvec2<T>& val, T angle) noexcept {
			ez::Complex<T> rot = ez::Complex<T>::fromPolar(angle);
			val = rot.rotate(val);
		};
		template<typename T>
		void rotate(glm::tvec2<T>& val, const ez::Complex<T>& rot) noexcept {
			val = rot.rotate(val);
		}


	};
};