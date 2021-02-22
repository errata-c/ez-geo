#pragma once
#include <glm/mat2x2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/vec2.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <ez/math/Complex.hpp>
#include "Ray.hpp"


namespace ez {
	template<typename T, int N>
	struct Transform {};

	template<typename T>
	struct Transform<T, 2>
	{
		using vec2_t = glm::tvec2<T>;

		Transform(const vec2_t& nPos = vec2_t{ 0.f }, Complex<T> nRot = Complex<T>{}, const vec2_t& nDim = vec2_t{ 1.f })
			: origin(nPos)
			, rotation(nRot)
			, scale(nDim)
		{}
		~Transform() = default;
		Transform(const Transform&) = default;
		Transform& operator=(const Transform&) = default;
		Transform(Transform&&) noexcept = default;
		Transform& operator=(Transform&&) noexcept = default;

		static vec2_t worldX() {
			return vec2_t{ static_cast<T>(1), static_cast<T>(0)};
		}
		static vec2_t worldY() {
			return vec2_t{ static_cast<T>(0), static_cast<T>(1) };
		}

		void rotate(Complex<T> amount) {
			rotation = amount.rotate(rotation);
		}
		void rotate(T amount) {
			rotation = Complex<T>::fromPolar(amount).rotate(rotation);
		}
		void setRotation(Complex<T> amount) {
			rotation = amount;
		}
		void setRotation(T amount) {
			rotation = Complex<T>::fromPolar(amount);
		}
		void translate(vec2_t off) {
			origin += off;
		}
		void move(vec2_t pos) {
			origin = pos;
		}
		void setPosition(vec2_t pos) {
			origin = pos;
		}
		void normalize() {
			rotation.normalize();
		}
		void scaleBy(T amount) {
			scale *= amount;
		}
		void scaleBy(const vec2_t& amount) {
			scale *= amount;
		}
		void setScale(T amount) {
			scale = vec2_t{amount, amount};
		}
		void setScale(const vec2_t& amount) {
			scale = amount;
		}

		void alignRight(const vec2_t& normVec) {
			rotation = Complex<T>(-normVec.y, normVec.x);
		}
		void alignUp(const vec2_t& normVec) {
			rotation = Complex<T>(normVec.x, normVec.y);
		}

		void alignLocalX(const vec2_t& normVec) {
			rotation = Complex<T>(-normVec.y, normVec.x);
		}
		void alignLocalY(const vec2_t& normVec) {
			rotation = Complex<T>(normVec.x, normVec.y);
		}

		vec2_t getUp() const {
			return rotation.rotate(worldY());
		}
		vec2_t getRight() const {
			return rotation.rotate(worldX());
		}
		vec2_t getLeft() const {
			return rotation.rotate(-worldX());
		}
		vec2_t getDown() const {
			return rotation.rotate(-worldY());
		}

		vec2_t localY() const {
			return rotation.rotate(worldY());
		}
		vec2_t localX() const {
			return rotation.rotate(worldX());
		}

		glm::tmat2x2<T> getBasis() const {
			glm::tmat2x2<T> ret;
			ret[0] = localX();
			ret[1] = localY();
			return ret;
		}
		glm::tmat3x3<T> getMatrix() const {
			glm::tmat3x3<T> ret;
			ret[0] = glm::tvec3<T>(localX() * scale.x, static_cast<T>(0));
			ret[1] = glm::tvec3<T>(localY() * scale.y, static_cast<T>(0));
			ret[2] = glm::tvec3<T>(origin, static_cast<T>(1));
			return ret;
		}
		glm::tmat4x4<T> getMatrix4() const {
			glm::tmat4x4<T> ret;
			ret[0] = glm::tvec4<T>(localX() * scale.x, static_cast<T>(0), static_cast<T>(0));
			ret[1] = glm::tvec4<T>(localY() * scale.y, static_cast<T>(0), static_cast<T>(0));
			ret[2] = glm::tvec4<T>(static_cast<T>(0), static_cast<T>(0), static_cast<T>(1), static_cast<T>(0));
			ret[3] = glm::tvec4<T>(origin, static_cast<T>(0), static_cast<T>(1));
			return ret;
		}
		glm::tmat4x3<T> getMatrix4x3() const {
			glm::tmat4x3<T> ret;
			ret[0] = glm::tvec3<T>(localX() * scale.x, static_cast<T>(0));
			ret[1] = glm::tvec3<T>(localY() * scale.y, static_cast<T>(0));
			ret[2] = glm::tvec3<T>(static_cast<T>(0), static_cast<T>(0), static_cast<T>(1));
			ret[3] = glm::tvec3<T>(origin, static_cast<T>(0));
			return ret;
		}

