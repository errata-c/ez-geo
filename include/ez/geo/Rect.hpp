#pragma once
#include <ez/math/MathConstants.hpp>
#include <glm/vec2.hpp>
#include <algorithm>

namespace ez {
	template<typename T>
	struct Rect {
		using vec_t = glm::tvec2<T>;

		Rect()
			: position{static_cast<T>(0)}
			, size{static_cast<T>(0)}
		{}
		Rect(const vec_t& p, const vec_t& d)
			: position(p)
			, size(d)
		{}
		Rect(const T & x, const T & y, const T & w, const T & h)
			: position{x, y}
			, size{w, h}
		{}

		template<typename U>
		Rect(const Rect<U>& other)
			: position{ static_cast<T>(other.position.x), static_cast<T>(other.position.y) }
			, size{ static_cast<T>(other.size.x), static_cast<T>(other.size.y) }
		{}

		~Rect() = default;
		Rect(const Rect&) = default;
		Rect(Rect&&) = default;
		Rect& operator=(const Rect&) = default;
		Rect& operator=(Rect&&) = default;

		Rect& merge(const Rect& other) {
			vec_t _max = maximum();
			for (int i = 0; i < 2; ++i) {
				_max[i] = std::max(_max[i], other.position[i] + other.size[i]);
				position[i] = std::min(position[i], other.position[i]);
			}

			size = _max - position;
			
			return *this;
		}

		vec_t minimum() const {
			return position;
		}
		vec_t maximum() const {
			return position + size;
		}

		const vec_t& getPosition() const {
			return position;
		}
		const vec_t& getSize() const {
			return size;
		}

		void setPosition(const vec_t & p) {
			position = p;
		}
		void setSize(const vec_t & s) {
			size = s;
		}

		vec_t toWorld(const vec_t & point) const {
			return point * size + position;
		}
		vec_t toLocal(const vec_t & point) const {
			return (point - position) / size;
		}

		bool isValid() const {
			return (std::abs(size.x) >= ez::epsilon<T>()) &&
				(std::abs(size.y) >= ez::epsilon<T>());
		}

		template<typename F>
		bool inBound(const glm::tvec2<F> & point) const {
			return !(
				(position.x > point.x) ||
				(position.y > point.y) ||
				((position.x + size.x) <= point.x) ||
				((position.y + size.y) <= point.y)
				);
		}

		bool inBound(const Rect& other) const {
			return !(
				((other.position.x + other.size.x) <= position.x) ||
				((other.position.y + other.size.y) <= position.y) ||
				((position.x + size.x) <= other.position.x) ||
				((position.y + size.y) <= other.position.y)
				);
		}

		static Rect between(const vec_t& p0, const vec_t& p1) {
			Rect tmp;
			for (int i = 0; i < 2; ++i) {
				tmp.position[i] = std::min(p0[i], p1[i]);
				tmp.size[i] = std::abs(p1[i] - p0[i]);
			}
			return tmp;
		}
		static Rect merge(const Rect& a, const Rect& b) {
			Rect tmp = a;
			return tmp.merge(b);
		}

		vec_t position, size;
	};
};