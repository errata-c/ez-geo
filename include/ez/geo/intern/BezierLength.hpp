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
		namespace intern {
			template<typename T>
			T circleArcApprox(T D1, T D2) {
				static constexpr T coeff = static_cast<T>(1.0 / 3.0);
				return D2 + (D2 - D1) * coeff;
			}

			// iterations has to be odd number

			template<typename T>
			constexpr int quadIterations() {
				return 19;
			};
			template<>
			constexpr int quadIterations<double>() {
				return 41;
			};

			template<typename T>
			constexpr int cubicIterations() {
				return 31;
			};
			template<>
			constexpr int cubicIterations<double>() {
				return 61;
			};
		};

		template<typename vec_t, std::enable_if_t<is_vec<vec_t>::value, int> = 0>
		vec_value_t<vec_t> length(const vec_t& p0, const vec_t& p1) {
			return glm::length(p1 - p0);
		};

		template<typename vec_t, std::enable_if_t<is_vec<vec_t>::value, int> = 0>
		vec_value_t<vec_t> length(const vec_t& p0, const vec_t& p1, const vec_t& p2) {
			using T = vec_value_t<vec_t>;
			
			constexpr int N = intern::quadIterations<T>();
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
		vec_value_t<vec_t> length(const vec_t& p0, const vec_t& p1, const vec_t& p2, const vec_t& p3) {
			using T = vec_value_t<vec_t>;

			constexpr int N = intern::cubicIterations<T>();
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
			using T = vec_value_t<vec_t>;

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