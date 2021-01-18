#pragma once
#include <algorithm>
#include <type_traits>
#include <cinttypes>
#include <cassert>
#include <vector>
#include <array>
#include <glm/vec2.hpp>
#include <ez/geo/CBezier.hpp>

namespace ez {
	template<typename T, glm::length_t L>
	class BPath {
	public:
		using value_type = glm::vec<L, T>;
		using Point = value_type;

		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;

		using reference = value_type&;
		using pointer = value_type*;
		using const_reference = const value_type&;
		using const_pointer = const value_type*;

		using Container = std::vector<Point>;

		using iterator = typename Container::iterator;
		using const_iterator = typename Container::const_iterator;
		using reverse_iterator = typename Container::reverse_iterator;
		using const_reverse_iterator = typename Container::const_reverse_iterator;

		class segment_generator;

		using Segment = ez::CBezier<T, L>;
		struct Index {
			size_type index;
			T delta;
		};

		BPath()
			: open(true)
		{}

		BPath(size_type n, const_reference point, bool _open = true)
			: open(_open)
			, points(n, point)
		{}

		template<typename Iter>
		BPath(Iter first, Iter last, bool _open = true)
			: open(_open)
			, points(first, last)
		{
			static_assert(!std::is_same_v<Iter::iterator_category, std::output_iterator_tag>);
		}

		BPath(const BPath& other)
			: open(other.open)
			, points(other.points)
		{}

		BPath(BPath&& other) noexcept
			: open(other.open)
			, points(std::move(other.points))
		{}

		BPath& operator=(const BPath& other) {
			points = other.points;
			open = other.open;

			return *this;
		}

		BPath& operator=(BPath&& other) noexcept {
			points = std::move(other.points);
			open = other.open;

			return *this;
		}

		// Return the segment at the interpolation value.
		Index indexAt(T t) const {
			t = std::min(static_cast<T>(1), std::max(t, static_cast<T>(0)));
			Index index;

			index.index = numPoints();
			if (isClosed()) {
				index.index -= N1;
			}

			index.delta = static_cast<T>(index.index)* t;
			t = std::floor(index.delta);
			index.delta = index.delta - t;
			index.index = static_cast<size_type>(t);

			return index;
		}

		Segment segmentAt(size_type i) const {
			assert(i < numSegments());
			Segment seg;

			if (isClosed()) {
				seg[0] = interp(points[(i - 1 + numPoints()) % numPoints()], points[i], points[(i + 1) % numPoints()], points[(i + 2) % numPoints()], T(1.0 / 3.0));
				seg[1] = interp(points[(i - 1 + numPoints()) % numPoints()], points[i], points[(i + 1) % numPoints()], points[(i + 2) % numPoints()], T(2.0 / 3.0));
				seg[0] = (seg[0] + seg[1]) * T(0.5);

				seg[2] = interp(points[i], points[(i + 1) % numPoints()], points[(i + 2) % numPoints()], points[(i + 3) % numPoints()], T(1.0 / 3.0));
				seg[3] = interp(points[i], points[(i + 1) % numPoints()], points[(i + 2) % numPoints()], points[(i + 3) % numPoints()], T(2.0 / 3.0));
				seg[3] = (seg[2] + seg[3]) * T(0.5);
			}
			else {
				if (i == 0) {
					// no previous segment.
					seg[0] = points.front();
					seg[1] = interp(seg[0], points[1], T(0.5)); // (seg[0] + points[1])* T(0.5);

					seg[2] = interp(points[0], points[1], points[2], points[3], T(1.0/3.0));
					seg[3] = interp(points[0], points[1], points[2], points[3], T(2.0/3.0));
					seg[3] = (seg[2] + seg[3]) * T(0.5);
				}
				else if (i == (numPoints()-3)) {
					// No next segment.
					seg[0] = interp(points[numPoints() - 4], points[numPoints() - 3], points[numPoints() - 2], points[numPoints()-1], T(1.0 / 3.0));
					seg[1] = interp(points[numPoints() - 4], points[numPoints() - 3], points[numPoints() - 2], points[numPoints()-1], T(2.0 / 3.0));
					seg[0] = (seg[0] + seg[1]) * T(0.5);

					seg[3] = points.back();
					seg[2] = interp(seg[3], points[numPoints()-2], T(0.5)); // (seg[3] + points[numPoints() - 2])* T(0.5);
				}
				else {
					seg[0] = interp(points[i-1], points[i], points[i+1], points[i+2], T(1.0 / 3.0));
					seg[1] = interp(points[i-1], points[i], points[i+1], points[i+2], T(2.0 / 3.0));
					seg[0] = (seg[0] + seg[1]) * T(0.5);

					seg[2] = interp(points[i], points[i+1], points[i + 2], points[i + 3], T(1.0 / 3.0));
					seg[3] = interp(points[i], points[i+1], points[i + 2], points[i + 3], T(2.0 / 3.0));
					seg[3] = (seg[2] + seg[3]) * T(0.5);
				}
			}
			return seg;
		}

