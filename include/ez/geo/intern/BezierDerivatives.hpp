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
		template<typename vec_t, std::enable_if_t<is_vec<vec_t>::value, int> = 0>
		vec_t derivativeAt(const vec_t& p0, const vec_t& p1, typename is_vec<vec_t>::value_type t) {
			return p1 - p0;
		};
		template<typename vec_t, std::enable_if_t<is_vec<vec_t>::value, int> = 0>
		vec_t derivativeAt(const vec_t& p0, const vec_t& p1, const vec_t& p2, typename is_vec<vec_t>::value_type t) {
			using T = typename is_vec<vec_t>::value_type;

			return interpolate(static_cast<T>(2) * (p1 - p0), static_cast<T>(2) * (p2 - p1), t);
		};
		template<typename vec_t, std::enable_if_t<is_vec<vec_t>::value, int> = 0>
		vec_t derivativeAt(const vec_t& p0, const vec_t& p1, const vec_t& p2, const vec_t& p3, typename is_vec<vec_t>::value_type t) {
			using T = typename is_vec<vec_t>::value_type;

			return interpolate(static_cast<T>(3.0) * (p1 - p0), static_cast<T>(3.0) * (p2 - p1), static_cast<T>(3.0) * (p3 - p2), t);
		};

		template<typename vec_t, std::enable_if_t<is_vec<vec_t>::value, int> = 0>
		vec_t tangent(const vec_t& p0, const vec_t& p1, typename is_vec<vec_t>::value_type t) {
			return glm::normalize(p1 - p0);
		};
		template<typename T>
		glm::vec<2, T> normal(const glm::vec<2, T>& p0, const glm::vec<2, T>& p1, T t) {
			static_assert(std::is_floating_point<T>::value, "Bezier functions only work for floating point types.");
			glm::tvec2<T> tmp = tangent(p0, p1, t);
			return glm::tvec2<T>{ -tmp.y, tmp.x };
		};

		template<typename vec_t, std::enable_if_t<is_vec<vec_t>::value, int> = 0>
		vec_t tangent(const vec_t& p0, const vec_t& p1, const vec_t& p2, typename is_vec<vec_t>::value_type t) {
			using T = typename is_vec<vec_t>::value_type;
			return glm::normalize(derivativeAt(p0, p1, p2, t));
		};

		template<typename T>
		glm::vec<2, T> normal(const glm::vec<2, T>& p0, const glm::vec<2, T>& p1, const glm::vec<2, T>& p2, T t) {
			static_assert(std::is_floating_point<T>::value, "Bezier functions only work for floating point types.");
			glm::tvec2<T> tmp = tangent(p0, p1, p2, t);
			return glm::tvec2<T>{ -tmp.y, tmp.x };
		};

		template<typename vec_t, std::enable_if_t<is_vec<vec_t>::value, int> = 0>
		vec_t tangent(const vec_t& p0, const vec_t& p1, const vec_t& p2, const vec_t& p3, typename is_vec<vec_t>::value_type t) {
			using T = typename is_vec<vec_t>::value_type;
			return glm::normalize(derivativeAt(p0, p1, p2, p3, t));
		};
		template<typename T>
		glm::vec<2, T> normal(const glm::vec<2, T>& p0, const glm::vec<2, T>& p1, const glm::vec<2, T>& p2, const glm::vec<2, T>& p3, T t) {
			static_assert(std::is_floating_point<T>::value, "Bezier functions only work for floating point types.");
			glm::tvec2<T> tmp = tangent(p0, p1, p2, p3, t);
			return glm::tvec2<T>{ -tmp.y, tmp.x };
		};

		template<typename T, typename Iter, std::enable_if_t<is_random_iterator<Iter>::value && std::is_floating_point<T>::value, int> = 0>
		typename Iter::value_type tangent(Iter begin, Iter end, T t) {
			using vec_t = Iter::value_type;

			std::intptr_t diff = end - begin;
			switch (diff) {
			case 0:
			case 1:
				return vec_t{};
			case 2: // linear
				return glm::normalize(*(begin + 1) - *(begin));
			case 3: // quadratic
				return glm::normalize(Bezier::interpolate(*(begin + 1), *(begin + 2), t) - Bezier::interpolate(*(begin), *(begin + 1), t));
			case 4: // cubic
				return glm::normalize(Bezier::interpolate(*(begin + 1), *(begin + 2), *(begin + 3), t) - Bezier::interpolate(*(begin), *(begin + 1), *(begin + 2), t));
			default:
				// Dynamic:
				return glm::normalize(Bezier::interpolate(begin + 1, end, t) - Bezier::interpolate(begin, end - 1, t));
			}
		};
		template<typename T, typename Iter, std::enable_if_t<is_random_iterator<Iter>::value && std::is_floating_point<T>::value, int> = 0>
		typename Iter::value_type normal(Iter begin, Iter end, T t) {
			using vec_t = Iter::value_type;
	
			static_assert(is_vec<vec_t>::length == 2, "ez::Bezier::normal function requires a two dimensional vector.");

			std::intptr_t diff = end - begin;
			vec_t tmp{ 1, 0 };
			switch (diff) {
			case 0:
				assert(false); // Zero length iterator range passed in.
				break;
			case 1:
				assert(false); // Impossible to find a normal when given just one point.
				break;
			case 2: // linear
				tmp = glm::normalize(*(begin + 1) - *(begin));
				break;
			case 3: // quadratic
				tmp = glm::normalize(Bezier::interpolate(*(begin + 1), *(begin + 2), t) - Bezier::interpolate(*(begin), *(begin + 1), t));
				break;
			case 4: // cubic
				tmp = glm::normalize(Bezier::interpolate(*(begin + 1), *(begin + 2), *(begin + 3), t) - Bezier::interpolate(*(begin), *(begin + 1), *(begin + 2), t));
				break;
			default:
				// Dynamic:
				tmp = glm::normalize(Bezier::interpolate(begin + 1, end, t) - Bezier::interpolate(begin, end - 1, t));
				break;
			}

			tmp = vec_t{ -tmp.y, tmp.x };
			return tmp;
		};

		template<typename vec_t, typename Iter, std::enable_if_t<is_vec<vec_t>::value, int> = 0>
		void derivative(const vec_t& p0, const vec_t& p1, Iter output) {
			static_assert(is_output_iterator<Iter, vec_t>::value, "The iterator passed in is not a proper output iterator!");

			(*output) = p1 - p0;
		};
		template<typename vec_t, typename Iter, std::enable_if_t<is_vec<vec_t>::value, int> = 0>
		void derivative(const vec_t& p0, const vec_t& p1, const vec_t& p2, Iter output) {
			static_assert(is_output_iterator<Iter, vec_t>::value, "The iterator passed in is not a proper output iterator!");

			using T = typename is_vec<vec_t>::value_type;

			(*output) = static_cast<T>(2.0)* (p1 - p0);
			++output;
			(*output) = static_cast<T>(2.0)* (p2 - p1);
		};
		template<typename vec_t, typename Iter, std::enable_if_t<is_vec<vec_t>::value, int> = 0>
		void derivative(const vec_t& p0, const vec_t& p1, const vec_t& p2, const vec_t& p3, Iter output) {
			static_assert(is_output_iterator<Iter, vec_t>::value, "The iterator passed in is not a proper output iterator!");

			using T = typename is_vec<vec_t>::value_type;

			(*output) = static_cast<T>(3.0)* (p1 - p0);
			++output;
			(*output) = static_cast<T>(3.0)* (p2 - p1);
			++output;
			(*output) = static_cast<T>(3.0)* (p3 - p2);
		};
	};
};
