#pragma once
#include <glm/vec2.hpp>
#include <algorithm>

namespace ez {
	template<typename T>
	struct AABB2d
	{
		using vec_t = glm::tvec2<T>;

		AABB2d()
			: min(static_cast<T>(0), static_cast<T>(0))
		{}
		AABB2d(const vec_t& dim)
			: min(dim / static_cast<T>(2))
		{
			min.x = -std::abs(min.x);
			min.y = -std::abs(min.y);
			max = -min;
		}
		AABB2d(const vec_t& p0, const vec_t& p1) {
			fitBetween(p0, p1);
		}

		~AABB2d() = default;
		AABB2d(const AABB2d&) = default;
		AABB2d(AABB2d&&) noexcept = default;
		AABB2d& operator=(const AABB2d&) = default;
		AABB2d& operator=(AABB2d&&) noexcept = default;

		void translate(const vec_t& offset) {
			min += offset;
			max += offset;
		}
		void centerTo(const vec_t& point) {
			translate(point - center());
		}

		template<typename F>
		void scale(const F& factor) {
			vec_t c = center();
			max = (max - c);
			min = (min - c);

			for (int i = 0; i < 2; ++i) {
				max[i] = static_cast<T>(static_cast<F>(max[i])* factor);
				min[i] = static_cast<T>(static_cast<F>(min[i])* factor);
			}
		}
		void scale(const vec_t& factor) {
			vec_t c = center();
			max = (max - c) * factor + c;
			min = (min - c) * factor + c;
		}
		template<typename F>
		void scale(const vec_t& c, const F& factor) {
			max = (max - c);
			min = (min - c);

			for (int i = 0; i < 2; ++i) {
				max[i] = static_cast<T>(static_cast<F>(max[i])* factor);
				min[i] = static_cast<T>(static_cast<F>(min[i])* factor);
			}
		}
		void scale(const vec_t& c, const vec_t& factor) {
			max = (max - c) * factor + c;
			min = (min - c) * factor + c;
		}

		void growToInclude(vec_t& point) {
			for (int i = 0; i < 2; ++i) {
				max[i] = std::max(point[i], max[i]);
				min[i] = std::min(point[i], min[i]);
			}
		}
		void growToInclude(const AABB2d& other) {
			for (int i = 0; i < 2; ++i) {
				max[i] = std::max(other.max[i], max[i]);
				min[i] = std::min(other.min[i], min[i]);
			}
		}

		void fitBetween(const vec_t& p0, const vec_t& p1) {
			for (int i = 0; i < 2; ++i) {
				max[i] = std::max(p0[i], p1[i]);
				min[i] = std::min(p0[i], p1[i]);
			}
		}

		const vec_t& minimum() const {
			return min;
		}
		const vec_t& maximum() const {
			return max;
		}

		vec_t center() const {
			return (min + max) / static_cast<T>(2);
		}

		static AABB2d between(const vec_t& p0, const vec_t& p1) {
			AABB2d tmp;
			tmp.fitBetween(p0, p1);

			return tmp;
		}
		static AABB2d originDim(const vec_t& origin, vec_t dim) {
			AABB2d tmp;
			dim.x = std::abs(dim.x);
			dim.y = std::abs(dim.y);

			tmp.min = origin - dim;
			tmp.max = origin + dim;

			return tmp;
		}

		vec_t min, max;
	};
};