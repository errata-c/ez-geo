#pragma once
#include <cinttypes>
#include <type_traits>
#include <ez/math/constants.hpp>
#include <cmath>
#include <string_view>
#include <cassert>
#include <ostream>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace ez {
	template<typename T>
	struct Color {
	private:
		static constexpr T maxval = std::is_floating_point_v<T> ? T(1) : T(255);

		template<typename From, typename To>
		static To convert(From val) noexcept {
			if constexpr (std::is_floating_point_v<From>) {
				if constexpr (std::is_floating_point_v<To>) {
					return static_cast<To>(val);
				}
				else {
					return static_cast<To>(std::round(val * From(255)));
				}
			}
			else {
				if constexpr (std::is_floating_point_v<To>) {
					return static_cast<To>(val) / To(255);
				}
				else {
					return static_cast<To>(val);
				}
			}
		}

		static uint8_t toU8(const T& val) noexcept {
			return convert<T, uint8_t>(val);
		}
		static T fromU8(int val) noexcept {
			return convert<int, T>(val);
		}
		
	public:
		static_assert(
			std::is_same_v<T, uint8_t> ||
			std::is_same_v<T, float> ||
			std::is_same_v<T, double> ||
			std::is_same_v<T, long double>,
			"Unsupported value type for ez::Color!"
		);

		// ARGB
		static Color fromU32(uint32_t value) noexcept {
			Color tmp;
			tmp.b = fromU8(value & 0xFF);
			value >>= 8;
			tmp.g = fromU8(value & 0xFF);
			value >>= 8;
			tmp.r = fromU8(value & 0xFF);
			value >>= 8;
			tmp.a = fromU8(value & 0xFF);
			return tmp;
		}
		// ARGB
		static uint32_t toU32(const Color& value) noexcept {
				uint32_t tmp = toU8(value.b);
				tmp |= toU8(value.g) << 8;
				tmp |= toU8(value.r) << 16;
				tmp |= toU8(value.a) << 24;
				return tmp;
		}
		// ARGB
		static Color fromHex(std::string_view text) noexcept {
			if (text.length() >= 2 && (text[0] == '0' && (text[1] == 'x' || text[1] == 'X'))) {
				text.remove_prefix(2);
			}

			int read[8];
			int count = 0;
			for (char val : text) {
				int hex = 0;
				if (val >= '0' && val <= '9') {
					hex = val - '0';
				}
				else if(val >= 'a' && val <= 'f') {
					hex = (val - 'a') + 10;
				}
				else if (val >= 'A' && val <= 'F') {
					hex = (val - 'A') + 10;
				}
				else {
					continue;
				}

				read[count] = hex;
				++count;

				if (count == 8) {
					break;
				}
			}

			switch (count) {
			case 0: 
				return Color{};
			case 1: {
				int value = read[0] | (read[0] << 4);
				return Color{ fromU8(value), fromU8(value), fromU8(value) };
			}
			case 2: {
				int value = read[1] | (read[0] << 4);
				return Color{ fromU8(value), fromU8(value), fromU8(value) };
			}
			case 3: {
				int
					v0 = read[0] | (read[0] << 4),
					v1 = read[1] | (read[1] << 4),
					v2 = read[2] | (read[2] << 4);
				return Color{ fromU8(v0), fromU8(v1), fromU8(v2) };
			}
			case 4: {
				int
					v3 = read[0] | (read[0] << 4),
					v0 = read[1] | (read[1] << 4),
					v1 = read[2] | (read[2] << 4),
					v2 = read[3] | (read[3] << 4);
				return Color{ fromU8(v0), fromU8(v1), fromU8(v2), fromU8(v3) };
			}
			case 6: {
				int
					v0 = read[1] | (read[0] << 4),
					v1 = read[3] | (read[2] << 4),
					v2 = read[5] | (read[4] << 4);
				return Color{ fromU8(v0), fromU8(v1), fromU8(v2) };
			}
			case 8: {
				int
					v3 = read[1] | (read[0] << 4),
					v0 = read[3] | (read[2] << 4),
					v1 = read[5] | (read[4] << 4),
					v2 = read[7] | (read[6] << 4);
				return Color{ fromU8(v0), fromU8(v1), fromU8(v2), fromU8(v3) };
			}
			default:
				return Color{};
			}
		}

		// hue is in degrees [0, 360], s and v are normalized to range [0, 1]
		template<typename = std::enable_if_t<std::is_floating_point_v<T>>>
		static Color fromHSV(T h, T s, T v) {
			return fromHSVA(h, s, v, T(1));
		}
		template<typename = std::enable_if_t<std::is_floating_point_v<T>>>
		static Color fromHSVA(T h, T s, T v, T a) {
			s = std::max(T(0), std::min(s, T(1)));
			v = std::max(T(0), std::min(v, T(1)));
			a = std::max(T(0), std::min(a, T(1)));

			int i;
			T p, q, t, ff;
			Color out{ T(0), T(0), T(0), a };
			if (s <= T(0)) {
				out.r = v; 
				out.g = v; 
				out.b = v;
				return out;
			}
			// Rollover the angle
			if (h > T(360)) {
				h = T(0);
			}

			// Divide by one sixth rotation
			h /= T(60);
			// Convert to int to get index of color space (out of possible 6)
			i = static_cast<int>(h);
			ff = h - T(i);
			p = v * (T(1) - s);
			q = v * (T(1) - s * ff);
			t = v * (T(1) - s * (T(1) - ff));

			// Change result based on the selected color space
			switch (i) {
			case 0:
				out.r = v; 
				out.g = t; 
				out.b = p; 
				break;
			case 1: 
				out.r = q; 
				out.g = v; 
				out.b = p; 
				break;
			case 2: 
				out.r = p; 
				out.g = v; 
				out.b = t; 
				break;
			case 3: 
				out.r = p; 
				out.g = q; 
				out.b = v; 
				break;
			case 4: 
				out.r = t; 
				out.g = p; 
				out.b = v; 
				break;
			case 5:
			default:
				out.r = v; 
				out.g = p; 
				out.b = q;
				break;
			}
			return out;
		}

		Color(const Color&) noexcept = default;
		Color& operator=(const Color&) noexcept = default;
		~Color() = default;

		Color() noexcept
			: Color(0, 0, 0)
		{}
		Color(const T& v) noexcept
			: Color(v, v, v)
		{}
		Color(const T& _r, const T& _g, const T& _b, const T& _a = maxval) noexcept
			: r(_r)
			, g(_g)
			, b(_b)
			, a(_a)
		{}
		
		template<typename U>
		Color(const Color<U> & other) noexcept 
			: r(convert<U, T>(other.r))
			, g(convert<U, T>(other.g))
			, b(convert<U, T>(other.b))
			, a(convert<U, T>(other.a))
		{}
		template<typename U>
		Color& operator=(const Color<U>& other) noexcept {
			r = convert<U, T>(other.r);
			g = convert<U, T>(other.g);
			b = convert<U, T>(other.b);
			a = convert<U, T>(other.a);
			return *this;
		}

		const T& operator[](int i) const noexcept {
			assert(i >= 0 && i < 4);
			return data[i];
		}
		T& operator[](int i) noexcept {
			assert(i >= 0 && i < 4);
			return data[i];
		}
		
		void normalize() noexcept {
			r = std::max(0, std::min(r, maxval));
			g = std::max(0, std::min(g, maxval));
			b = std::max(0, std::min(b, maxval));
			a = std::max(0, std::min(a, maxval));
		}

		bool operator==(const Color& other) const noexcept {
			if constexpr (std::is_floating_point_v<T>) {
				return
					std::abs(r - other.r) <= ez::epsilon<T>() &&
					std::abs(g - other.g) <= ez::epsilon<T>() &&
					std::abs(b - other.b) <= ez::epsilon<T>() &&
					std::abs(a - other.a) <= ez::epsilon<T>();
			}
			else {
				return
					r == other.r &&
					g == other.g &&
					b == other.b &&
					a == other.a;
			}
		}
		bool operator!=(const Color& other) const noexcept {
			if constexpr (std::is_floating_point_v<T>) {
				return
					std::abs(r - other.r) > ez::epsilon<T>() ||
					std::abs(g - other.g) > ez::epsilon<T>() ||
					std::abs(b - other.b) > ez::epsilon<T>() ||
					std::abs(a - other.a) > ez::epsilon<T>();
			}
			else {
				return
					r != other.r ||
					g != other.g ||
					b != other.b ||
					a != other.a;
			}
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

	using ColorU = Color<uint8_t>;
	using ColorF = Color<float>;
	using ColorD = Color<double>;

	template<typename T>
	std::ostream& operator<<(std::ostream& os, const ez::Color<T>& val) {
		os << "Color(";
		os << int(val.r) << ", " << int(val.g) << ", " << int(val.b) << ", " << int(val.a);
		os << ')';
		return os;
	}
}

using ez::operator<<;
