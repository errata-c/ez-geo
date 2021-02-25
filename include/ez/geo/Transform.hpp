#pragma once
#include <glm/mat2x2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/vec2.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <ez/math/complex.hpp>
#include "Ray.hpp"


namespace ez {
	template<typename T, int N>
	struct Transform {};

	template<typename T>
	struct Transform<T, 2>
	{
		using vec_t = glm::tvec2<T>;

		Transform(const vec_t& nPos = vec_t{ 0.f }, glm::tcomplex<T> nRot = glm::tcomplex<T>{1, 0}, const vec_t& nDim = vec_t{ 1.f })
			: origin(nPos)
			, rotation(nRot)
			, size(nDim)
		{}
		~Transform() = default;
		Transform(const Transform&) = default;
		Transform& operator=(const Transform&) = default;
		Transform(Transform&&) noexcept = default;
		Transform& operator=(Transform&&) noexcept = default;

		static vec_t worldX() {
			return vec_t{ static_cast<T>(1), static_cast<T>(0)};
		}
		static vec_t worldY() {
			return vec_t{ static_cast<T>(0), static_cast<T>(1) };
		}

		void rotate(glm::tcomplex<T> amount) {
			rotation = amount * rotation;
		}
		void rotate(T amount) {
			rotation = glm::polar(amount) * rotation;
		}
		void translate(vec_t off) {
			origin += off;
		}
		void move(vec_t pos) {
			origin = pos;
		}
		void normalize() {
			rotation.normalize();
		}
		void scale(T amount) {
			size *= amount;
		}
		void scale(const vec_t& amount) {
			size *= amount;
		}

		void setSize(T amount) {
			setScale(vec_t{ amount, amount });
		}
		void setSize(const vec_t& amount) {
			size = amount;
		}
		void setOrigin(vec_t pos) {
			origin = pos;
		}
		void setRotation(glm::tcomplex<T> amount) {
			rotation = amount;
		}
		void setRotation(T amount) {
			rotation = glm::polar(amount);
		}

		void alignRight(const vec_t& normVec) {
			rotation = glm::tcomplex<T>(-normVec.y, normVec.x);
		}
		void alignUp(const vec_t& normVec) {
			rotation = glm::tcomplex<T>(normVec.x, normVec.y);
		}

		void alignLocalX(const vec_t& normVec) {
			rotation = glm::tcomplex<T>(-normVec.y, normVec.x);
		}
		void alignLocalY(const vec_t& normVec) {
			rotation = glm::tcomplex<T>(normVec.x, normVec.y);
		}

		vec_t getUpVector() const {
			return localY();
		}
		vec_t getRightVector() const {
			return localX();
		}

		vec_t localY() const {
			return glm::rotate(rotation, worldY());
		}
		vec_t localX() const {
			return glm::rotate(rotation, worldX());
		}

		const vec_t& getOrigin() const {
			return origin;
		};
		const vec_t& getScale() const {
			return size;
		};
		const glm::tcomplex<T>& getRotation() const {
			return rotation;
		};

		glm::tmat2x2<T> getBasis() const {
			glm::tmat2x2<T> ret;
			ret[0] = localX();
			ret[1] = localY();
			return ret;
		}
		glm::tmat3x3<T> getMatrix() const {
			glm::tmat3x3<T> ret;
			ret[0] = glm::tvec3<T>(localX() * size.x, static_cast<T>(0));
			ret[1] = glm::tvec3<T>(localY() * size.y, static_cast<T>(0));
			ret[2] = glm::tvec3<T>(origin, static_cast<T>(1));
			return ret;
		}
		glm::tmat4x4<T> getMatrix4() const {
			glm::tmat4x4<T> ret;
			ret[0] = glm::tvec4<T>(localX() * size.x, static_cast<T>(0), static_cast<T>(0));
			ret[1] = glm::tvec4<T>(localY() * size.y, static_cast<T>(0), static_cast<T>(0));
			ret[2] = glm::tvec4<T>(static_cast<T>(0), static_cast<T>(0), static_cast<T>(1), static_cast<T>(0));
			ret[3] = glm::tvec4<T>(origin, static_cast<T>(0), static_cast<T>(1));
			return ret;
		}
		glm::tmat4x3<T> getMatrix4x3() const {
			glm::tmat4x3<T> ret;
			ret[0] = glm::tvec3<T>(localX() * size.x, static_cast<T>(0));
			ret[1] = glm::tvec3<T>(localY() * size.y, static_cast<T>(0));
			ret[2] = glm::tvec3<T>(static_cast<T>(0), static_cast<T>(0), static_cast<T>(1));
			ret[3] = glm::tvec3<T>(origin, static_cast<T>(0));
			return ret;
		}

