#pragma once
#include "intern/DimTraits.hpp"
#include "Ray.hpp"

namespace ez {
	/*
	Non-skewed transformation, follows opengl coordinate system for 3d. 
	This means y is up, z is the look direction, x is right.
	*/
	template<typename T, int N>
	struct Transform {
		static_assert(N == 2 || N == 3, "Invalid dimension parameter (N) for ez::Transform template! Must be 2 or 3.");

		using trait_t = intern::DimTraits<T, N>;
		using vec_t = typename trait_t::vec_t;
		using rot_t = typename trait_t::rot_t;
		using basis_t = typename trait_t::basis_t;
		using matrix_t = typename trait_t::matrix_t;

		Transform(
			const vec_t& nPos = vec_t{ 0.f }, 
			const rot_t& nRot = trait_t::defaultRotation(), 
			const vec_t& nDim = vec_t{ 1.f }
		) noexcept
			: origin(nPos)
			, rotation(nRot)
			, size(nDim)
		{}
		~Transform() = default;
		Transform(const Transform&) noexcept = default;
		Transform& operator=(const Transform&) noexcept = default;
		Transform(Transform&&) noexcept = default;
		Transform& operator=(Transform&&) noexcept = default;

		static vec_t worldX() noexcept {
			return trait_t::world[0];
		}
		static vec_t worldY() noexcept {
			return trait_t::world[1];
		}
		template<typename = std::enable_if_t<N == 3>>
		static vec_t worldZ() noexcept {
			return trait_t::world[2];
		}

		void rotate(const rot_t & amount) noexcept {
			rotation = rotation * amount;
		}
		template<typename = std::enable_if_t<N == 2>>
		void rotate(T amount) noexcept {
			rotation = glm::polar(amount) * rotation;
		}
		template<typename = std::enable_if_t<N == 3>>
		void rotate(T angle, const vec_t& axis) noexcept {
			rotate(glm::angleAxis(angle, axis));
		}
		template<typename = std::enable_if_t<N == 3>>
		void rotate(const vec_t& from, const vec_t& to) noexcept {
			rotate(glm::rotation(from, to));
		}

		void translate(vec_t off) noexcept {
			origin += off;
		}
		void move(vec_t pos) noexcept {
			origin = pos;
		}
		void normalize() noexcept {
			rotation = glm::normalize(rotation);
		}

		void scale(T amount) noexcept {
			size *= amount;
		}
		void scale(const vec_t& amount) noexcept {
			size *= amount;
		}

		void setSize(T amount) noexcept {
			setSize(vec_t{ amount });
		}
		void setSize(const vec_t& amount) noexcept {
			size = amount;
		}

		void setOrigin(vec_t pos) noexcept {
			origin = pos;
		}
		void setRotation(const rot_t & amount) noexcept {
			rotation = amount;
		}
		template<typename = std::enable_if_t<N == 2>>
		void setRotation(T amount) noexcept {
			rotation = glm::polar(amount);
		}
		template<typename = std::enable_if_t<N == 3>>
		void setRotation(T angle, const vec_t& axis) noexcept {
			rotation = glm::angleAxis(angle, axis);
		}
		template<typename = std::enable_if_t<N == 3>>
		void setRotation(const vec_t& from, const vec_t& to) noexcept {
			rotation = glm::rotation(from, to);
		}

		void alignLocalX(const vec_t& normVec) noexcept {
			if constexpr (N == 2) {
				rotation = glm::tcomplex<T>(-normVec.y, normVec.x);
			}
			else {
				glm::vec3 look = localZ();
				glm::vec3 up = glm::cross(normVec, look);
				rotation = glm::quatLookAtLH(look, up);
			}
			
		}
		void alignLocalY(const vec_t& normVec) noexcept {
			if constexpr (N == 2) {
				rotation = glm::tcomplex<T>(normVec.x, normVec.y);
			}
			else {
				rotation = glm::quatLookAtLH(normVec, -localZ());
			}
		}
		template<typename = std::enable_if_t<N == 3>>
		void alignLocalZ(const vec_t& normVec) noexcept {
			rotation = glm::quatLookAtLH(normVec, localY());
		}

		template<typename = std::enable_if_t<N == 2>>
		void lookAt(const vec_t& point) noexcept {
			vec_t normVec = glm::normalize(point - origin);
			alignLocalY(normVec);
		}
		template<typename = std::enable_if_t<N == 3>>
		void lookAt(const vec_t& point, const vec_t& up) noexcept {
			vec_t normVec = glm::normalize(point - origin);
			rotation = glm::quatLookAtLH(normVec, up);
		}

		void alignRight(const vec_t& normVec) noexcept {
			alignLocalX(normVec);
		}
		void alignUp(const vec_t& normVec) noexcept {
			alignLocalY(normVec);
		}
		template<typename = std::enable_if_t<N == 3>>
		void alignLook(const vec_t& normVec) noexcept {
			alignLocalZ(normVec);
		}


