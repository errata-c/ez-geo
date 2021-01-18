#pragma once
#ifndef EZ_GEO_BEZIER_FUNCTIONS_INCLUDED
#include <type_traits>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/norm.hpp>
#include "BezierTypeChecks.hpp"
#endif

namespace ez {
	namespace Bezier {
		// Line interpolation
		template<typename T, std::enable_if_t<is_vec<T>::value, int> = 0>
		T interpolate(const T& p0, const T& p1, typename is_vec<T>::value_type t) {
			return p0 * (static_cast<T>(1) - t) + p1 * t;
		};

		// Normal quadratic
		template<typename T, std::enable_if_t<is_vec<T>::value, int> = 0>
		T interpolate(const T& p0, const T& p1, const T& p2, typename is_vec<T>::value_type t) {
			using value_type = typename is_vec<T>::value_type;

			value_type t1 = static_cast<value_type>(1) - t;

			return p0 * t1 * t1 + p1 * static_cast<value_type>(2)* t1* t + p2 * t * t;
		};

		// Normal cubic
		template<typename T, std::enable_if_t<is_vec<T>::value, int> = 0>
		T interpolate(const T& p0, const T& p1, const T& p2, const T& p3, typename is_vec<T>::value_type t) {
			using value_type = typename is_vec<T>::value_type;
			value_type t1 = static_cast<value_type>(1) - t;
			value_type tt = t * t;
			value_type t1t1 = t1 * t1;

			return p0 * t1t1 * t1 + p1 * static_cast<value_type>(3)* t1t1* t + p2 * static_cast<value_type>(3)* tt* t1 + p3 * tt * t;
		};

		template<typename T, typename Iter, std::enable_if_t<is_random_iterator<Iter>::value, int> = 0>
		typename Iter::value_type interpolate(Iter begin, Iter end, T t) {
			using vec_t = typename Iter::value_type;

			static_assert(std::is_floating_point<T>::value, "Bezier functions only work for floating point types.");

			intptr_t diff = end - begin;
			switch (diff) {
			case 0:
			case 1:
				return vec_t{};
			case 2: // linear
				return Bezier::interpolate(*(begin), *(begin + 1), t);
			case 3: // quadratic
				return Bezier::interpolate(*(begin), *(begin + 1), *(begin + 2), t);
			case 4: // cubic
				return Bezier::interpolate(*(begin), *(begin + 1), *(begin + 2), *(begin + 3), t);
			default:
				// Dynamic:
				// Recursively evaluate the bezier from the input points.
				return Bezier::interpolate(Bezier::interpolate(begin, end - 1, t), Bezier::interpolate(begin + 1, end, t), t);
			}
		};

		template<std::size_t N, typename T, typename Iter, 
			std::enable_if_t<is_input_iterator<Iter>::value && (N == 1), int> = 0>
		typename Iter::value_type interpolate(Iter it, T t) {
			using vec_t = typename Iter::value_type;

			static_assert(std::is_floating_point<T>::value, "Bezier functions only work for floating point types.");
			return *it;
		};

		template<std::size_t N, typename T, typename Iter, 
			std::enable_if_t<is_input_iterator<Iter>::value && (N == 2), int> = 0>
		typename Iter::value_type interpolate(Iter it, T t) {
			using vec_t = typename Iter::value_type;

			static_assert(std::is_floating_point<T>::value, "Bezier functions only work for floating point types.");
			vec_t p0 = *it;
			++it;
			vec_t p1 = *it;
			return Bezier::interpolate(p0, p1, t);
		};

		template<std::size_t N, typename T, typename Iter, 
			std::enable_if_t<is_input_iterator<Iter>::value && (N == 3), int> = 0>
		typename Iter::value_type interpolate(Iter it, T t) {
			using vec_t = typename Iter::value_type;

			static_assert(std::is_floating_point<T>::value, "Bezier functions only work for floating point types.");
			vec_t p0 = *it;
			++it;
			vec_t p1 = *it;
			++it;
			vec_t p2 = *it;
			return Bezier::interpolate(p0, p1, p2, t);
		};