		glm::tmat4x4<T> getViewMatrix() const {
			glm::tmat4x4<T> ret;
			ret[0] = glm::tvec4<T>(localX() / size.x, static_cast<T>(0), static_cast<T>(0));
			ret[1] = glm::tvec4<T>(localY() / size.y, static_cast<T>(0), static_cast<T>(0));
			ret[2] = glm::tvec4<T>(static_cast<T>(0), static_cast<T>(0), static_cast<T>(1), static_cast<T>(0));
			ret[3] = glm::tvec4<T>(-origin.x / size.x, -origin.y / size.y, static_cast<T>(0), static_cast<T>(1));
			return ret;
		}

		// Treats input as origin
		vec_t toLocal(vec_t point) const {
			point -= origin;
			point = glm::rotate(glm::conjugate(rotation), point);
			point /= size;
			return point;
		}
		vec_t toWorld(vec_t point) const {
			point *= size;
			point = glm::rotate(rotation, point);
			point += origin;
			return point;
		}

		// Treats input as vector
		vec_t toLocalVector(vec_t axis) const {
			axis = glm::rotate(glm::conjugate(rotation), axis);
			axis /= size;
			return axis;
		}
		vec_t toWorldVector(vec_t axis) const {
			axis *= size;
			axis = glm::rotate(rotation, axis);
			return axis;
		}

		glm::tcomplex<T> toLocal(glm::tcomplex<T> rot) const {
			return rot / rotation;
		}
		glm::tcomplex<T> toWorld(glm::tcomplex<T> rot) const {
			return rotation * rot;
		}

		// changes origin and rotation.
		Transform toLocal(const Transform& form) const {
			Transform to;
			to.origin = toLocal(form.origin);
			to.rotation = toLocal(form.rotation);
			to.size = to.size / size;
			return to;
		}
		Transform toWorld(const Transform& form) const {
			Transform to;
			to.size = to.size * size;
			to.origin = toWorld(form.origin);
			to.rotation = toWorld(form.rotation);
			return to;
		}

		vec_t origin, size;
		glm::tcomplex<T> rotation;
	};

	template<typename T>
	struct Transform<T, 3>
	{
		using vec_t = glm::tvec3<T>;
		using quat_t = glm::tquat<T>;

		Transform(const vec_t& p = vec_t{ 0.0 }, const vec_t& s = vec_t{ 1.0 }, const quat_t & q = quat_t{ 1.0, 0.0, 0.0, 0.0 })
			: origin(p)
			, size(s)
			, rotation(q)
		{}
		~Transform() = default;
		Transform(const Transform&) = default;
		Transform& operator=(const Transform&) = default;
		Transform(Transform&&) noexcept = default;
		Transform& operator=(Transform&&) noexcept = default;

		static vec_t worldX() noexcept {
			return vec_t{ T(1), T(0), T(0) };
		}
		static vec_t worldY() noexcept {
			return vec_t{ T(0), T(1), T(0) };
		}
		static vec_t worldZ() noexcept {
			return vec_t{ T(0), T(0), T(1) };
		}

		void translate(const vec_t& offset) noexcept {
			origin += offset;
		}
		void move(const vec_t& pos) noexcept {
			origin = pos;
		}

		void rotate(const quat_t& localQuat) noexcept {
			rotation = rotation * localQuat;
		}
		void rotate(T angle, const vec_t& axis) noexcept {
			rotate(glm::angleAxis(angle, axis));
		}
		void rotate(const vec_t& from, const vec_t& to) noexcept {
			rotate(glm::rotation(from, to));
		}

		void scale(T factor) noexcept {
			size *= factor;
		}
		void scale(const vec_t& factor) noexcept {
			size *= factor;
		}

		void setSize(T factor) noexcept {
			size = vec_t{ factor };
		}
		void setSize(const vec_t & factor) noexcept {
			size = factor;
		}

		void setOrigin(const vec_t& pos) noexcept {
			origin = pos;
		}
		void setRotation(const quat_t& orient) noexcept {
			rotation = orient;
		}
		void setRotation(T angle, const vec_t& axis) noexcept {
			rotation = glm::angleAxis(angle, axis);
		}
		void setRotation(const vec_t& from, const vec_t& to) noexcept {
			rotation = glm::rotation(from, to);
		}

		void reset() noexcept {
			origin = vec_t{ 0 };
			rotation = quat_t{1.0, 0.0, 0.0, 0.0};
			size = vec_t{ 1 };
		}
		void normalize() noexcept {
			rotation = glm::normalize(rotation);
		}

