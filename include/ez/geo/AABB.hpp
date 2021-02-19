#pragma once
#include "MMRect.hpp"

namespace ez {
	template<typename T, int N>
	using AABB = MMRect<T, N>;

	template<typename T>
	using AABB2 = MMRect<T, 2>;

	template<typename T>
	using AABB3 = MMRect<T, 3>;
};