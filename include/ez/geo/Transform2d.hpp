#pragma once
#include <glm/mat2x2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/vec2.hpp>
#include <ez/math/Complex.hpp>

namespace ez {
	template<typename T>
	struct Transform2d
	{
		using vec2_t = glm::tvec2<T>;

		Transform2d(const vec2_t& nPos = vec2_t{ 0.f }, Complex<T> nRot = Complex<T>{}, const vec2_t& nDim = vec2_t{ 1.f })
			: position(nPos)
			, rotation(nRot)
			, scale(nDim)
		{}
		~Transform2d() = default;
		Transform2d(const Transform2d&) = default;
		Transform2d& operator=(const Transform2d&) = default;
		Transform2d( Transform2d&&) noexcept = default;
		Transform2d& operator=( Transform2d&&) noexcept = default;

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
			position += off;
		}
		void move(vec2_t pos) {
			position = pos;
		}
		void setPosition(vec2_t pos) {
			position = pos;
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
			ret[2] = glm::tvec3<T>(position, static_cast<T>(1));
			return ret;
		}
		glm::tmat4x4<T> getMatrix4() const {
			glm::tmat4x4<T> ret;
			ret[0] = glm::tvec4<T>(localX() * scale.x, static_cast<T>(0), static_cast<T>(0));
			ret[1] = glm::tvec4<T>(localY() * scale.y, static_cast<T>(0), static_cast<T>(0));
			ret[2] = glm::tvec4<T>(static_cast<T>(0), static_cast<T>(0), static_cast<T>(1), static_cast<T>(0));
			ret[3] = glm::tvec4<T>(position, static_cast<T>(0), static_cast<T>(1));
			return ret;
		}
		glm::tmat4x3<T> getMatrix4x3() const {
			glm::tmat4x3<T> ret;
			ret[0] = glm::tvec3<T>(localX() * scale.x, static_cast<T>(0));
			ret[1] = glm::tvec3<T>(localY() * scale.y, static_cast<T>(0));
			ret[2] = glm::tvec3<T>(static_cast<T>(0), static_cast<T>(0), static_cast<T>(1));
			ret[3] = glm::tvec3<T>(position, static_cast<T>(0));
			return ret;
		}

		glm::tmat4x4<T> getViewMatrix() const {
			glm::tmat4x4<T> ret;
			ret[0] = glm::tvec4<T>(localX() / scale.x, static_cast<T>(0), static_cast<T>(0));
			ret[1] = glm::tvec4<T>(localY() / scale.y, static_cast<T>(0), static_cast<T>(0));
			ret[2] = glm::tvec4<T>(static_cast<T>(0), static_cast<T>(0), static_cast<T>(1), static_cast<T>(0));
			ret[3] = glm::tvec4<T>(-position.x / scale.x, -position.y / scale.y, static_cast<T>(0), static_cast<T>(1));
			return ret;
		}

		// Treats input as position
		vec2_t toLocal(vec2_t point) const {
			point -= position;
			point = rotation.reverseRotate(point);
			point /= scale;
			return point;
		}
		vec2_t toWorld(vec2_t point) const {
			point *= scale;
			point = rotation.rotate(point);
			point += position;
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

		// changes position and rotation.
		Transform2d toLocal(const Transform2d& form) const {
			Transform2d to;
			to.position = toLocal(form.position);
			to.rotation = toLocal(form.rotation);
			return to;
		}
		Transform2d toWorld(const Transform2d& form) const {
			Transform2d to;
			to.position = toWorld(form.position);
			to.rotation = toWorld(form.rotation);
			to.rotation.normalize();
			return to;
		}

		vec2_t position, scale;
		Complex<T> rotation;
	};
};