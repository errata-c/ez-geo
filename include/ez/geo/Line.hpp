#pragma once
#include <glm/vec2.hpp>
#include <ez/math/MathConstants.hpp>

namespace ez {
	// Generic line segment
	template<typename T, int N>
	struct Line {
		using vec_t = glm::vec<N, T>;

		Line()
			: start(static_cast<T>(0))
			, end(static_cast<T>(0))
		{}
		Line(const vec_t& p0, const vec_t& p1)
			: start(p0)
			, end(p0)
		{}

		~Line() = default;
		Line(const Line&) = default;
		Line(Line&&) noexcept = default;
		Line& operator=(const Line&) = default;
		Line& operator=(Line&&) noexcept = default;

		bool isValid() const noexcept {
			vec_t tmp = end - start;
			return glm::dot(tmp, tmp) > ez::epsilon<T>();
		}

		vec_t start, end;
	};

	template<typename T>
	using Line2 = Line<T, 2>;

	template<typename T>
	using Line3 = Line<T, 3>;
}