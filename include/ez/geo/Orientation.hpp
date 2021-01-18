#pragma once
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

#include "Ray3d.hpp"

namespace ez {

	template<typename T>
	struct Orientation
	{
		using vec3_t = glm::tvec3<T>;
		using quat_t = glm::tquat<T>;

		static constexpr T eps = ez::epsilon<T>();
		static constexpr T eps1 = static_cast<T>(1) - ez::epsilon<T>();

		Orientation(const quat_t& q = quat_t{})
			: rotation(q)
		{}
		~Orientation() = default;
		Orientation(const Orientation&) = default;
		Orientation& operator=(const Orientation&) = default;
		Orientation(Orientation&&) noexcept = default;
		Orientation& operator=(Orientation&&) noexcept = default;

		static vec3_t worldX() {
			return { static_cast<T>(1), static_cast<T>(0), static_cast<T>(0) };
		}
		static vec3_t worldY() {
			return { static_cast<T>(0), static_cast<T>(1), static_cast<T>(0) };
		}
		static vec3_t worldZ() {
			return { static_cast<T>(0), static_cast<T>(0), static_cast<T>(1) };
		}

		void rotate(const quat_t& localRotation) {
			rotation *= localRotation;
		}
		void rotate(float angle, const vec3_t& axis) {
			quat_t inv = glm::inverse(rotation);
			rotation *= glm::angleAxis(angle, glm::rotate(inv, axis));
		}
		void rotate(const vec3_t& v1, const vec3_t& v2) {
			quat_t rot;
			T det = glm::dot(v1, v2);
			if (det >= eps1) {
				rot = quat_t{};
			}
			else if (det <= -eps1) {
				rot = glm::angleAxis(ez::pi<T>(), vec3_t{ static_cast<T>(1), static_cast<T>(0), static_cast<T>(0) });
			}
			else {
				T s = std::sqrt((static_cast<T>(1) + det) * static_cast<T>(2));
				T invs = static_cast<T>(1) / s;
				vec3_t c = glm::cross(v1, v2);
				rot = {
					s / static_cast<T>(2),
					c * invs
				};
				rot = glm::normalize(rot);
			}
			rotation = rotation * rot;
		}
		void rotate(const Orientation& rot) {
			rotation *= rot.rotation;
		}
		void reset() {
			rotation = quat_t{};
		}
		void normalize() {
			rotation = glm::normalize(rotation);
		}

		void setRotation(const quat_t& q) {
			rotation = q;
		}
		const quat_t& getRotation() const {
			return rotation;
		}

		// All align methods take normalized vectors
		void alignLook(const vec3_t& normVec) {
			alignLocalZ(normVec);
		}
		void alignRight(const vec3_t& normVec) {
			alignLocalX(-normVec);
		}
		void alignUp(const vec3_t& normVec) {
			alignLocalY(normVec);
		}

		void alignLocalX(const vec3_t& normVec) {
			rotate(normVec, getLocalX());
		}
		void alignLocalY(const vec3_t& normVec) {
			rotate(normVec, getLocalY());
		}
		void alignLocalZ(const vec3_t& normVec) {
			rotate(normVec, getLocalZ());
		}
		void alignLocalZY(const vec3_t& zVec, const vec3_t& yVec) {
			reset();
			Orientation rot;

			rotate(zVec, worldZ());
			rot.rotate(worldY(), yVec);

			rotate(rot);
		}
		void alignLocalZX(const vec3_t& zVec, const vec3_t& xVec) {
			reset();
			Orientation rot;

			rotate(zVec, worldZ());
			rot.rotate(worldX(), xVec);

			rotate(rot);
		}
		void alignLocalYZ(const vec3_t& yVec, const vec3_t& zVec) {
			reset();
			Orientation rot;

			rotate(yVec, worldY());
			rot.rotate(worldZ(), zVec);

			rotate(rot);
		}
		void alignLocalYX(const vec3_t& yVec, const vec3_t& xVec) {
			reset();
			Orientation rot;

			rotate(yVec, worldY());
			rot.rotate(worldX(), xVec);

			rotate(rot);
		}
		void alignLocalXZ(const vec3_t& xVec, const vec3_t& zVec) {
			reset();
			Orientation rot;

			rotate(xVec, worldX());
			rot.rotate(worldZ(), zVec);

			rotate(rot);
		}
		void alignLocalXY(const vec3_t& xVec, const vec3_t& yVec) {
			reset();
			Orientation rot;

			rotate(xVec, worldX());
			rot.rotate(worldY(), yVec);

			rotate(rot);
		}

		vec3_t getUpVec() const {
			return glm::rotate(rotation, worldY());
		}
		vec3_t getRightVec() const {
			return glm::rotate(rotation, -worldX());
		}
		vec3_t getLookVec() const {
			return glm::rotate(rotation, worldZ());
		}

		vec3_t getLocalY() const {
			return glm::rotate(rotation, worldY());
		}
		vec3_t getLocalX() const {
			return glm::rotate(rotation, worldX());
		}
		vec3_t getLocalZ() const {
			return glm::rotate(rotation, worldZ());
		}

		glm::tmat3x3<T> getBasis() const {
			glm::tmat3x3<T> ret;
			ret[0] = getLocalX();
			ret[1] = getLocalY();
			ret[2] = getLocalZ();
			return ret;
		}
		glm::tmat4x4<T> getMatrix() const {
			glm::tmat4x4<T> ret;// = glm::mat4_cast(orientation);

			ret[0] = glm::tvec4<T>(getLocalX(), static_cast<T>(0));
			ret[1] = glm::tvec4<T>(getLocalY(), static_cast<T>(0));
			ret[2] = glm::tvec4<T>(getLocalZ(), static_cast<T>(0));

			ret[3] = glm::tvec4<T>(static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(1));
			return ret;
		}

		vec3_t toLocalRotation(const vec3_t& axis) const {
			return glm::rotate(glm::inverse(rotation), axis);
		}
		vec3_t toWorldRotation(const vec3_t& axis) const {
			return glm::rotate(rotation, axis);
		}

		Ray3d<T> toLocalRotation(const Ray3d<T>&) const {
			return Ray3d<T>{ toLocalRotation(r.getAxis()), r.getOrigin() };
		}
		
		Ray3d<T> toWorldRotation(const Ray3d<T>&) const {
			return Ray3d<T>{ toWorldRotation(r.getAxis()), r.getOrigin() };
		}

		quat_t rotation;
	};
};