		template<std::size_t N, typename T, typename Iter, 
			std::enable_if_t<is_input_iterator<Iter>::value && (N == 4) && std::is_floating_point<T>::value, int> = 0>
		typename Iter::value_type interpolate(Iter it, T t) {
			using vec_t = typename Iter::value_type;

			vec_t p0 = *it;
			++it;
			vec_t p1 = *it;
			++it;
			vec_t p2 = *it;
			++it;
			vec_t p3 = *it;
			return Bezier::interpolate(p0, p1, p2, p3, t);
		};

		template<typename T, typename Iter, std::enable_if_t<is_input_iterator<Iter>::value && std::is_floating_point<T>::value, int> = 0>
		typename Iter::value_type surfaceInterpolate(Iter row0, Iter row1, T u, T v) {
			using vec_t = Iter::value_type;

			T v1 = static_cast<T>(1) - v;
			T u1 = static_cast<T>(1) - u;

			T uc[2];
			T vc[2];

			uc[0] = u1;
			uc[1] = u;

			vc[0] = v1;
			vc[1] = v;

			vec_t arr[2][2];

			arr[0][0] = *row0;
			++row0;
			arr[0][1] = *row0;

			arr[1][0] = *row1;
			++row1;
			arr[1][1] = *row1;

			vec_t ret{ static_cast<T>(0) };

			for (int y = 0; y < 2; ++y) {
				for (int x = 0; x < 2; ++x) {
					ret += arr[x][y] * uc[x] * vc[y];
				}
			}

			return ret;
		};

		template<typename T, typename Iter, std::enable_if_t<is_input_iterator<Iter>::value && std::is_floating_point<T>::value, int> = 0>
		typename Iter::value_type surfaceInterpolate(Iter row0, Iter row1, Iter row2, T u, T v) {
			using vec_t = typename Iter::value_type;

			T v1 = static_cast<T>(1) - v;
			T u1 = static_cast<T>(1) - u;

			T uc[3];
			T vc[3];

			uc[0] = u1 * u1;
			uc[1] = u * u1;
			uc[2] = u * u;

			vc[0] = v1 * v1;
			vc[1] = v * v1;
			vc[2] = v * v;

			vec_t arr[3][3];

			arr[0][0] = *row0;
			++row0;
			arr[0][1] = *row0;
			++row0;
			arr[0][2] = *row0;

			arr[1][0] = *row1;
			++row1;
			arr[1][1] = *row1;
			++row1;
			arr[1][2] = *row1;

			arr[2][0] = *row2;
			++row2;
			arr[2][1] = *row2;
			++row2;
			arr[2][2] = *row2;

			vec_t ret{static_cast<T>(0)};

			for (int y = 0; y < 3; ++y) {
				for (int x = 0; x < 3; ++x) {
					ret += arr[x][y] * uc[x] * vc[y];
				}
			}

			return ret;
		};

		template<typename T, typename Iter, std::enable_if_t<is_input_iterator<Iter>::value && std::is_floating_point<T>::value, int> = 0>
		typename Iter::value_type surfaceInterpolate(Iter row0, Iter row1, Iter row2, Iter row3, T u, T v) {
			using vec_t = typename Iter::value_type;

			T v1 = static_cast<T>(1) - v;
			T u1 = static_cast<T>(1) - u;

			T uc[4];
			T vc[4];

			uc[0] = u1 * u1 * u1;
			uc[1] = u * u1 * u1 * static_cast<T>(3);
			uc[2] = u * u * u1 * static_cast<T>(3);
			uc[3] = u * u * u;

			vc[0] = v1 * v1 * v1;
			vc[1] = v * v1 * v1 * static_cast<T>(3);
			vc[2] = v * v * v1 * static_cast<T>(3);
			vc[3] = v * v * v;

			vec_t arr[4][4];

			arr[0][0] = *row0;
			++row0;
			arr[0][1] = *row0;
			++row0;
			arr[0][2] = *row0;
			++row0;
			arr[0][3] = *row0;

			arr[1][0] = *row1;
			++row1;
			arr[1][1] = *row1;
			++row1;
			arr[1][2] = *row1;
			++row1;
			arr[1][3] = *row1;

			arr[2][0] = *row2;
			++row2;
			arr[2][1] = *row2;
			++row2;
			arr[2][2] = *row2;
			++row2;
			arr[2][3] = *row2;

			arr[3][0] = *row3;
			++row3;
			arr[3][1] = *row3;
			++row3;
			arr[3][2] = *row3;
			++row3;
			arr[3][3] = *row3;

			vec_t ret{ static_cast<T>(0) };

			for (int y = 0; y < 4; ++y) {
				for (int x = 0; x < 4; ++x) {
					ret += arr[x][y] * uc[x] * vc[y];
				}
			}

			return ret;
		};

