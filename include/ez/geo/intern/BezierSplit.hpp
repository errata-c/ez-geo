#pragma once
#ifndef EZ_GEO_BEZIER_FUNCTIONS_INCLUDED
#include <type_traits>
#include <glm/gtc/vec1.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/norm.hpp>
#include "BezierTypeChecks.hpp"
#include "BezierInterpolation.hpp"
#endif

namespace ez {
	namespace Bezier {
		// Returns the handles to two Quadratics created from the input quadratic bezier, split at 't'. Creates 5 points, the two segments share a handle.
		template<typename vec_t, typename U, typename Iter>
		void split(const vec_t& p0, const vec_t& p1, const vec_t& p2, U t, Iter output) {
			static_assert(is_vec<vec_t>::value, "The points passed in must be vectors!");
			using T = typename is_vec<vec_t>::value_type;

			static_assert(std::is_floating_point<U>::value, "The interpolation value passed in must be floating point!");

			static_assert(is_output_iterator<Iter, vec_t>::value, "The iterator passed in is not a proper output iterator!");

			vec_t lp = interpolate(p0, p1, t);
			vec_t rp = interpolate(p1, p2, t);

			vec_t mp = interpolate(lp, rp, t);

			(*output) = p0;
			++output;
			(*output) = lp;
			++output;
			(*output) = mp;
			++output;
			(*output) = rp;
			++output;
			(*output) = p2;
		}

		template<typename vec_t, typename U, typename Iter>
		void leftSplit(const vec_t& p0, const vec_t& p1, const vec_t& p2, U t, Iter output) {
			static_assert(is_vec<vec_t>::value, "The points passed in must be vectors!");
			using T = typename is_vec<vec_t>::value_type;

			static_assert(std::is_floating_point<U>::value, "The interpolation value passed in must be floating point!");

			static_assert(is_output_iterator<Iter, vec_t>::value, "The iterator passed in is not a proper output iterator!");
			
			// Is this necessary? Is splitting out of range actually a bad thing?
			assert((t + ez::epsilon<T>()) > 0.f && (t - ez::epsilon<T>()) < 1.f);

			vec_t lp = interpolate(p0, p1, t);
			vec_t rp = interpolate(p1, p2, t);

			vec_t mp = interpolate(lp, rp, t);

			(*output) = p0;
			++output;
			(*output) = lp;
			++output;
			(*output) = mp;
		}

		template<typename vec_t, typename U, typename Iter>
		void rightSplit(const vec_t& p0, const vec_t& p1, const vec_t& p2, U t, Iter output) {
			static_assert(is_vec<vec_t>::value, "The points passed in must be vectors!");
			using T = typename is_vec<vec_t>::value_type;

			static_assert(std::is_floating_point<U>::value, "The interpolation value passed in must be floating point!");

			static_assert(is_output_iterator<Iter, vec_t>::value, "The iterator passed in is not a proper output iterator!");

			assert((t + ez::epsilon<T>()) > 0.f && (t - ez::epsilon<T>()) < 1.f);

			vec_t lp = interpolate(p0, p1, t);
			vec_t rp = interpolate(p1, p2, t);

			vec_t mp = interpolate(lp, rp, t);

			(*output) = mp;
			++output;
			(*output) = rp;
			++output;
			(*output) = p2;
		}

		// Returns the handles to two cubics created from the input cubic bezier, split at 't'.
		template<typename vec_t, typename U, typename Iter>
		void split(const vec_t& p0, const vec_t& p1, const vec_t& p2, const vec_t& p3, U t, Iter output) {
			static_assert(is_vec<vec_t>::value, "The points passed in must be vectors!");
			using T = typename is_vec<vec_t>::value_type;

			static_assert(std::is_floating_point<U>::value, "The interpolation value passed in must be floating point!");

			static_assert(is_output_iterator<Iter, vec_t>::value, "The iterator passed in is not a proper output iterator!");

			assert((t + ez::epsilon<T>()) > static_cast<T>(0) && (t - ez::epsilon<T>()) < static_cast<T>(1));

			std::array<vec_t, 3> d2{
				interpolate(p0, p1, t),
				interpolate(p1, p2, t),
				interpolate(p2, p3, t),
			};

			std::array<vec_t, 2> d1{
				interpolate(d2[0], d2[1], t),
				interpolate(d2[1], d2[2], t),
			};

			glm::vec2 d0 = interpolate(d1[0], d1[1], t);

			(*output) = p0;
			++output;
			(*output) = d2[0];
			++output;
			(*output) = d1[0];
			++output;
			(*output) = d0;
			++output;
			(*output) = d1[1];
			++output;
			(*output) = d2[2];
			++output;
			(*output) = p3;
		}

