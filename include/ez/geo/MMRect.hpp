#pragma once
#include <glm/common.hpp>
#include <glm/vec2.hpp>
#include <algorithm>

namespace ez {
	// Define a MMRect, a rect with min max points in absolute coordinates instead of one absolute and one relative.
	// Additionally the bounds of this rect a considered inclusive, meaning maximum point is inside the rect.
	template<typename T, int N>
	struct MMRect {
		using vec_t = glm::vec<N, T>;
		static constexpr int components = N;
		using rect_t = MMRect<T, N>;

		MMRect() noexcept
			: min{ static_cast<T>(0) }
			, max{ static_cast<T>(0) }
		{};
		MMRect(const vec_t& _min, const vec_t& _max) noexcept
			: min(_min)
			, max(_max)
		{};

		template<typename U>
		MMRect(const MMRect<U, N>& other) noexcept
			: min{ other.min }
			, max{ other.max }
		{};

		~MMRect() noexcept = default;
		MMRect(const MMRect&) noexcept = default;
		MMRect(MMRect&&) noexcept = default;
		MMRect& operator=(const MMRect&) noexcept = default;
		MMRect& operator=(MMRect&&) noexcept = default;

		vec_t center() const noexcept {
			// Divide by two instead of multiply by 0.5, since the T variable could be an integer type.
			return (minimum() + maximum()) / static_cast<T>(2);
		};

		void translate(const vec_t& offset) noexcept {
			min += offset;
			max += offset;
		};
		void centerTo(const vec_t& point) noexcept {
			translate(point - center());
		};

		void expand(const T& amount) noexcept {
			min -= amount;
			max += amount;
		};
		void expand(const vec_t& amount) noexcept {
			min -= amount;
			max += amount;
		};

		void shrink(const T& amount) noexcept {
			min += amount;
			max -= amount;
			vec_t c = center();
			min = glm::max(c, min);
			max = glm::min(c, max);
		};
		void shrink(const vec_t& amount) noexcept {
			min += amount;
			max -= amount;
			vec_t c = center();
			min = glm::max(c, min);
			max = glm::min(c, max);
		};

		// By default the scale origin is the center of the Rect
		template<typename F>
		void scale(const F& factor) {
			vec_t c = center();
			min = (min - c) * factor + c;
			max = (max - c) * factor + c;
		};
		template<typename F>
		void scale(const glm::vec<N, F>& factor) {
			vec_t c = center();
			min = (min - c) * factor + c;
			max = (max - c) * factor + c;
		};
		template<typename F>
		void scale(const vec_t& c, const F& factor) {
			min = (min - c) * factor + c;
			max = (max - c) * factor + c;
		};
		template<typename F>
		void scale(const vec_t& c, const glm::vec<N, F>& factor) {
			min = (min - c) * factor + c;
			max = (max - c) * factor + c;
		};

		rect_t& merge(const rect_t& other) noexcept {
			max = glm::max(max, other.max);
			min = glm::min(min, other.min);

			return *this;
		};
		rect_t& merge(const vec_t& point) noexcept {
			max = glm::max(max, point);
			min = glm::min(min, point);

			return *this;
		};

		vec_t minimum() const noexcept {
			return min;
		};
		vec_t maximum() const noexcept {
			return max;
		};

		// This function always succeeds.
		vec_t toWorld(const vec_t& point) const noexcept {
			return point * (max - min) + min;
		};
		// Check isValid prior to calling this function if theres a chance the rect could be invalid.
		vec_t toLocal(const vec_t& point) const noexcept {
			return (point - min) / (max - min);
		};

		// Check if the assumptions made by this struct are maintained. min must always be less than max
		bool isValid() const noexcept {
			for (int i = 0; i < components; ++i) {
				if (min[i] >= max[i]) {
					return false;
				}
			}
			return true;
		};

		template<typename F>
		bool inBound(const glm::tvec2<F>& point) const noexcept {
			// If statements to have early return like the normal boolean operators.
			for (int i = 0; i < components; ++i) {
				if (point[i] < min[i]) {
					return false;
				}
				if (max[i] < point[i]) {
					return false;
				}
			}
			return true;
		};

		bool inBound(const rect_t& other) const noexcept {
			// If statements to have early return like the normal boolean operators.
			for (int i = 0; i < components; ++i) {
				if (max[i] > other.max[i]) {
					return false;
				}
				if (min[i] < other.min[i]) {
				return false;
				}
			}
			return true;
		};

		static rect_t between(const vec_t& p0, const vec_t& p1) noexcept {
			rect_t tmp;
			tmp.max = glm::max(p0, p1);
			tmp.min = glm::min(p0, p1);
			return tmp;
		};
		static rect_t merge(const rect_t& a, const rect_t& b) noexcept {
			rect_t tmp = a;
			return tmp.merge(b);
		};

		vec_t min, max;
	};

	template<typename T>
	using MMRect2 = MMRect<T, 2>;

	template<typename T>
	using MMRect3 = MMRect<T, 3>;
};