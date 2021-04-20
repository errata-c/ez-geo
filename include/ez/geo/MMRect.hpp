#pragma once
#include <glm/common.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <algorithm>
#include <ez/math/constants.hpp>

namespace ez {
	// Define a MMRect, a rect with min max points in absolute coordinates instead of one absolute and one relative.
	// Additionally the bounds of this rect a considered inclusive, meaning maximum point is inside the rect.
	template<typename T, int N>
	struct MMRect {
		static_assert(N > 1, "ez::MMRect is not defined for dimensions less than two!");
		static_assert(N <= 4, "ez::MMRect is no defined for dimensions greater than four!");

		using vec_t = glm::vec<N, T>;
		static constexpr int components = N;
		using rect_t = MMRect<T, N>;

		static constexpr bool has_x = N >= 1;
		static constexpr bool has_y = N >= 2;
		static constexpr bool has_z = N >= 3;

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

		template<typename = std::enable_if_t<has_x>>
		T width() const noexcept {
			return max[0] - min[0];
		}

		template<typename = std::enable_if_t<has_y>>
		T height() const noexcept {
			return max[1] - min[1];
		}
		template<typename = std::enable_if_t<N == 2>>
		T area() const noexcept {
			return width() * height();
		}

		template<typename = std::enable_if_t<has_z>>
		T depth() const noexcept {
			return max[2] - min[2];
		}
		template<typename = std::enable_if_t<N == 3>>
		T volume() const noexcept {
			return width() * height() * depth();
		}

		vec_t size() const noexcept {
			return max - min;
		}

		vec_t center() const noexcept {
			// Divide by two instead of multiply by 0.5, since the T variable could be an integer type.
			return (min + max) / T(2);
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
		void expand(const vec_t& minv, const vec_t & maxv) noexcept {
			min -= minv;
			max += maxv;
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
		void shrink(const vec_t& minv, const vec_t& maxv) noexcept {
			min += minv;
			max -= maxv;
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

		rect_t merged(const rect_t& other) const noexcept {
			rect_t copy = *this;
			copy.merge(other);
			return copy;
		};
		rect_t merged(const vec_t& point) const noexcept {
			rect_t copy = *this;
			copy.merge(point);
			return copy;
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
		bool contains(const glm::tvec2<F>& point) const noexcept {
			if constexpr (std::is_floating_point_v<T>) {
				for (int i = 0; i < components; ++i) {
					if ((point[i] - min[i]) < -ez::epsilon<T>()) {
						return false;
					}
					if ((max[i] - point[i]) < -ez::epsilon<T>()) {
						return false;
					}
				}
			}
			else {
				for (int i = 0; i < components; ++i) {
					if (min[i] > point[i]) {
						return false;
					}
					if (max[i] <= point[i]) {
						return false;
					}
				}
			}
			
			return true;
		};

		bool contains(const rect_t& other) const noexcept {
			if constexpr (std::is_floating_point_v<T>) {
				for (int i = 0; i < components; ++i) {
					if ((other.min[i] - min[i]) < -ez::epsilon<T>()) {
						return false;
					}
					if ((max[i] - other.max[i]) < -ez::epsilon<T>()) {
						return false;
					}
				}
			}
			else {
				for (int i = 0; i < components; ++i) {
					if (min[i] > other.min[i]) {
						return false;
					}
					if (max[i] < other.max[i]) {
						return false;
					}
				}
			}
			
			return true;
		};

		bool operator==(const rect_t& other) const noexcept {
			if constexpr (std::is_floating_point_v<T>) {
				vec_t
					tmin = glm::abs(min - other.min),
					tmax = glm::abs(max - other.max);

				for (int i = 0; i < components; ++i) {
					if (tmin[i] > ez::epsilon<T>()) {
						return false;
					}
					if (tmax[i] > ez::epsilon<T>()) {
						return false;
					}
				}
				return true;
			}
			else {
				return
					min == other.min &&
					max == other.max;
			}
		}
		bool operator!=(const rect_t& other) const noexcept {
			if constexpr (std::is_floating_point_v<T>) {
				vec_t
					tmin = glm::abs(min - other.min),
					tmax = glm::abs(max - other.max);
				for (int i = 0; i < components; ++i) {
					if (tmin[i] > ez::epsilon<T>()) {
						return true;
					}
					if (tmax[i] > ez::epsilon<T>()) {
						return true;
					}
				}
				return false;
			}
			else {
				return
					min != other.min ||
					max != other.max;
			}
		}

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