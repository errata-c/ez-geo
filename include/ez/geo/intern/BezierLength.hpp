#pragma once
#ifndef EZ_GEO_BEZIER_FUNCTIONS_INCLUDED
#include <type_traits>
#include <glm/gtc/vec1.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/norm.hpp>
#include "BezierUtil.hpp"
#endif

namespace ez {
	namespace Bezier {
		template<typename vec_t, std::enable_if_t<is_vec<vec_t>::value, int> = 0>
		typename is_vec<vec_t>::value_type length(const vec_t& p0, const vec_t& p1) {
			return glm::length(p1 - p0);
		};

		template<typename vec_t, std::enable_if_t<is_vec<vec_t>::value, int> = 0>
		typename is_vec<vec_t>::value_type length(const vec_t& p0, const vec_t& p1, const vec_t& p2) {
			using T = typename is_vec<vec_t>::value_type;
			// Compute 19 points
			constexpr int N = 19;
			constexpr int N1 = N - 1;

			constexpr T delta = static_cast<T>(1) / static_cast<T>(N1);
			constexpr T end = delta * (static_cast<T>(N1) - static_cast<T>(0.5));

			T len = static_cast<T>(0);

			vec_t prior = p0;
			for (T t = delta; t < end; t += delta) {
				vec_t mid = interpolate(p0, p1, p2, t);
				t += delta;
				vec_t post = interpolate(p0, p1, p2, t);

				len += intern::circleArcApprox(glm::length(post - prior), glm::length(mid - prior) + glm::length(post - mid));
				prior = post;
			}

			return len;
		};

		template<typename vec_t, std::enable_if_t<is_vec<vec_t>::value, int> = 0>
		typename is_vec<vec_t>::value_type length(const vec_t& p0, const vec_t& p1, const vec_t& p2, const vec_t& p3) {
			using T = typename is_vec<vec_t>::value_type;

			constexpr int N = 31;
			constexpr int N1 = N - 1;

			constexpr T delta = static_cast<T>(1) / static_cast<T>(N1);
			constexpr T end = delta * (static_cast<T>(N1) - static_cast<T>(0.5));

			T len = static_cast<T>(0);

			vec_t prior = p0;
			for (T t = delta; t < end; t += delta) {
				vec_t mid = interpolate(p0, p1, p2, p3, t);
				t += delta;
				vec_t post = interpolate(p0, p1, p2, p3, t);

				len += intern::circleArcApprox(glm::length(post - prior), glm::length(mid - prior) + glm::length(post - mid));
				prior = post;
			}

			return len;
		}

		template<typename Iter, std::enable_if_t<!is_vec<Iter>::value, int> = 0>
		typename is_random_iterator<Iter>::value_type length(Iter begin, Iter end) {
			static_assert(is_random_iterator<Iter>::value, "The iterator passed in must be random access!");

			using vec_t = typename is_random_iterator<Iter>::vec_t;
			using T = typename is_vec<vec_t>::value_type;

			std::size_t N1 = ((end - begin) - 1) * 10;

			T delta = static_cast<T>(1) / static_cast<T>(N1);
			T last = delta * (static_cast<T>(N1) - static_cast<T>(0.5));

			T len = static_cast<T>(0);

			vec_t prior = p0;
			for (T t = delta; t < end; t += delta) {
				vec_t mid = interpolate(p0, p1, p2, p3, t);
				t += delta;
				vec_t post = interpolate(p0, p1, p2, p3, t);

				len += intern::circleArcApprox(glm::length(post - prior), glm::length(mid - prior) + glm::length(post - mid));
				prior = post;
			}

			return len;
		}
	};
};