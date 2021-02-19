#pragma once
#include <glm/vec2.hpp>
#include <glm/geometric.hpp>

namespace ez {
	
	template<typename T>
	struct Circle
	{
		using vec2_t = glm::tvec2<T>;

		Circle(T r = static_cast<T>(1), const vec2_t& o = vec2_t{static_cast<T>(0)})
			: radius(r)
			, origin(o)
		{}

		~Circle() = default;
		Circle(const Circle&) = default;
		Circle(Circle&&) noexcept = default;
		Circle& operator=(const Circle&) = default;
		Circle& operator=(Circle&&) noexcept = default;

		void translate(const vec2_t & off) {
			origin += off;
		}
		void move(const vec2_t& pos) {
			origin = pos;
		}
		void resize(T size) {
			radius = size;
		}
		void scaleBy(T amount) {
			radius *= amount;
		}

		T getRadius() const {
			return radius;
		}
		void setRadius(T len) {
			radius = len;
		}

		const vec2_t& getOrigin() const {
			return origin;
		}
		void setOrigin(const vec2_t & o) {
			origin = o;
		}

		const vec2_t& getLocation() const {
			return origin;
		}
		void setLocation(const vec2_t& loc) {
			origin = loc;
		}

		static Circle fromOuterTriangle(const vec2_t& c1, const vec2_t& c2, const vec2_t& c3) {
			vec2_t H = (c3 + c1) / static_cast<T>(2);
			vec2_t G = H - c2;
			H -= c1;
			T H2 = glm::dot(H, H);
			T G2 = glm::dot(G, G);
			vec2_t S = G * (H2 / G2);
			vec2_t o = S + c2;
			T r = glm::length(o - c1);

			return Circle{ r, o };
		}

		T radius;
		vec2_t origin;
	};
};