		template<typename vec_t, typename U, typename Iter>
		void leftSplit(const vec_t& p0, const vec_t& p1, const vec_t& p2, const vec_t& p3, U t, Iter output) {
			static_assert(is_vec<vec_t>::value, "The points passed in must be vectors!");
			using T = typename is_vec<vec_t>::value_type;

			static_assert(std::is_floating_point<U>::value, "The interpolation value passed in must be floating point!");

			static_assert(is_output_iterator<Iter, vec_t>::value, "The iterator passed in is not a proper output iterator!");

			assert((t + ez::epsilon<T>()) > static_cast<T>(0) && (t - ez::epsilon<T>()) < static_cast<T>(1));

			std::array<vec_t, 3> d2{
				interpolate(p0, p1, static_cast<T>(t)),
				interpolate(p1, p2, static_cast<T>(t)),
				interpolate(p2, p3, static_cast<T>(t)),
			};

			std::array<vec_t, 2> d1{
				interpolate(d2[0], d2[1], static_cast<T>(t)),
				interpolate(d2[1], d2[2], static_cast<T>(t)),
			};

			vec_t d0 = interpolate(d1[0], d1[1], static_cast<T>(t));

			(*output) = p0;
			++output;
			(*output) = d2[0];
			++output;
			(*output) = d1[0];
			++output;
			(*output) = d0;
		}

		template<typename vec_t, typename U, typename Iter>
		void rightSplit(const vec_t& p0, const vec_t& p1, const vec_t& p2, const vec_t& p3, U t, Iter output) {
			static_assert(is_vec<vec_t>::value, "The points passed in must be vectors!");
			using T = typename is_vec<vec_t>::value_type;

			static_assert(std::is_floating_point<U>::value, "The interpolation value passed in must be floating point!");

			static_assert(is_output_iterator<Iter, vec_t>::value, "The iterator passed in is not a proper output iterator!");

			assert((t + ez::epsilon<U>()) > static_cast<T>(0) && (t - ez::epsilon<U>()) < static_cast<T>(1));

			std::array<vec_t, 3> d2{
				interpolate(p0, p1, t),
				interpolate(p1, p2, t),
				interpolate(p2, p3, t),
			};

			std::array<vec_t, 2> d1{
				interpolate(d2[0], d2[1], t),
				interpolate(d2[1], d2[2], t),
			};

			vec_t d0 = interpolate(d1[0], d1[1], t);

			(*output) = d0;
			++output;
			(*output) = d1[1];
			++output;
			(*output) = d2[2];
			++output;
			(*output) = p3;
		}


		template<typename vec_t, typename U, typename  Iter>
		void segment(const vec_t& p0, const vec_t& p1, const vec_t& p2, U t0, U t1, Iter output) {
			static_assert(is_vec<vec_t>::value, "The points passed in must be vectors!");
			using T = typename is_vec<vec_t>::value_type;

			static_assert(std::is_floating_point<U>::value, "The interpolation value passed in must be floating point!");

			static_assert(is_output_iterator<Iter, vec_t>::value, "The iterator passed in is not a proper output iterator!");

			assert(t0 < t1);

			t0 = t0 / t1;

			std::array<vec_t, 3> tmp;
			leftSplit(p0, p1, p2, t1, tmp.begin());
			rightSplit(tmp[0], tmp[1], tmp[2], t0, output);
		};

		template<typename vec_t, typename U, typename  Iter>
		void segment(const vec_t& p0, const vec_t& p1, const vec_t& p2, const vec_t p3, U t0, U t1, Iter output) {
			static_assert(is_vec<vec_t>::value, "The points passed in must be vectors!");
			using T = typename is_vec<vec_t>::value_type;

			static_assert(std::is_floating_point<U>::value, "The interpolation value passed in must be floating point!");

			static_assert(is_output_iterator<Iter, vec_t>::value, "The iterator passed in is not a proper output iterator!");

			assert(t0 < t1);

			t0 = t0 / t1;

			std::array<vec_t, 4> tmp;
			leftSplit(p0, p1, p2, p3, t1, tmp.begin());
			rightSplit(tmp[0], tmp[1], tmp[2], tmp[3], t0, output);
		};
	};
}