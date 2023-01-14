#pragma once
#include "intern/DimTraits.hpp"
#include "Ray.hpp"

namespace ez {
	/*
	Non-skewed transformation, follows opengl coordinate system for 3d. 
	Left handed coordinates, y is up, x is right, and z is forward.
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

		static vec_t WorldX() noexcept {
			return trait_t::world[0];
		}
		static vec_t WorldY() noexcept {
			return trait_t::world[1];
		}
		template<int K = N, typename = std::enable_if_t<(K == 3)>>
		static vec_t WorldZ() noexcept {
			return trait_t::world[2];
		}

		// A world space rotation that does not affect the origin
		template<int K = N, typename = std::enable_if_t<(K == 2)>>
		Transform& rotate(const std::complex<T>& amount) noexcept {
			rotation = rotation * amount;
			return *this;
		}

		//  A world space rotation that does not affect the origin
		template<int K = N, typename = std::enable_if_t<(K == 3)>>
		Transform& rotate(const glm::tquat<T>& amount) noexcept {
			// This is the 3d quaternion rotation, simple quaternion multiplication performs local space rotation.
			// We just swap the ordering
			rotation = amount * rotation;
			return *this;
		}

		// A world space rotation that does not affect the origin
		// Clockwise
		template<int K = N, typename = std::enable_if_t<(K == 2)>>
		Transform& rotate(T amount) noexcept {
			rotation = glm::polar(-amount) * rotation;
			return *this;
		}
		// A world rotation that does not affect the origin
		// Clockwise around the axis
		template<int K = N, typename = std::enable_if_t<(K == 3)>>
		Transform& rotate(T angle, const vec_t& axis) noexcept {
			rotate(glm::angleAxis(angle, axis));
			return *this;
		}
		// A world rotation that does not affect the origin
		template<int K = N, typename = std::enable_if_t<(K == 3)>>
		Transform& rotate(const vec_t& from, const vec_t& to) noexcept {
			rotate(glm::rotation(from, to));
			return *this;
		}
		
		// A local space rotation
		template<int K = N, typename = std::enable_if_t<(K == 3)>>
		Transform& rotate_local(const glm::tquat<T>& amount) noexcept {
			// This is the 3d quaternion rotation, simple quaternion multiplication performs local space rotation.
			// We just swap the ordering
			rotation *= amount;
			return *this;
		}
		// A local space rotation
		template<int K = N, typename = std::enable_if_t<(K == 3)>>
		Transform& rotate_local(T angle, const vec_t& axis) noexcept {
			rotate_local(glm::angleAxis(angle, axis));
			return *this;
		}
		template<int K = N, typename = std::enable_if_t<(K == 3)>>
		Transform& rotate_local(const vec_t& from, const vec_t& to) noexcept {
			rotateLocal(glm::rotation(from, to));
			return *this;
		}

		// A local translation that is not affected by the scale or rotation.
		Transform& translate(vec_t off) noexcept {
			origin += off;
			return *this;
		}
		// A local translation that is not affected by the scale or rotation.
		Transform& move(vec_t pos) noexcept {
			origin = pos;
			return *this;
		}
		// Renormalize the rotation quaternion
		Transform& normalize() noexcept {
			rotation = glm::normalize(rotation);
			return *this;
		}

		// A local scale that is not affected by the origin or rotation
		Transform& scale(T amount) noexcept {
			size *= amount;
			return *this;
		}
		// A local scale that is not affected by the origin or rotation
		Transform& scale(const vec_t& amount) noexcept {
			size *= amount;
			return *this;
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
		template<int K = N, typename = std::enable_if_t<(K == 2)>>
		void setRotation(T amount) noexcept {
			rotation = glm::polar(-amount);
		}
		template<int K = N, typename = std::enable_if_t<(K == 3)>>
		void setRotation(T angle, const vec_t& axis) noexcept {
			rotation = glm::angleAxis(angle, axis);
		}
		template<int K = N, typename = std::enable_if_t<(K == 3)>>
		void setRotation(const vec_t& from, const vec_t& to) noexcept {
			rotation = glm::rotation(from, to);
		}

		// Rotate from the current orientation to a new one such that the local x-axis is aligned with the vector passed in
		Transform& alignX(const vec_t& normVec) noexcept {
			if constexpr (N == 2) {
				rotation = glm::tcomplex<T>(-normVec.y, normVec.x);
			}
			else {
				glm::vec3 look = localZ();
				glm::vec3 up = glm::cross(normVec, look);
				rotation = glm::quatLookAtLH(look, up);
			}
			return *this;
		}
		// Rotate from the current orientation to a new one such that the local y-axis is aligned with the vector passed in
		Transform& alignY(const vec_t& normVec) noexcept {
			if constexpr (N == 2) {
				rotation = glm::tcomplex<T>(normVec.x, normVec.y);
			}
			else {
				rotation = glm::quatLookAtLH(normVec, -localZ());
			}
			return *this;
		}
		// Rotate from the current orientation to a new one such that the local z-axis is aligned with the vector passed in
		template<int K = N, typename = std::enable_if_t<(K == 3)>>
		Transform& alignZ(const vec_t& normVec) noexcept {
			rotation = glm::quatLookAtLH(normVec, localY());
			return *this;
		}


		template<int K = N, typename = std::enable_if_t<(K == 3)>>
		Transform& alignXY(const vec_t & x, const vec_t & y) noexcept {
			rotation = glm::quatLookAtLH(glm::cross(x, y), y);
			return *this;
		}
		template<int K = N, typename = std::enable_if_t<(K == 3)>>
		Transform& alignXZ(const vec_t& x, const vec_t& z) noexcept {
			rotation = glm::quatLookAtLH(z, glm::cross(z, x));
			return *this;
		}
		template<int K = N, typename = std::enable_if_t<(K == 3)>>
		Transform& alignYZ(const vec_t& y, const vec_t& z) noexcept {
			rotation = glm::quatLookAtLH(z, y);
			return *this;
		}


		// Rotate such that the local y-axis is aimed at the point passed in.
		template<int K = N, typename = std::enable_if_t<(K == 2)>>
		Transform& lookAt(const vec_t& point) noexcept {
			vec_t normVec = glm::normalize(point - origin);
			alignLocalY(normVec);
			return *this;
		}
		// Rotate such that the local z-axis is aimed at the point passed in.
		template<int K = N, typename = std::enable_if_t<(K == 3)>>
		Transform& lookAt(const vec_t& point, const vec_t& up) noexcept {
			vec_t normVec = glm::normalize(point - origin);
			rotation = glm::quatLookAtLH(normVec, up);
			return *this;
		}

		Transform& alignRight(const vec_t& normVec) noexcept {
			return alignX(normVec);
		}
		Transform& alignUp(const vec_t& normVec) noexcept {
			return alignY(normVec);
		}
		template<int K = N, typename = std::enable_if_t<K == 3>>
		Transform& alignLook(const vec_t& normVec) noexcept {
			return alignZ(normVec);
		}


		vec_t getUpVector() const noexcept {
			return localY();
		}
		vec_t getRightVector() const noexcept {
			return localX();
		}
		template<int K = N, typename = std::enable_if_t<K == 3>>
		vec_t getLookVector() const noexcept {
			return localZ();
		}

		vec_t localY() const noexcept {
			return glm::rotate(rotation, WorldY());
		}
		vec_t localX() const noexcept {
			return glm::rotate(rotation, WorldX());
		}
		template<int K = N, typename = std::enable_if_t<K == 3>>
		vec_t localZ() const noexcept {
			return glm::rotate(rotation, WorldZ());
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
			return glm::conjugate(rotation) * rot;
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