#pragma once
#ifndef EZ_GEO_BEZIER_FUNCTIONS_INCLUDED
#include <type_traits>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/norm.hpp>
#endif

namespace ez {
	namespace Bezier {
		// Vector type checks, decltype allows using expessions in type substitutions.
		template<typename T, typename = int>
		struct is_vec : public std::false_type {};
		template<typename T>
		struct is_vec<T, decltype((void)T::length(), (typename T::value_type*)0, 0)> {
			static_assert(std::is_floating_point<typename T::value_type>::value, "Bezier functions only work for floating point types.");
			static constexpr bool value = std::is_same<T, glm::vec<T::length(), typename T::value_type>>::value;
			using value_type = typename T::value_type;
			static constexpr std::size_t length = T::length();
		};
		template<>
		struct is_vec<float, int> {
			static constexpr bool value = true;
			using value_type = float;
			static constexpr std::size_t length = 1;
		};
		template<>
		struct is_vec<double, int> {
			static constexpr bool value = true;
			using value_type = double;
			static constexpr std::size_t length = 1;
		};

		template<typename T>
		inline constexpr bool is_vec_v = is_vec<T>::value;
		template<typename T>
		using vec_value_t = typename is_vec<T>::value_type;


		// Random access iterator to valid vec type
		template <typename T, typename = int>
		struct is_random_iterator : public std::false_type {
			using vec_t = void;
			using value_type = void;
		};
		template <typename T>
		struct is_random_iterator <T, decltype((typename T::iterator_category*)0, 0)> {
			static constexpr bool value = std::is_same<typename T::iterator_category, std::random_access_iterator_tag>::value && is_vec<typename T::value_type>::value;
			using vec_t = typename T::value_type;
			using value_type = typename is_vec<vec_t>::value_type;
		};
		template <typename T>
		struct is_random_iterator <T*, int> {
			static constexpr bool value = is_vec<T>::value;
			using vec_t = T;
			using value_type = typename is_vec<vec_t>::value_type;
		};

		template<typename T>
		inline constexpr bool is_random_iterator_v = is_random_iterator<T>::value;
		template<typename T>
		using random_iterator_value_t = typename is_random_iterator<T>::value_type;



		// Input iterator to valid vec type
		template <typename T, typename = int>
		struct is_input_iterator : public std::false_type {};
		template <typename T>
		struct is_input_iterator <T, decltype((typename T::iterator_category*)0, 0)> {
			static constexpr bool value = !std::is_same<typename T::iterator_category, std::output_iterator_tag>::value && is_vec<typename T::value_type>::value;
		};
		template <typename T>
		struct is_input_iterator <T*, int> {
			static constexpr bool value = is_vec<T>::value;
		};

		template<typename T>
		inline constexpr bool is_input_iterator_v = is_input_iterator<T>::value;
		template<typename T>
		using input_iterator_value_t = typename is_input_iterator<T>::value_type;


		// Output iterator 
		template <typename T, typename vec_t, typename = int>
		struct is_output_iterator : public std::false_type {};

		template <typename T, typename vec_t>
		struct is_output_iterator <T, vec_t, decltype((typename T::iterator_category*)0, 0) > {
			static constexpr bool value =
				(!std::is_same<typename T::iterator_category, std::input_iterator_tag>::value) &&
				(std::is_same<typename T::value_type, vec_t>::value || std::is_same<typename T::value_type, void>::value);
			// Check for void, because back_insert_iterators make their value_type void. As far as I know, no other iterators do this.
			// TODO: need a good way to check for container_type. 
		};
		// Simple pointer overload.
		template <typename T, typename vec_t>
		struct is_output_iterator <T*, vec_t, int> {
			static constexpr bool value = std::is_same<T, vec_t>::value;
		};
	};
};