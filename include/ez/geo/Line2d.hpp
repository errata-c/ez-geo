#pragma once
#include <glm/vec2.hpp>

namespace ez {
	template<typename T>
	struct Line2d {
		using vec_t = glm::tvec2<T>;

		Line2d()
			: start(static_cast<T>(0))
			, end(static_cast<T>(0))
		{}
		Line2d(const vec_t& p0, const vec_t& p1)
			: start(p0)
			, end(p0)
		{}

		~Line2d() = default;
		Line2d(const Line2d&) = default;
		Line2d(Line2d&&) noexcept = default;
		Line2d& operator=(const Line2d&) = default;
		Line2d& operator=(Line2d&&) noexcept = default;

		vec_t start, end;
	};
}