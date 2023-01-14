#pragma once
#include <ez/meta.hpp>
#include <glm/common.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/vec1.hpp>
#include <algorithm>
#include <ez/math/constants.hpp>

namespace ez {
	// Define a MMRect, a rect with min max points in absolute coordinates instead of one absolute and one relative.
	template<typename T, glm::length_t N, bool inclusive = false>
	struct MMRect {
		static_assert(N > 0, "ez::MMRect requires a number of dimensions greater than zero!");
		static_assert(N <= 4, "ez::MMRect is not defined for dimensions greater than four!");
		static_assert(std::is_arithmetic_v<T>, "ez::MMRect requires an arithmetic value type!");

		using self_t = MMRect<T, N>;
		using vec_t = std::conditional_t<N == 1, T, typename glm::vec<N, T>>;
		static constexpr int Components = N;

	private:
		static constexpr bool
			is_floating_point = std::is_floating_point_v<T>,
			is_vec = Components > 1;

		static const typename glm::vec<N, T>& cvt( const vec_t& val) noexcept {
			return reinterpret_cast<const typename glm::vec<N, T>&>(val);
		}
		static typename glm::vec<N, T>& cvt(vec_t& val) noexcept {
			return reinterpret_cast<typename glm::vec<N, T>&>(val);
		}

		static constexpr typename glm::vec<N, T> Eps() {
			return typename glm::vec<N, T>(ez::epsilon<T>());
		}
	public:
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
			return cvt(max)[0] - cvt(min)[0];
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

		self_t& translate(const vec_t& offset) noexcept {
			min += offset;
			max += offset;

			return *this;
		};
		self_t& center_to(const vec_t& point) noexcept {
			return translate(point - center());
		};
		
		self_t& expand(const T& amount) noexcept {
			min -= amount;
			max += amount;

			return *this;
		};
		template<int K, std::enable_if_t<(K > 1)>>
		self_t& expand(const vec_t& amount) noexcept {
			min -= amount;
			max += amount;

			return *this;
		};
		self_t& expand(const vec_t& minv, const vec_t & maxv) noexcept {
			min -= minv;
			max += maxv;

			return *this;
		};
		self_t expanded(const T& amount) const noexcept {
			self_t copy = *this;
			return copy.expand(amount);
		};
		template<int K, std::enable_if_t<(K > 1)>>
		self_t expanded(const vec_t& amount) const noexcept {
			self_t copy = *this;
			return copy.expand(amount);
		};
		self_t expanded(const vec_t& minv, const vec_t& maxv) const noexcept {
			self_t copy = *this;
			return copy.expand(minv, maxv);
		};


		self_t& contract(const vec_t& minv, const vec_t& maxv) noexcept {
			min += minv;
			max -= maxv;
			vec_t c = center();

			min = glm::min(cvt(c), cvt(min));
			max = glm::max(cvt(c), cvt(max));

			return *this;
		};
		template<int K, std::enable_if_t<(K != 1)>>
		self_t& contract(const T& amount) noexcept {
			return contract(vec_t(amount), vec_t(amount));
		};
		self_t& contract(const vec_t& amount) noexcept {
			return contract(amount, amount);
		};
		
		template<int K, std::enable_if_t<(K != 1)>>
		self_t contracted(const T& amount) const noexcept {
			self_t copy = *this;
			return copy.shrink(amount);
		};
		self_t contracted(const vec_t& amount) const noexcept {
			self_t copy = *this;
			return copy.shrink(amount);
		};
		self_t contracted(const vec_t& minv, const vec_t& maxv) const noexcept {
			self_t copy = *this;
			return copy.shrink(minv, maxv);
		};
		

		// By default the scale origin is the center of the Rect
		template<typename F>
		self_t& scale(const F& factor) noexcept {
			min *= factor;
			max *= factor;

			return *this;
		};
		template<typename F>
		self_t& scale(const glm::vec<N, F>& factor) noexcept {
			min *= factor;
			max *= factor;

			return *this;
		};
		template<typename F>
		self_t& scale(const vec_t& c, const F& factor) noexcept {
			min = (min - c) * factor + c;
			max = (max - c) * factor + c;

			return *this;
		};
		template<typename F>
		self_t& scale(const vec_t& c, const glm::vec<N, F>& factor) noexcept {
			min = (min - c) * factor + c;
			max = (max - c) * factor + c;

			return *this;
		};

		template<typename F>
		self_t scaled(const F& factor) const noexcept {
			self_t copy = *this;
			return copy.scale(factor);
		};
		template<typename F>
		self_t scaled(const glm::vec<N, F>& factor) const noexcept {
			self_t copy = *this;
			return copy.scale(factor);
		};
		template<typename F>
		self_t scaled(const vec_t& c, const F& factor) const noexcept {
			self_t copy = *this;
			return copy.scale(c, factor);
		};
		template<typename F>
		self_t scaled(const vec_t& c, const glm::vec<N, F>& factor) const noexcept {
			self_t copy = *this;
			return copy.scale(c, factor);
		};