		vec_t getUpVector() const noexcept {
			return localY();
		}
		vec_t getRightVector() const noexcept {
			return localX();
		}
		template<typename = std::enable_if_t<N == 3>>
		vec_t getLookVector() const noexcept {
			return localZ();
		}

		vec_t localY() const noexcept {
			return glm::rotate(rotation, worldY());
		}
		vec_t localX() const noexcept {
			return glm::rotate(rotation, worldX());
		}
		template<typename = std::enable_if_t<N == 3>>
		vec_t localZ() const noexcept {
			return glm::rotate(rotation, worldZ());
		}

		basis_t getBasis() const noexcept {
			basis_t ret;
			for (int i = 0; i < N; ++i) {
				ret[i] = glm::rotate(rotation, trait_t::world[i]);
			}
			return ret;
		}
		matrix_t getMatrix() const noexcept {
			matrix_t ret;
			for (int i = 0; i < N; ++i) {
				ret[i] = typename matrix_t::col_type{glm::rotate(rotation, trait_t::world[i]) * size[i], T(0)};
			}
			ret[N] = typename matrix_t::col_type{origin, T(1)};
			return ret;
		}

		glm::tmat4x4<T> getMatrix4x4() const noexcept {
			if constexpr (N == 2) {
				glm::tmat4x4<T> ret;
				ret[0] = glm::tvec4<T>(localX() * size.x, T(0), T(0));
				ret[1] = glm::tvec4<T>(localY() * size.y, T(0), T(0));
				ret[2] = glm::tvec4<T>(T(0), T(0), T(1), T(0));
				ret[3] = glm::tvec4<T>(origin, T(0), T(1));
				return ret;
			}
			else {
				return getMatrix();
			}
		}
		glm::tmat4x3<T> getMatrix4x3() const noexcept {
			if constexpr (N == 2) {
				glm::tmat4x3<T> ret;
				ret[0] = glm::tvec3<T>(localX() * size.x, T(0));
				ret[1] = glm::tvec3<T>(localY() * size.y, T(0));
				ret[2] = glm::tvec3<T>(T(0), T(0), T(1));
				ret[3] = glm::tvec3<T>(origin, T(0));
				return ret;
			}
			else {
				glm::tmat4x3<T> ret{
					glm::mat3_cast(rotation),
					origin
				};
				ret[0] *= size.x;
				ret[1] *= size.y;
				ret[2] *= size.z;

				return ret;
			}
		}

		glm::tmat4x4<T> getViewMatrix() const noexcept {
			if constexpr (N == 2) {
				glm::tmat4x4<T> ret;
				ret[0] = glm::tvec4<T>(localX() / size.x, T(0), T(0));
				ret[1] = glm::tvec4<T>(localY() / size.y, T(0), T(0));
				ret[2] = glm::tvec4<T>(T(0), T(0), T(1), T(0));
				ret[3] = glm::tvec4<T>(-origin.x / size.x, -origin.y / size.y, T(0), T(1));
				return ret;
			}
			else {
				return glm::inverse(getMatrix());
			}
		}

		// Treats input as a position in world space
		vec_t toLocal(vec_t point) const noexcept {
			point -= origin;
			point = glm::rotate(glm::conjugate(rotation), point);
			point /= size;
			return point;
		}
		// Treats input as a position in local space
		vec_t toWorld(vec_t point) const noexcept {
			point *= size;
			point = glm::rotate(rotation, point);
			point += origin;
			return point;
		}

		// Treats input as vector in world space
		vec_t toLocalVector(vec_t axis) const noexcept {
			axis = glm::rotate(glm::conjugate(rotation), axis);
			axis /= size;
			return axis;
		}
		// Treats input as vector in local space
		vec_t toWorldVector(vec_t axis) const noexcept {
			axis *= size;
			axis = glm::rotate(rotation, axis);
			return axis;
		}

		rot_t toLocal(const rot_t& rot) const noexcept {
			return rot / rotation;
		}
		rot_t toWorld(const rot_t& rot) const noexcept {
			return rotation * rot;
		}

		// Note that this does not skew.
		Transform toLocal(const Transform& form) const noexcept {
			Transform to;
			to.origin = toLocal(form.origin);
			to.rotation = toLocal(form.rotation);
			to.size = to.size / size;
			return to;
		}
		// Note that this does not skew.
		Transform toWorld(const Transform& form) const noexcept {
			Transform to;
			to.size = to.size * size;
			to.origin = toWorld(form.origin);
			to.rotation = toWorld(form.rotation);
			return to;
		}

		vec_t origin, size;
		rot_t rotation;
	};

	template<typename T>
	using Transform2 = Transform<T, 2>;

	template<typename T>
	using Transform3 = Transform<T, 3>;
};