		glm::tmat4x4<T> getViewMatrix() const {
			glm::tmat4x4<T> ret;
			ret[0] = glm::tvec4<T>(localX() / scale.x, static_cast<T>(0), static_cast<T>(0));
			ret[1] = glm::tvec4<T>(localY() / scale.y, static_cast<T>(0), static_cast<T>(0));
			ret[2] = glm::tvec4<T>(static_cast<T>(0), static_cast<T>(0), static_cast<T>(1), static_cast<T>(0));
			ret[3] = glm::tvec4<T>(-origin.x / scale.x, -origin.y / scale.y, static_cast<T>(0), static_cast<T>(1));
			return ret;
		}

		// Treats input as origin
		vec2_t toLocal(vec2_t point) const {
			point -= origin;
			point = rotation.reverseRotate(point);
			point /= scale;
			return point;
		}
		vec2_t toWorld(vec2_t point) const {
			point *= scale;
			point = rotation.rotate(point);
			point += origin;
			return point;
		}

		// Treats input as vector
		vec2_t toLocalVector(vec2_t axis) const {
			axis = rotation.reverseRotate(axis);
			axis /= scale;
			return axis;
		}
		vec2_t toWorldVector(vec2_t axis) const {
			axis *= scale;
			axis = rotation.rotate(axis);
			return axis;
		}

		Complex<T> toLocal(Complex<T> rot) const {
			return rotation.reverseRotate(rot);
		}
		Complex<T> toWorld(Complex<T> rot) const {
			return rotation.rotate(rot);
		}

		// changes origin and rotation.
		Transform toLocal(const Transform& form) const {
			Transform to;
			to.origin = toLocal(form.origin);
			to.rotation = toLocal(form.rotation);
			return to;
		}
		Transform toWorld(const Transform& form) const {
			Transform2d to;
			to.origin = toWorld(form.origin);
			to.rotation = toWorld(form.rotation);
			to.rotation.normalize();
			return to;
		}

		vec2_t origin, scale;
		Complex<T> rotation;
	};

	template<typename T>
	struct Transform<T, 3>
	{
		using vec3_t = glm::tvec3<T>;
		using quat_t = glm::tquat<T>;

		Transform(const vec3_t& p = vec3_t{0.0}, const quat_t& q = quat_t{1.0, 0.0, 0.0, 0.0})
			: origin(p)
			, rotation(q)
		{}
		~Transform() = default;
		Transform(const Transform&) = default;
		Transform& operator=(const Transform&) = default;
		Transform(Transform&&) noexcept = default;
		Transform& operator=(Transform&&) noexcept = default;

		void translate(const vec3_t& offset) {
			origin += offset;
		}
		void move(const vec3_t& pos) {
			origin = pos;
		}
		void rotate(const quat_t& localQuat) {
			rotation = rotation * localQuat;
		}
		void rotate(T angle, const vec3_t& axis) {
			rotate(glm::angleAxis(angle, axis));
		}
		void rotate(const vec3_t& from, const vec3_t& to) {
			rotate(glm::rotation(from, to));
		}

		void setOrigin(const vec3_t& pos) {
			origin = pos;
		}
		void setRotation(const quat_t& orient) {
			rotation = orient;
		}
		void setRotation(T angle, const vec3_t& axis) {
			rotation = glm::angleAxis(angle, axis);
		}
		void setRotation(const vec3_t& from, const vec3_t& to) {
			rotation = glm::rotation(from, to);
		}

		void reset() {
			origin = vec3_t{ 0 };
			rotation.reset();
		}
		void normalize() {
			rotation.normalize();
		}

		// All align methods take normalized vectors
		void alignLook(const vec3_t& normVec) {
			rotation = glm::quatLookAtLH(normVec, getUpVec());
		}
		void alignRight(const vec3_t& normVec) {
			glm::vec3 look = getLookVec();
			glm::vec3 up = glm::cross(normVec, look);
			rotation = glm::quatLookAtLH(look, up);
		}
		void alignUp(const vec3_t& normVec) {
			rotation = glm::quatLookAtLH(getLookVec(), normVec);
		}

		void alignLocalX(const vec3_t& normVec) {
			alignRight(normVec);
		}
		void alignLocalY(const vec3_t& normVec) {
			alignUp(normVec);
		}
		void alignLocalZ(const vec3_t& normVec) {
			alignLook(normVec);
		}

