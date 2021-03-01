#pragma once
#include <cinttypes>
#include <typetraits>
#include <ez/math/constants.hpp>
#include <cmath>
#include <string_view>
#include <cassert>

namespace ez {
	namespace intern {
		// Convert from T to integral type U
		template<typename T, typename U, typename = std::enable_if_t<std::is_integral_v<U>>>
		U remapColor(const T & value) noexcept {
			if constexpr (std::is_floating_point_v<T>) {
				return value * U(255);
			}
			else {
				return static_cast<U>(value);
			}
		}

		// Convert from T to floating point type U
		template<typename T, typename U, typename = std::enable_if_t<std::is_floating_point_v<U>>>
		U remapColor(const T & value) noexcept {
			if constexpr (std::is_floating_point_v<T>) {
				return static_cast<U>(value);
			}
			else {
				return value / U(255);
			}
		}
	}


	template<typename T>
	struct Color {
		static_assert(
			std::is_same_v<T, uint8_t> ||
			std::is_same_v<T, float> ||
			std::is_same_v<T, double> ||
			std::is_same_v<T, long double>,
			"Unsupported value type for ez::Color!"
		);

		static constexpr T maxval = std::is_same_v<T, uint8_t> ? T(255) : T(1);

		// ARGB
		static Color fromU32(uint32_t value) noexcept {
			Color tmp;
			tmp.b = intern::remapColor<uint8_t, T>(value & 0xFF);
			value >>= 8;
			tmp.g = intern::remapColor<uint8_t, T>(value & 0xFF);
			value >>= 8;
			tmp.r = intern::remapColor<uint8_t, T>(value & 0xFF);
			value >>= 8;
			tmp.a = intern::remapColor<uint8_t, T>(value & 0xFF);
			return tmp;
		}
		// ARGB
		static uint32_t toU32(const Color& value) noexcept {
			uint32_t tmp = intern::remapColor<T, uint8_t>(value.b);
			tmp |= (intern::remapColor<T, uint8_t>(value.g) << 8);
			tmp |= (intern::remapColor<T, uint8_t>(value.r) << 16);
			tmp |= (intern::remapColor<T, uint8_t>(value.a) << 24);
			return tmp;
		}
		// ARGB
		static Color fromHex(std::string_view text) noexcept {
			if (text.length() >= 2 && (text[0] == '0' && (text[1] == 'x' || text[1] == 'X'))) {
				text.remove_prefix(2);
			}

			uint32_t value = 0;
			int shift = 28;
			for (char val : text) {
				int hex = 0;
				if (val >= '0' && val <= '9') {
					hex = val - '0';
				}
				else if(val >= 'a' && val <= 'f') {
					hex = val - 'a';
				}
				else if (val >= 'A' && val <= 'F') {
					hex = val - 'A';
				}
				else {
					continue;
				}

				value |= hex << shift;
				shift -= 4;

				if (shift < 0) {
					break;
				}
			}

			return fromU32(value);
		}

		Color(const Color&) noexcept = default;
		Color& operator=(const Color&) noexcept = default;
		~Color() = default;

		Color() noexcept
			: Color(0, 0, 0)
		{}
		Color(const T& _r, const T& _g, const T& _b, const T& _a = maxval) noexcept
			: r(_r)
			, g(_g)
			, b(_b)
			, a(_a)
		{}

		template<typename U>
		Color(const Color<U>& other) noexcept
			: r(intern::remapColor<U, T>(other.r))
			, g(intern::remapColor<U, T>(other.g))
			, b(intern::remapColor<U, T>(other.b))
			, a(intern::remapColor<U, T>(other.a))
		{}

		template<typename = std::enable_if_t<std::is_floating_point_v<T>>>
		bool operator==(const Color& other) const noexcept {
			return
				std::abs(r - other.r) <= ez::epsilon<T>() &&
				std::abs(g - other.g) <= ez::epsilon<T>() &&
				std::abs(b - other.b) <= ez::epsilon<T>() &&
				std::abs(a - other.a) <= ez::epsilon<T>();
		}
		template<typename = std::enable_if_t<std::is_integral_v<T>>>
		bool operator==(const Color& other) const noexcept {
			return
				r == other.r &&
				g == other.g &&
				b == other.b &&
				a == other.a;
		}

		template<typename = std::enable_if_t<std::is_floating_point_v<T>>>
		bool operator!=(const Color& other) const noexcept {
			return
				std::abs(r - other.r) > ez::epsilon<T>() ||
				std::abs(g - other.g) > ez::epsilon<T>() ||
				std::abs(b - other.b) > ez::epsilon<T>() ||
				std::abs(a - other.a) > ez::epsilon<T>();
		}
		template<typename = std::enable_if_t<std::is_integral_v<T>>>
		bool operator!=(const Color& other) const noexcept {
			return
				r != other.r ||
				g != other.g ||
				b != other.b ||
				a != other.a;
		}

		// Lexicographical compairison r->g->b->a
		bool operator<(const Color& other) const noexcept {
			return 
				r < other.r || 
				g < other.g || 
				b < other.b || 
				a < other.a;
		}
		bool operator>(const Color& other) const noexcept {
			return
				r > other.r ||
				g > other.g ||
				b > other.b ||
				a > other.a;
		}
		bool operator<=(const Color& other) const noexcept {
			return
				r <= other.r ||
				g <= other.g ||
				b <= other.b ||
				a <= other.a;
		}
		bool operator>=(const Color& other) const noexcept {
			return
				r >= other.r ||
				g >= other.g ||
				b >= other.b ||
				a >= other.a;
		}


		union {
			struct {
				T r, g, b, a;
			};
			T data[4];
		};
	};
}