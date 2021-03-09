#pragma once
#include <array>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat2x2.hpp>
#include <ez/math/complex.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace ez::intern {
	template<typename T, int N>
	struct DimTraits {};

	template<typename T>
	struct DimTraits<T, 2> {
		using vec_t = glm::tvec2<T>;
		using rot_t = glm::tcomplex<T>;
		using basis_t = glm::tmat2x2<T>;
		using matrix_t = glm::tmat3x3<T>;
		static constexpr std::array<vec_t, 2> world = { vec_t{T(1), T(0)}, vec_t{T(0), T(1)} };

		static constexpr rot_t defaultRotation() {
			return rot_t{T(1), T(0)};
		}
	};

	template<typename T>
	struct DimTraits<T, 3> {
		using vec_t = glm::tvec3<T>;
		using rot_t = glm::tquat<T>;
		using basis_t = glm::tmat3x3<T>;
		using matrix_t = glm::tmat4x4<T>;
		static constexpr std::array<vec_t, 3> world = { vec_t{T(1), T(0), T(0)}, vec_t{T(0), T(1), T(0)}, vec_t{T(0), T(0), T(1)} };

		static constexpr rot_t defaultRotation() {
			return rot_t{ T(1), T(0), T(0), T(0) };
		}
	};
}