		self_t& merge(const self_t& other) noexcept {
			cvt(max) = glm::max(cvt(max), cvt(other.max));
			cvt(min) = glm::min(cvt(min), cvt(other.min));

			return *this;
		};
		self_t& merge(const vec_t& point) noexcept {
			cvt(max) = glm::max(cvt(max), cvt(point));
			cvt(min) = glm::min(cvt(min), cvt(point));

			return *this;
		};

		self_t merged(const self_t& other) const noexcept {
			self_t copy = *this;
			copy.merge(other);
			return copy;
		};
		self_t merged(const vec_t& point) const noexcept {
			self_t copy = *this;
			copy.merge(point);
			return copy;
		};

		// This function always succeeds.
		vec_t to_world(const vec_t& point) const noexcept {
			return point * (max - min) + min;
		};
		// Check isValid prior to calling this function if theres a chance the rect could be invalid.
		vec_t to_local(const vec_t& point) const noexcept {
			return (point - min) / (max - min);
		};
		
		// When inclusive, the rect is invalid when max is less than min.
		// When exclusive, the rect is invalid when max is less than or equal to min.
		bool valid() const noexcept {
			if constexpr (is_floating_point) {
				if constexpr (inclusive) {
					return glm::all(glm::greaterThanEqual(
						cvt(max) - cvt(min),
						-Eps()
					));
				}
				else {
					return glm::all(glm::greaterThanEqual(
						cvt(max) - cvt(min),
						Eps()
					));
				}
			}
			else {
				if  constexpr (inclusive) {
					return glm::all(glm::greaterThanEqual(cvt(max), cvt(min)));
				}
				else {
					return glm::all(glm::greaterThan(cvt(max), cvt(min)));
				}
			}
		};
		bool invalid() const noexcept {
			return !valid();
		};
		
		bool contains(const vec_t& point) const noexcept {
			if constexpr (inclusive) {
				if constexpr (is_floating_point) {
					return
						glm::all(glm::lessThanEqual(cvt(min) - cvt(point), Eps())) &&
						glm::all(glm::greaterThanEqual(cvt(max) - cvt(point), -Eps()));
				}
				else {
					return
						glm::all(glm::lessThanEqual(cvt(min), cvt(point))) &&
						glm::all(glm::greaterThanEqual(cvt(max), cvt(point)));
				}
			}
			else {
				if constexpr (is_floating_point) {
					return
						glm::all(glm::lessThanEqual(cvt(min) - cvt(point), Eps())) &&
						glm::all(glm::greaterThanEqual(cvt(max) - cvt(point), Eps()));
				}
				else {
					return
						glm::all(glm::lessThanEqual(cvt(min), cvt(point))) &&
						glm::all(glm::greaterThan(cvt(max), cvt(point)));
				}
			}
		};

		bool contains(const self_t& other) const noexcept {
			if constexpr (is_floating_point) {
				return !(
					glm::any(glm::greaterThan(cvt(min) - cvt(other.min), Eps())) ||
					glm::any(glm::lessThan(cvt(max) - cvt(other.max), -Eps()))
				);
			}
			else {
				return !(
					glm::any(glm::greaterThan(cvt(min), cvt(other.min))) ||
					glm::any(glm::lessThan(cvt(max), cvt(other.max)))
				);
			}
		};

		bool operator==(const self_t& other) const noexcept {
			if constexpr (is_floating_point) {
				return
					glm::all(glm::lessThan(glm::abs(cvt(min) - cvt(other.min)), Eps())) &&
					glm::all(glm::lessThan(glm::abs(cvt(max) - cvt(other.max)), Eps()));
			}
			else {
				return
					min == other.min &&
					max == other.max;
			}
		}
		bool operator!=(const self_t& other) const noexcept {
			if constexpr (is_floating_point) {
				return 
					glm::any(glm::greaterThan(glm::abs(cvt(min) - cvt(other.min)), Eps())) ||
					glm::any(glm::greaterThan(glm::abs(cvt(max) - cvt(other.max)), Eps()));
			}
			else {
				return
					min != other.min ||
					max != other.max;
			}
		}

		static self_t Between(const vec_t& p0, const vec_t& p1) noexcept {
			self_t tmp;
			cvt(tmp.max) = glm::max(cvt(p0), cvt(p1));
			cvt(tmp.min) = glm::min(cvt(p0), cvt(p1));
			return tmp;
		};
		static self_t Merge(const self_t& a, const self_t& b) noexcept {
			self_t tmp = a;
			return tmp.merge(b);
		};

		vec_t min, max;
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