		T length() const {
			T total = static_cast<T>(0);

			size_type count = numSegments();
			for (size_type i = 0; i < count; ++i) {
				Segment seg = segmentAt(i);
				total += seg.length();
			}

			return total;
		}

		Point evalAt(Index index) const {
			Segment seg = segmentAt(index.index);
			return seg.evalAt(index.delta);
		}

		Point evalAt(T t) const {
			return evalAt(indexAt(t));
		}

		reference operator[](size_type i) {
			assert(i < points.size());
			return points[i];
		}
		const_reference operator[](size_type i) const {
			assert(i < points.size());
			return points[i];
		}
		reference at(size_type i) {
			return points.at(i);
		}
		const_reference at(size_type i) const {
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

		template<typename Iter>
		void assign(Iter first, Iter last) {
			static_assert(!std::is_same<typename Iter::iterator_category, std::output_iterator_tag>::value, "Method 'assign' requires the iterator not be an output iterator.");

			points.assign<Iter>(first, last);
		}

		void assign(size_type n, const_reference point) {
			points.assign(n, point);
		}

		void assign(std::initializer_list<Point> il) {
			points.assign(il);
		}

		void insert(const_iterator it, const_reference point) {
			points.insert(it, point);
		}
		void insert(const_iterator it, size_type n, const_reference point) {
			points.insert(it, n, point);
		}
		template<typename Iter>
		void insert(const_iterator it, Iter first, Iter last) {
			static_assert(!std::is_same<Iter::iterator_category, std::output_iterator_tag>::value, "Method 'insert' requires the iterator not be an output iterator.");

			points.insert<Iter>(it, first, last);
		}
		void insert(const_iterator it, std::initializer_list<Point> il) {
			points.insert(it, il);
		}

		void erase(const_iterator it) {
			points.erase(it);
		}
		void erase(const_iterator first, const_iterator last) {
			points.erase(first, last);
		}

		void resize(size_type n) {
			points.resize(n);
		}
		void resize(size_type n, const_reference point) {
			points.resize(n, point);
		}

		size_type size() const {
			return points.size();
		}
		size_type numPoints() const {
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

		void swap(BPath& other) noexcept {
			points.swap(other.points);
			std::swap(open, other.open);
		}

		void clear() {
			open = false;
			points.clear();
		}

		void append(const_reference point) {
			points.push_back(point);
		}
		void push_back(const_reference point) {
			points.push_back(point);
		}
		void pop_back() {
			points.pop_back();
		}

		bool isOpen() const {
			return open;
		}
		bool isClosed() const {
			return !open;
		}
		void setOpen(bool value) {
			open = value;
		}
		void setClosed(bool value) {
			open = !value;
		}

		size_type numSegments() const {
			if (points.size() > 3) {
				if (isOpen()) {
					return std::max(points.size(), size_type(2)) - size_type(2);
				}
				else {
					return points.size();
				}
			}
			else {
				return 0;
			}
		}

		BPath clone() const {
			return BPath{ *this };
		}

	private:
		bool open;
		Container points;

		static constexpr T weight = 1.0;

		Point interp(const Point& p0, const Point& p1, T t) const {
			return ez::Bezier::interpolate(p0, p1, t);
		};
		Point interp(const Point& p0, const Point& p1, const Point& p2, T t) const {
			return ez::Bezier::interpolate(p0, p1, p2, t);
		};
		Point interp(const Point& p0, const Point& p1, const Point& p2, const Point& p3, T t) const {
			return ez::Bezier::interpolate(p0, p1, p2, p3, t);
		};
		
	};
};