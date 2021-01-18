#pragma once
#include <glm/vec3.hpp>

namespace ez {
	template<typename T>
	struct Line3d {
		using vec_t = glm::tvec3<T>;

		Line3d()
			: start(static_cast<T>(0))
			, end(static_cast<T>(0))
		{}
		Line3d(const vec_t& p0, const vec_t& p1)
			: start(p0)
			, end(p0)
		{}

		~Line3d() = default;
		Line3d(const Line3d&) = default;
		Line3d(Line3d&&) noexcept = default;
		Line3d& operator=(const Line3d&) = default;
		Line3d& operator=(Line3d&&) noexcept = default;

		vec_t start, end;
	};
};