		void lookAt(const vec3_t& lookDir, const vec3_t& upDir) {
			rotation = glm::quatLookAtLH(lookDir, upDir);
		}

		vec3_t getUpVec() const {
			return glm::rotate(rotation, vec3_t{ 0, 1, 0 });
		}
		vec3_t getRightVec() const {
			return glm::rotate(rotation, vec3_t{ 1, 0, 0 });
		}
		vec3_t getLookVec() const {
			return glm::rotate(rotation, vec3_t{ 0, 0, 1 });
		}

		vec3_t getLocalY() const {
			return getUpVec();
		}
		vec3_t getLocalX() const {
			return getRightVec();
		}
		vec3_t getLocalZ() const {
			return getLookVec();
		}

		glm::tmat3x3<T> getBasis() const {
			return glm::mat3_cast(rotation);
		}
		glm::tmat4x4<T> getMatrix() const {
			glm::tmat4x4<T> ret = glm::mat4_cast(rotation);

			//ret[0] = glm::tvec4<T>(getLocalX(), 0.0);
			//ret[1] = glm::tvec4<T>(getLocalY(), 0.0);
			//ret[2] = glm::tvec4<T>(getLocalZ(), 0.0);

			ret[3] = glm::tvec4<T>(getLocation(), 1.0);
			return ret;
		}
		glm::tmat4x4<T> getScaledMatrix(T s) const {
			glm::tmat4x4<T> ret;

			ret[0] = glm::tvec4<T>(getLocalX() * s, 0.0);
			ret[1] = glm::tvec4<T>(getLocalY() * s, 0.0);
			ret[2] = glm::tvec4<T>(getLocalZ() * s, 0.0);

			ret[3] = glm::tvec4<T>(getLocation(), 1.0);
			return ret;
		}
		glm::tmat4x3<T> getBoneMatrix() const {
			glm::tmat4x3<T> ret;

			ret[0] = getLocalX();
			ret[1] = getLocalY();
			ret[2] = getLocalZ();

			ret[3] = getLocation();

			return ret;
		}
		glm::tmat4x4<T> getViewMatrix() const {
			glm::tmat4x4<T> ret;

			ret[0] = glm::tvec4<T>(-getLocalX(), 0.0);
			ret[1] = glm::tvec4<T>(-getLocalY(), 0.0);
			ret[2] = glm::tvec4<T>(-getLocalZ(), 0.0);

			ret[3] = glm::tvec4<T>(-getLocation(), 1.0);
			return ret;
		}

		const vec3_t& getLocation() const {
			return origin;
		}
		const quat_t& getRotation() const {
			return rotation;
		}

		vec3_t toLocal(const vec3_t& point) const {
			vec3_t modelCoordinate = point - getLocation();
			modelCoordinate = glm::rotate(glm::inverse(rotation), modelCoordinate);
			return modelCoordinate;
		}
		vec3_t toWorld(const vec3_t& point) const {
			vec3_t worldCoordinate = glm::rotate(rotation, point);
			worldCoordinate += getLocation();
			return worldCoordinate;
		}
		vec3_t toLocalRotation(const vec3_t& axis) const {
			return glm::rotate(glm::inverse(rotation), axis);
		}
		vec3_t toWorldRotation(const vec3_t& axis) const {
			return glm::rotate(rotation, axis);
		}
		Transform toLocal(const Transform& form) const {
			Transform ret;
			ret.rotation = getRotation() * form.getRotation();
			ret.origin = toLocal(form.getLocation());
			return ret;
		}
		Transform toWorld(const Transform& form) const {
			Transform ret;
			ret.origin = toWorld(form.getLocation());
			ret.rotation = form.getRotation() * getRotation();
			return ret;
		}

		Ray3<T> toLocal(const Ray3<T>& r) const {
			return Ray3<T>{ toLocal(r.getAxis()), toLocal(r.getOrigin()) };
		}
		Ray3<T> toWorld(const Ray3<T>& r) const {
			return Ray3<T>{ toWorld(r.getAxis()), toWorld(r.getOrigin()) };
		}
		Ray3<T> toLocalRotation(const Ray3<T>& r) const {
			return Ray3<T>{ toLocal(r.getAxis()), r.getOrigin() };
		}
		Ray3<T> toWorldRotation(const Ray3<T>& r) const {
			return Ray3<T>{ toWorld(r.getAxis()), r.getOrigin() };
		}

		vec3_t origin;
		quat_t rotation;
	};

	template<typename T>
	using Transform2 = Transform<T, 2>;

	template<typename T>
	using Transform3 = Transform<T, 3>;
};