		// Find a subcurve along the surface.
		template<typename T, typename InIter, typename OutIter>
		void surfaceSubcurveU(InIter row0, InIter row1, T u, OutIter output) {
			static_assert(is_random_iterator<InIter>::value, "The vector valued iterator passed in must be random access!");
			using vec_t = typename is_random_iterator<InIter>::vec_t;

			static_assert(is_output_iterator<OutIter, vec_t>::value, "The iterator passed in is not a proper output iterator!");
			using T = typename is_vec<vec_t>::value_type;

			static_assert(std::is_floating_point<T>::value, "The interpolation value passed in must be a floating point value!");
			
			T u1 = static_cast<T>(1) - u;

			T uc[2];

			uc[0] = u1;
			uc[1] = u;
			
			vec_t arr[2][2];

			arr[0][0] = *row0;
			++row0;
			arr[0][1] = *row0;

			arr[1][0] = *row1;
			++row1;
			arr[1][1] = *row1;

			vec_t ret{ static_cast<T>(0) };
			for (int x = 0; x < 2; ++x) {
				ret += arr[x][0] * uc[x];
			}
			(*output) = ret;
			++output;

			ret = vec_t{ static_cast<T>(0) };
			for (int x = 0; x < 2; ++x) {
				ret += arr[x][0] * uc[x];
			}
			(*output) = ret;
		};

		// Find a subcurve along the surface.
		template<typename T, typename InIter, typename OutIter>
		void surfaceSubcurveV(InIter row0, InIter row1, T v, OutIter output) {
			static_assert(is_random_iterator<InIter>::value, "The vector valued iterator passed in must be random access!");
			using vec_t = typename is_random_iterator<InIter>::vec_t;

			static_assert(is_output_iterator<OutIter, vec_t>::value, "The iterator passed in is not a proper output iterator!");
			using T = typename is_vec<vec_t>::value_type;

			static_assert(std::is_floating_point<T>::value, "The interpolation value passed in must be a floating point value!");

			T v1 = static_cast<T>(1) - v;

			T vc[2];

			vc[0] = v1;
			vc[1] = v;

			vec_t arr[2][2];

			arr[0][0] = *row0;
			++row0;
			arr[0][1] = *row0;

			arr[1][0] = *row1;
			++row1;
			arr[1][1] = *row1;

			vec_t ret{ static_cast<T>(0) };
			for (int y = 0; y < 2; ++y) {
				ret += arr[0][y] * vc[y];
			}
			(*output) = ret;
			++output;

			ret = vec_t{ static_cast<T>(0) };
			for (int y = 0; y < 2; ++y) {
				ret += arr[0][y] * vc[y];
			}
			(*output) = ret;
		};

