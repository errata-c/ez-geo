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
#include <glm/gtc/color_space.hpp>
#include <glm/gtx/color_space.hpp>

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
		static Color fromHSV(T h, T s, T v) noexcept {
			return fromHSV(glm::tvec3<T>{h, s, v});
		}
		template<typename = std::enable_if_t<std::is_floating_point_v<T>>>
		static Color fromHSVA(T h, T s, T v, T a) noexcept {
			return fromHSVA(glm::tvec4<T>{h, s, v, a});
		}
		template<typename = std::enable_if_t<std::is_floating_point_v<T>>>
		static Color fromHSV(const glm::tvec3<T>& value) noexcept {
			return Color{ glm::rgbColor(value) };
		}
		template<typename = std::enable_if_t<std::is_floating_point_v<T>>>
		static Color fromHSVA(const glm::tvec4<T> & value) noexcept {
			return Color{
				glm::rgbColor(glm::tvec3<T>{ value.data }),
				value.a
			};
		}

		template<typename = std::enable_if < std::is_floating_point_v<T>>>
		static glm::tvec3<T> toHSV(const Color& value) noexcept {
			return glm::hsvColor(glm::tvec3<T>{value.data});
		}
		template<typename = std::enable_if_t<std::is_floating_point_v<T>>>
		static glm::tvec4<T> toHSVA(const Color& value) noexcept {
			return glm::tvec4<T>{
				glm::hsvColor(glm::tvec3<T>{value.data}),
				value.a
			};
		}

		template<typename = std::enable_if < std::is_floating_point_v<T>>>
		static glm::tvec3<T> toSRGB(const Color& value) noexcept {
			return glm::convertLinearToSRGB(glm::tvec3<T>{value.data});
		}
		template<typename = std::enable_if_t<std::is_floating_point_v<T>>>
		static glm::tvec4<T> toSRGBA(const Color& value) noexcept {
			return glm::tvec4<T>{
				glm::convertLinearToSRGB(glm::tvec3<T>{value.data}),
				value.a
			};
		}

		template<typename = std::enable_if < std::is_floating_point_v<T>>>
		static glm::tvec3<T> toSRGB(const Color& value, const T& gamma) noexcept {
			return glm::convertLinearToSRGB(glm::tvec3<T>{value.data}, gamma);
		}
		template<typename = std::enable_if_t<std::is_floating_point_v<T>>>
		static glm::tvec4<T> toSRGBA(const Color& value, const T& gamma) noexcept {
			return glm::tvec4<T>{
				glm::convertLinearToSRGB(glm::tvec3<T>{value.data}, gamma),
				value.a
			};
		}

		template<typename = std::enable_if < std::is_floating_point_v<T>>>
		static Color fromSRGB(const glm::tvec3<T>& value) noexcept {
			return Color{
				glm::convertSRGBToLinear(value)
			};
		}
		template<typename = std::enable_if_t<std::is_floating_point_v<T>>>
		static glm::tvec4<T> fromSRGBA(const glm::tvec4<T>& value) noexcept {
			return Color{
				glm::convertSRGBToLinear(glm::tvec3<T>{value}),
				value.a
			};
		}

		template<typename = std::enable_if < std::is_floating_point_v<T>>>
		static Color fromSRGB(const glm::tvec3<T>& value, const T& gamma) noexcept {
			return Color{
				glm::convertSRGBToLinear(value, gamma)
			};
		}
		template<typename = std::enable_if_t<std::is_floating_point_v<T>>>
		static glm::tvec4<T> fromSRGBA(const glm::tvec4<T>& value, const T& gamma) noexcept {
			return Color{
				glm::convertSRGBToLinear(glm::tvec3<T>{value}, gamma),
				value.a
			};
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
		explicit Color(const glm::tvec4<T>& value) noexcept
			: data(value)
		{}
		explicit Color(const glm::tvec3<T>& value, const T& _a = maxval) noexcept
			: data{ value, _a }
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

		template<typename = std::enable_if_t<std::is_floating_point_v<T>>>
		T luminosity() const noexcept {
			return glm::luminosity(glm::tvec3<T>{ data });
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
			glm::tvec4<T> data;
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
