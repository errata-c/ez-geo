#pragma once
#include <type_traits>
#include <array>
#include <cassert>
#include <cinttypes>
#include <glm/vec2.hpp>
#include <ez/geo/BezierFunctions.h>

namespace ez {
	class QBezier {
	public:
		using T = double;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;

		static constexpr std::size_t L = 2;
		using value_type = glm::vec<L, T>;
		using Point = value_type;
		using reference = value_type&;
		using pointer = value_type*;
		using const_reference = const value_type&;
		using const_pointer = const value_type*;

		using Container = std::array<Point, 3>;

		using iterator = typename Container::iterator;
		using const_iterator = typename Container::const_iterator;
		using reverse_iterator = typename Container::reverse_iterator;
		using const_reverse_iterator = typename Container::const_reverse_iterator;

		QBezier()
			: points{ Point{static_cast<T>(0)}, Point{static_cast<T>(0)}, Point{static_cast<T>(0)} }
		{}

		QBezier(const Point& p0, const Point& p1, const Point& p2)
			: points{ p0, p1, p2 }
		{}

		Point evalAt(T t) const {
			return ez::Bezier::interpolate(points[0], points[1], points[2], t);
		}

		T length() const {
			return ez::Bezier::length(points[0], points[1], points[2]);
		}

		reference operator[](std::size_t i) {
			assert(i < points.size());
			return points[i];
		}
		const_reference operator[](std::size_t i) const {
			assert(i < points.size());
			return points[i];
		}
		reference at(std::size_t i) {
			return points.at(i);
		}
		const_reference at(std::size_t i) const {
			return points.at(i);
		}

		pointer data() {
			return points.data();
		}
		const_pointer data() const {
			return points.data();
		}

		constexpr bool empty() const {
			return false;
		}

		size_type size() const {
			return points.size();
		}
		size_type max_size() const {
			return points.max_size();
		}

		reference front() {
			return points.front();
		}
		const_reference front() const {
			return points.front();
		}

		reference back() {
			return points.back();
		}
		const_reference back() const {
			return points.back();
		}

		iterator begin() {
			return points.begin();
		}
		iterator end() {
			return points.end();
		}

		const_iterator begin() const {
			return points.begin();
		}
		const_iterator end() const {
			return points.end();
		}

		const_iterator cbegin() const {
			return points.cbegin();
		}
		const_iterator cend() const {
			return points.cend();
		}

		reverse_iterator rbegin() {
			return points.rbegin();
		}
		reverse_iterator rend() {
			return points.rend();
		}

		const_reverse_iterator crbegin() const {
			return points.crbegin();
		}
		const_reverse_iterator crend() const {
			return points.crend();
		}

		void fill(const_reference value) {
			points.fill(value);
		}

		void swap(QBezier& other) noexcept {
			points.swap(other.points);
		}

		template<typename Iter>
		void assign(Iter first, Iter last) {
			static_assert(!std::is_same<typename Iter::iterator_category, std::output_iterator_tag>::value, "Method 'assign' requires the iterator not be an output iterator.");
			static_assert(std::is_convertible<typename Iter::value_type, value_type>::value, "Method 'assign' requires the iterator to have a convertible value_type.");

			std::remove_const_t<std::remove_reference_t<Iter>> iter = first;

			for (reference value : points) {
				if (iter == last) {
					break;
				}

				value = *iter;
				++iter;
			}
		}

		Container points;
	};
};