		// Find a subcurve along the surface.
		template<typename T, typename InIter, typename OutIter>
		void surfaceSubcurveU(InIter row0, InIter row1, InIter row2, T u, OutIter output) {
			static_assert(is_random_iterator<InIter>::value, "The vector valued iterator passed in must be random access!");
			using vec_t = typename is_random_iterator<InIter>::vec_t;

			static_assert(is_output_iterator<OutIter, vec_t>::value, "The iterator passed in is not a proper output iterator!");
			using T = typename is_vec<vec_t>::value_type;

			static_assert(std::is_floating_point<T>::value, "The interpolation value passed in must be a floating point value!");

			T u1 = static_cast<T>(1) - u;

			T uc[3];

			uc[0] = u1 * u1;
			uc[1] = u * u1;
			uc[2] = u * u;

			vec_t arr[3][3];

			arr[0][0] = *row0;
			++row0;
			arr[0][1] = *row0;
			++row0;
			arr[0][2] = *row0;

			arr[1][0] = *row1;
			++row1;
			arr[1][1] = *row1;
			++row1;
			arr[1][2] = *row1;

			arr[2][0] = *row2;
			++row2;
			arr[2][1] = *row2;
			++row2;
			arr[2][2] = *row2;

			vec_t ret{ static_cast<T>(0) };
			for (int x = 0; x < 3; ++x) {
				ret += arr[x][0] * uc[x];
			}
			(*output) = ret;
			++output;

			ret = vec_t{ static_cast<T>(0) };
			for (int x = 0; x < 3; ++x) {
				ret += arr[x][1] * uc[x];
			}
			(*output) = ret;
			++output;

			ret = vec_t{ static_cast<T>(0) };
			for (int x = 0; x < 3; ++x) {
				ret += arr[x][2] * uc[x];
			}
			(*output) = ret;
		};

		// Find a subcurve along the surface.
		template<typename T, typename InIter, typename OutIter>
		void surfaceSubcurveV(InIter row0, InIter row1, InIter row2, T v, OutIter output) {
			static_assert(is_random_iterator<InIter>::value, "The vector valued iterator passed in must be random access!");
			using vec_t = typename is_random_iterator<InIter>::vec_t;

			static_assert(is_output_iterator<OutIter, vec_t>::value, "The iterator passed in is not a proper output iterator!");
			using T = typename is_vec<vec_t>::value_type;

			static_assert(std::is_floating_point<T>::value, "The interpolation value passed in must be a floating point value!");

			T v1 = static_cast<T>(1) - v;

			T vc[3];

			vc[0] = v1 * v1;
			vc[1] = v * v1;
			vc[2] = v * v;

			vec_t arr[3][3];

			arr[0][0] = *row0;
			++row0;
			arr[0][1] = *row0;
			++row0;
			arr[0][2] = *row0;

			arr[1][0] = *row1;
			++row1;
			arr[1][1] = *row1;
			++row1;
			arr[1][2] = *row1;

			arr[2][0] = *row2;
			++row2;
			arr[2][1] = *row2;
			++row2;
			arr[2][2] = *row2;

			vec_t ret{ static_cast<T>(0) };
			for (int y = 0; y < 3; ++y) {
				ret += arr[0][y] * vc[y];
			}
			(*output) = ret;
			++output;

			ret = vec_t{ static_cast<T>(0) };
			for (int y = 0; y < 3; ++y) {
				ret += arr[1][y] * vc[y];
			}
			(*output) = ret;
			++output;

			ret = vec_t{ static_cast<T>(0) };
			for (int y = 0; y < 3; ++y) {
				ret += arr[2][y] * vc[y];
			}
			(*output) = ret;
		};

