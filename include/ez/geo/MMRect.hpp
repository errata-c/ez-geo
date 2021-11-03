#pragma once
#include <ez/meta.hpp>
#include <glm/common.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <algorithm>
#include <ez/math/constants.hpp>

// Conditionally active member functions need special care, they must be in a deduced context.
// This requires something like below where we define an int K = N in the template

namespace ez {
	namespace intern {
		template<typename vec_t>
		vec_t max(const vec_t & a, const vec_t & b) noexcept {
			if constexpr (ez::is_vec_strict_v<vec_t>) {
				return glm::max(a, b);
			}
			else {
				return std::max(a, b);
			}
		}
		template<typename vec_t>
		vec_t min(const vec_t& a, const vec_t& b) noexcept {
			if constexpr (ez::is_vec_strict_v<vec_t>) {
				return glm::min(a, b);
			}
			else {
				return std::min(a, b);
			}
		}
	}

	// Define a MMRect, a rect with min max points in absolute coordinates instead of one absolute and one relative.
	// Additionally the bounds of this rect a considered inclusive, meaning maximum point is inside the rect.
	template<typename T, int N>
	struct MMRect {
		static_assert(N > 0, "ez::MMRect requires a number of dimensions greater than zero!");
		static_assert(N <= 4, "ez::MMRect is not defined for dimensions greater than four!");
		static_assert(std::is_arithmetic_v<T>, "ez::MMRect requires an arithmetic value type!");

		using rect_t = MMRect<T, N>;
		using vec_t = std::conditional_t<N == 1, T, typename glm::vec<N, T>>;
		static constexpr int Components = N;

		MMRect() noexcept
			: min{ T(0) }
			, max{ T(0) }
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

		T width() const noexcept {
			return max[0] - min[0];
		}

		template<int K = N, typename = std::enable_if_t<(K > 1)>>
		T height() const noexcept {
			return max[1] - min[1];
		}
		template<int K = N, typename = std::enable_if_t<(K == 2)>>
		T area() const noexcept {
			return width() * height();
		}

		template<int K = N, typename = std::enable_if_t<(K > 2)>>
		T depth() const noexcept {
			return max[2] - min[2];
		}
		template<int K = N, typename = std::enable_if_t<(K == 3)>>
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

		rect_t& translate(const vec_t& offset) noexcept {
			min += offset;
			max += offset;

			return *this;
		};
		rect_t& centerTo(const vec_t& point) noexcept {
			return translate(point - center());
		};
		
		rect_t& expand(const T& amount) noexcept {
			min -= amount;
			max += amount;

			return *this;
		};
		template<int K, std::enable_if_t<(K > 1)>>
		rect_t& expand(const vec_t& amount) noexcept {
			min -= amount;
			max += amount;

			return *this;
		};
		rect_t& expand(const vec_t& minv, const vec_t & maxv) noexcept {
			min -= minv;
			max += maxv;

			return *this;
		};
		rect_t expanded(const T& amount) const noexcept {
			rect_t copy = *this;
			return copy.expand(amount);
		};
		template<int K, std::enable_if_t<(K > 1)>>
		rect_t expanded(const vec_t& amount) const noexcept {
			rect_t copy = *this;
			return copy.expand(amount);
		};
		rect_t expanded(const vec_t& minv, const vec_t& maxv) const noexcept {
			rect_t copy = *this;
			return copy.expand(minv, maxv);
		};


		rect_t& shrink(const T& amount) noexcept {
			min += amount;
			max -= amount;
			vec_t c = center();
			min = intern::max(c, min);
			max = intern::min(c, max);

			return *this;
		};
		template<int K, std::enable_if_t<(K > 1)>>
		rect_t& shrink(const vec_t& amount) noexcept {
			min += amount;
			max -= amount;
			vec_t c = center();
			min = intern::max(c, min);
			max = intern::min(c, max);

			return *this;
		};
		rect_t& shrink(const vec_t& minv, const vec_t& maxv) noexcept {
			min += minv;
			max -= maxv;
			vec_t c = center();

			min = intern::max(c, min);
			max = intern::min(c, max);

			return *this;
		};
		rect_t shrinked(const T& amount) const noexcept {
			rect_t copy = *this;
			return copy.shrink(amount);
		};
		template<int K, std::enable_if_t<(K > 1)>>
		rect_t shrinked(const vec_t& amount) const noexcept {
			rect_t copy = *this;
			return copy.shrink(amount);
		};
		rect_t shrinked(const vec_t& minv, const vec_t& maxv) const noexcept {
			rect_t copy = *this;
			return copy.shrink(minv, maxv);
		};
		

