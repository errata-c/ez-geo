#pragma once
#include <ez/math/constants.hpp>
#include <glm/common.hpp>
#include <glm/vec2.hpp>
#include <algorithm>

// There are other Rect types, with slightly different properties.
// MMRect, min max rect has better properties for gui rectangles for instance (expand and shrink funcs are just simple addition/subtractions).

namespace ez {
	// Note: This struct assumes that the size is all positive and non-zero.
	// Additionally the bounds of this rect a considered inclusive, meaning maximum point is inside the rect.
	template<typename T, int N>
	struct Rect {
		using vec_t = glm::vec<N, T>;
		static constexpr int components = N;

		Rect() noexcept
			: origin{static_cast<T>(0)}
			, size{static_cast<T>(0)}
		{}
		Rect(const vec_t& _origin, const vec_t& _size) noexcept
			: origin(_origin)
			, size(_size)
		{}

		template<typename U>
		Rect(const Rect<U, N>& other) noexcept
			: origin{ other.origin }
			, size{ other.size }
		{}

		~Rect() noexcept = default;
		Rect(const Rect&) noexcept = default;
		Rect(Rect&&) noexcept = default;
		Rect& operator=(const Rect&) noexcept = default;
		Rect& operator=(Rect&&) noexcept = default;

		vec_t center() const noexcept {
			// Divide by two instead of multiply by 0.5, since the T variable could be an integer type.
			return (minimum() + maximum()) / static_cast<T>(2);
		}

		void translate(const vec_t& offset) noexcept {
			origin += offset;
		}
		void centerTo(const vec_t& point) noexcept {
			translate(point - center());
		}

		void expand(const T & amount) noexcept {
			vec_t c = center();
			vec_t dim = size / static_cast<T>(2);
			dim += amount;
			origin = c - dim;
			size = dim + dim;
		}
		void expand(const vec_t& amount) noexcept {
			vec_t c = center();
			vec_t dim = size / static_cast<T>(2);
			dim += amount;
			origin = c - dim;
			size = dim + dim;
		}

		void shrink(const T& amount) noexcept {
			vec_t c = center();
			vec_t dim = size / static_cast<T>(2);
			dim -= amount;
			dim = glm::max(dim, vec_t{ static_cast<T>(0) });
			origin = c - dim;
			size = dim + dim;
		}
		void shrink(const vec_t& amount) noexcept {
			vec_t c = center();
			vec_t dim = size / static_cast<T>(2);
			dim -= amount;
			dim = glm::max(dim, vec_t{ static_cast<T>(0) });
			origin = c - dim;
			size = dim + dim;
		}

		template<typename F>
		void scale(const F& factor) {
			vec_t c = center();
			vec_t dim = size / static_cast<T>(2);
			dim *= factor;

			origin = c - dim;
			size = dim + dim;
		}
		template<typename F>
		void scale(const glm::vec<N, F>& factor) {
			vec_t c = center();
			vec_t dim = size / static_cast<T>(2);
			dim *= factor;

			origin = c - dim;
			size = dim + dim;
		}
		template<typename F>
		void scale(const vec_t& c, const F& factor) {
			vec_t minp = minimum() - c;
			vec_t maxp = maximum() - c;
			minp = factor * minp + c;
			maxp = factor * maxp + c;
			*this = Rect::between(minp, maxp);
		}
		template<typename F>
		void scale(const vec_t& c, const glm::vec<N, F>& factor) {
			vec_t minp = minimum() - c;
			vec_t maxp = maximum() - c;
			minp = factor * minp + c;
			maxp = factor * maxp + c;
			*this = Rect::between(minp, maxp);
		}

		Rect& merge(const Rect& other) noexcept {
			vec_t _max = maximum();

			_max = glm::max(_max, other.maximum());
			origin = glm::min(origin, other.origin);

			size = _max - origin;
			
			return *this;
		}
		Rect& merge(const vec_t & point) noexcept {
			vec_t _max = maximum();

			_max = glm::max(_max, point);
			origin = glm::min(origin, point);

			size = _max - origin;

			return *this;
		}

		vec_t minimum() const noexcept {
			return origin;
		}
		vec_t maximum() const noexcept {
			return origin + size;
		}

		// This function always succeeds.
		vec_t toWorld(const vec_t & point) const noexcept {
			return point * size + origin;
		}
		// Check isValid prior to calling this function if theres a chance the rect could be invalid.
		vec_t toLocal(const vec_t & point) const noexcept {
			return (point - origin) / size;
		}

		// Check if the assumptions made by this struct are maintained. Size must be all positive and non-zero.
		bool isValid() const noexcept {
			for (int i = 0; i < components; ++i) {
				if (size[i] < ez::epsilon<T>()) {
					return false;
				}
			}
			return true;
		}

		template<typename F>
		bool inBound(const glm::tvec2<F> & point) const noexcept {
			// If statements to have early return like the normal boolean operators.
			for (int i = 0; i < components; ++i) {
				if (point[i] < origin[i]) {
					return false;
				}
				if ((origin[i] + size[i]) < point[i]) {
					return false;
				}
			}
			return true;
		}

		bool inBound(const Rect& other) const noexcept {
			// If statements to have early return like the normal boolean operators.
			for (int i = 0; i < components; ++i) {
				if (!((other.origin[i] + other.size[i]) <= origin[i])) {
					return false;
				}
				if (!((origin[i] + size[i]) <= other.origin[i])) {
					return false;
				}
			}
			return true;
		}

		static Rect between(const vec_t& p0, const vec_t& p1) noexcept {
			Rect tmp;
			// origin.length() is constexpr of number of elements in the vector
			for (int i = 0; i < components; ++i) {
				tmp.origin[i] = std::min(p0[i], p1[i]);
				tmp.size[i] = std::abs(p1[i] - p0[i]);
			}
			return tmp;
		}
		static Rect merge(const Rect& a, const Rect& b) noexcept {
			Rect tmp = a;
			return tmp.merge(b);
		}

		vec_t origin, size;
	};
};