		// Find a subcurve along the surface.
		template<typename T, typename InIter, typename OutIter>
		void surfaceSubcurveU(InIter row0, InIter row1, InIter row2, InIter row3, T u, OutIter output) {
			static_assert(is_random_iterator<InIter>::value, "The vector valued iterator passed in must be random access!");
			using vec_t = typename is_random_iterator<InIter>::vec_t;

			static_assert(is_output_iterator<OutIter, vec_t>::value, "The iterator passed in is not a proper output iterator!");
			using T = typename is_vec<vec_t>::value_type;

			static_assert(std::is_floating_point<T>::value, "The interpolation value passed in must be a floating point value!");

			T u1 = static_cast<T>(1) - u;

			T uc[4];

			uc[0] = u1 * u1 * u1;
			uc[1] = u * u1 * u1 * static_cast<T>(3);
			uc[2] = u * u * u1 * static_cast<T>(3);
			uc[3] = u * u * u;

			vec_t arr[4][4];

			arr[0][0] = *row0;
			++row0;
			arr[0][1] = *row0;
			++row0;
			arr[0][2] = *row0;
			++row0;
			arr[0][3] = *row0;

			arr[1][0] = *row1;
			++row1;
			arr[1][1] = *row1;
			++row1;
			arr[1][2] = *row1;
			++row1;
			arr[1][3] = *row1;

			arr[2][0] = *row2;
			++row2;
			arr[2][1] = *row2;
			++row2;
			arr[2][2] = *row2;
			++row2;
			arr[2][3] = *row2;

			arr[3][0] = *row3;
			++row3;
			arr[3][1] = *row3;
			++row3;
			arr[3][2] = *row3;
			++row3;
			arr[3][3] = *row3;

			vec_t ret{ static_cast<T>(0) };
			for (int x = 0; x < 4; ++x) {
				ret += arr[x][0] * uc[x];
			}
			(*output) = ret;
			++output;

			ret = vec_t{ static_cast<T>(0) };
			for (int x = 0; x < 4; ++x) {
				ret += arr[x][1] * uc[x];
			}
			(*output) = ret;
			++output;

			ret = vec_t{ static_cast<T>(0) };
			for (int x = 0; x < 4; ++x) {
				ret += arr[x][2] * uc[x];
			}
			(*output) = ret;
			++output;

			ret = vec_t{ static_cast<T>(0) };
			for (int x = 0; x < 4; ++x) {
				ret += arr[x][3] * uc[x];
			}
			(*output) = ret;
		};

		// Find a subcurve along the surface.
		template<typename T, typename InIter, typename OutIter>
		void surfaceSubcurveV(InIter row0, InIter row1, InIter row2, InIter row3, T v, OutIter output) {
			static_assert(is_random_iterator<InIter>::value, "The vector valued iterator passed in must be random access!");
			using vec_t = typename is_random_iterator<InIter>::vec_t;

			static_assert(is_output_iterator<OutIter, vec_t>::value, "The iterator passed in is not a proper output iterator!");
			using T = typename is_vec<vec_t>::value_type;

			static_assert(std::is_floating_point<T>::value, "The interpolation value passed in must be a floating point value!");

			T v1 = static_cast<T>(1) - v;

			T vc[4];

			vc[0] = v1 * v1 * v1;
			vc[1] = v * v1 * v1 * static_cast<T>(3);
			vc[2] = v * v * v1 * static_cast<T>(3);
			vc[3] = v * v * v;

			vec_t arr[4][4];

			arr[0][0] = *row0;
			++row0;
			arr[0][1] = *row0;
			++row0;
			arr[0][2] = *row0;
			++row0;
			arr[0][3] = *row0;

			arr[1][0] = *row1;
			++row1;
			arr[1][1] = *row1;
			++row1;
			arr[1][2] = *row1;
			++row1;
			arr[1][3] = *row1;

			arr[2][0] = *row2;
			++row2;
			arr[2][1] = *row2;
			++row2;
			arr[2][2] = *row2;
			++row2;
			arr[2][3] = *row2;

			arr[3][0] = *row3;
			++row3;
			arr[3][1] = *row3;
			++row3;
			arr[3][2] = *row3;
			++row3;
			arr[3][3] = *row3;

			vec_t ret{ static_cast<T>(0) };
			for (int y = 0; y < 4; ++y) {
				ret += arr[0][y] * vc[y];
			}
			(*output) = ret;
			++output;

			ret = vec_t{ static_cast<T>(0) };
			for (int y = 0; y < 4; ++y) {
				ret += arr[1][y] * vc[y];
			}
			(*output) = ret;
			++output;

			ret = vec_t{ static_cast<T>(0) };
			for (int y = 0; y < 4; ++y) {
				ret += arr[2][y] * vc[y];
			}
			(*output) = ret;
			++output;

			ret = vec_t{ static_cast<T>(0) };
			for (int y = 0; y < 4; ++y) {
				ret += arr[3][y] * vc[y];
			}
			(*output) = ret;
		};
	};
};