		// By default the scale origin is the center of the Rect
		template<typename F>
		rect_t& scale(const F& factor) noexcept {
			vec_t c = center();
			min = (min - c) * factor + c;
			max = (max - c) * factor + c;

			return *this;
		};
		template<typename F>
		rect_t& scale(const glm::vec<N, F>& factor) noexcept {
			vec_t c = center();
			min = (min - c) * factor + c;
			max = (max - c) * factor + c;

			return *this;
		};
		template<typename F>
		rect_t& scale(const vec_t& c, const F& factor) noexcept {
			min = (min - c) * factor + c;
			max = (max - c) * factor + c;

			return *this;
		};
		template<typename F>
		rect_t& scale(const vec_t& c, const glm::vec<N, F>& factor) noexcept {
			min = (min - c) * factor + c;
			max = (max - c) * factor + c;

			return *this;
		};
		template<typename F>
		rect_t scaled(const F& factor) const noexcept {
			rect_t copy = *this;
			return copy.scale(factor);
		};
		template<typename F>
		rect_t scaled(const glm::vec<N, F>& factor) const noexcept {
			rect_t copy = *this;
			return copy.scale(factor);
		};
		template<typename F>
		rect_t scaled(const vec_t& c, const F& factor) const noexcept {
			rect_t copy = *this;
			return copy.scale(c, factor);
		};
		template<typename F>
		rect_t scaled(const vec_t& c, const glm::vec<N, F>& factor) const noexcept {
			rect_t copy = *this;
			return copy.scale(c, factor);
		};


		rect_t& merge(const rect_t& other) noexcept {
			max = intern::max(max, other.max);
			min = intern::min(min, other.min);

			return *this;
		};
		rect_t& merge(const vec_t& point) noexcept {
			using namespace glm;

			max = intern::max(max, point);
			min = intern::min(min, point);

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
			// I'm still trying to find a better way to do this.
			// I need to be able to handle multiple components, but also single components

			if constexpr (std::is_floating_point_v<T>) {
				for (int i = 0; i < Components; ++i) {
					if ((minptr()[i] - maxptr()[i]) >= -ez::epsilon<T>()) {
						return false;
					}
				}
			}
			else {
				for (int i = 0; i < Components; ++i) {
					if (minptr()[i] >= maxptr()[i]) {
						return false;
					}
				}
			}
			return true;
		};
		
		bool contains(const vec_t& point) const noexcept {
			if constexpr (std::is_floating_point_v<T>) {
				for (int i = 0; i < Components; ++i) {
					if ((((const T*)&point)[i] - minptr()[i]) < -ez::epsilon<T>()) {
						return false;
					}
					if ((maxptr()[i] - ((const T*)&point)[i]) < -ez::epsilon<T>()) {
						return false;
					}
				}
			}
			else {
				for (int i = 0; i < Components; ++i) {
					if (minptr()[i] > ((const T*)&point)[i]) {
						return false;
					}
					if (maxptr()[i] <= ((const T*)&point)[i]) {
						return false;
					}
				}
			}
			
			return true;
		};

		bool contains(const rect_t& other) const noexcept {
			if constexpr (std::is_floating_point_v<T>) {
				for (int i = 0; i < Components; ++i) {
					if ((other.minptr()[i] - minptr()[i]) < -ez::epsilon<T>()) {
						return false;
					}
					if ((maxptr()[i] - other.maxptr()[i]) < -ez::epsilon<T>()) {
						return false;
					}
				}
			}
			else {
				for (int i = 0; i < Components; ++i) {
					if (minptr()[i] > other.minptr()[i]) {
						return false;
					}
					if (maxptr()[i] < other.maxptr()[i]) {
						return false;
					}
				}
			}
			
			return true;
		};

		bool operator==(const rect_t& other) const noexcept {
			if constexpr (std::is_floating_point_v<T>) {
				for (int i = 0; i < Components; ++i) {
					if (std::abs(minptr()[i] - other.minptr()[i]) > ez::epsilon<T>()) {
						return false;
					}
					if (std::abs(maxptr()[i] - other.maxptr()[i]) > ez::epsilon<T>()) {
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
				for (int i = 0; i < Components; ++i) {
					if (std::abs(minptr()[i] - other.minptr()[i]) > ez::epsilon<T>()) {
						return true;
					}
					if (std::abs(maxptr()[i] - other.maxptr()[i]) > ez::epsilon<T>()) {
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

		static rect_t Between(const vec_t& p0, const vec_t& p1) noexcept {
			rect_t tmp;
			tmp.max = intern::max(p0, p1);
			tmp.min = intern::min(p0, p1);
			return tmp;
		};
		static rect_t Merge(const rect_t& a, const rect_t& b) noexcept {
			rect_t tmp = a;
			return tmp.merge(b);
		};

		vec_t min, max;
	private:
		T* minptr() noexcept {
			return (T*)(&min);
		}
		T* maxptr() noexcept {
			return (T*)(&max);
		}
		const T* minptr() const noexcept {
			return (const T*)(&min);
		}
		const T* maxptr() const noexcept {
			return (const T*)(&max);
		}
	};

	template<typename T>
	using MMRect1 = MMRect<T, 1>;

	template<typename T>
	using MMRect2 = MMRect<T, 2>;

	template<typename T>
	using MMRect3 = MMRect<T, 3>;

	template<typename T>
	using MMRect4 = MMRect<T, 4>;
};