		// All align methods take normalized vectors
		void alignLook(const vec_t& normVec) noexcept {
			rotation = glm::quatLookAtLH(normVec, getUpVec());
		}
		void alignRight(const vec_t& normVec) noexcept {
			glm::vec3 look = getLookVec();
			glm::vec3 up = glm::cross(normVec, look);
			rotation = glm::quatLookAtLH(look, up);
		}
		void alignUp(const vec_t& normVec) noexcept {
			rotation = glm::quatLookAtLH(getLookVec(), normVec);
		}

		void alignLocalX(const vec_t& normVec) noexcept {
			alignRight(normVec);
		}
		void alignLocalY(const vec_t& normVec) noexcept {
			alignUp(normVec);
		}
		void alignLocalZ(const vec_t& normVec) noexcept {
			alignLook(normVec);
		}

		void lookAt(const vec_t& lookDir, const vec_t& upDir) noexcept {
			rotation = glm::quatLookAtLH(lookDir, upDir);
		}

		vec_t getUpVector() const noexcept {
			return localY();
		}
		vec_t getRightVector() const noexcept {
			return localX();
		}
		vec_t getLookVector() const noexcept {
			return localZ();
		}

		vec_t localY() const noexcept {
			return glm::rotate(rotation, worldY());
		}
		vec_t localX() const noexcept {
			return glm::rotate(rotation, worldX());
		}
		vec_t localZ() const noexcept {
			return glm::rotate(rotation, worldZ());
		}

		glm::tmat3x3<T> getBasis() const {
			return glm::mat3_cast(rotation);
		}
		glm::tmat4x4<T> getMatrix() const {
			glm::tmat4x4<T> ret = glm::mat4_cast(rotation);
			ret[0] *= size.x;
			ret[1] *= size.y;
			ret[2] *= size.z;

			ret[3] = glm::tvec4<T>(getPosition(), 1.0);
			return ret;
		}
		glm::tmat4x3<T> getBoneMatrix() const {
			glm::tmat4x3<T> ret{
				glm::mat3_cast(rotation),
				origin
			};
			ret[0] *= size.x;
			ret[1] *= size.y;
			ret[2] *= size.z;

			return ret;
		}
		glm::tmat4x4<T> getViewMatrix() const {
			return glm::inverse(getMatrix());
		}

		const vec_t& getPosition() const {
			return origin;
		}
		const quat_t& getRotation() const {
			return rotation;
		}
		const vec_t& getScale() const {
			return size;
		}

		vec_t toLocal(const vec_t& point) const {
			vec_t lcoord = point - getPosition();
			lcoord = glm::rotate(glm::inverse(rotation), lcoord);
			lcoord /= size;
			
			return lcoord;
		}
		vec_t toWorld(const vec_t& point) const {
			vec_t wcoord = point * size;
			wcoord = glm::rotate(rotation, wcoord);
			wcoord += getPosition();
			return wcoord;
		}
		vec_t toLocalRotation(const vec_t& axis) const {
			return glm::rotate(glm::inverse(rotation), axis);
		}
		vec_t toWorldRotation(const vec_t& axis) const {
			return glm::rotate(rotation, axis);
		}
		Transform toLocal(const Transform& form) const {
			Transform ret;
			ret.rotation = getRotation() * form.getRotation();
			ret.origin = toLocal(form.getPosition());
			ret.size = form.getScale() / getScale();
			return ret;
		}
		Transform toWorld(const Transform& form) const {
			Transform ret;
			ret.origin = toWorld(form.getPosition());
			ret.rotation = form.getRotation() * getRotation();
			ret.size = form.getScale() * getScale();
			return ret;
		}

		Ray3<T> toLocal(const Ray3<T>& r) const {
			return Ray3<T>{ toLocalRotation(r.getAxis()), toLocal(r.getOrigin()) };
		}
		Ray3<T> toWorld(const Ray3<T>& r) const {
			return Ray3<T>{ toWorldRotation(r.getAxis()), toWorld(r.getOrigin()) };
		}
		Ray3<T> toLocalRotation(const Ray3<T>& r) const {
			return Ray3<T>{ toLocalRotation(r.getAxis()), r.getOrigin() };
		}
		Ray3<T> toWorldRotation(const Ray3<T>& r) const {
			return Ray3<T>{ toWorldRotation(r.getAxis()), r.getOrigin() };
		}

		vec_t origin, size;
		quat_t rotation;
	};

	template<typename T>
	using Transform2 = Transform<T, 2>;

	template<typename T>
	using Transform3 = Transform<T, 3>;
};