#pragma once
#include "Orientation.hpp"

namespace ez {
	template<typename T>
	struct Transform3d
	{
		using vec3_t = glm::tvec3<T>;
		using quat_t = glm::tquat<T>;

		Transform3d(const vec3_t& p = vec3_t{}, const quat_t &q = quat_t{})
			: position(p)
			, rotation(q)
		{}
		~Transform3d() = default;
		Transform3d(const Transform3d &) = default;
		Transform3d& operator=(const Transform3d&) = default;
		Transform3d(Transform3d&&) noexcept = default;
		Transform3d& operator=(Transform3d&&) noexcept = default;

		void translate(const vec3_t& offset) {
			position += offset;
		}
		void move(const vec3_t& pos) {
			position = pos;
		}
		void rotate(const quat_t& localQuat) {
			rotation.rotate(localQuat);
		}
		void rotate(T angle, const vec3_t& axis) {
			rotation.rotate(angle, axis);
		}
		void rotate(const vec3_t& from, const vec3_t& to) {
			rotation.rotate(from, to);
		}

		void setLocation(const vec3_t& pos) {
			position = pos;
		}
		void setRotation(const quat_t& orient) {
			rotation.setRotation(orient);
		}

		void reset() {
			position = vec3_t{0};
			rotation.reset();
		}
		void normalize() {
			rotation.normalize();
		}

		// All align methods take normalized vectors
		void alignLook(const vec3_t& normVec) {
			rotation.alignLook(normVec);
		}
		void alignRight(const vec3_t& normVec) {
			rotation.alignRight(normVec);
		}
		void alignUp(const vec3_t& normVec) {
			rotation.alignUp(normVec);
		}

		void alignLocalX(const vec3_t& normVec) {
			rotation.alignLocalX(normVec);
		}
		void alignLocalY(const vec3_t& normVec) {
			rotation.alignLocalY(normVec);
		}
		void alignLocalZ(const vec3_t& normVec) {
			rotation.alignLocalZ(normVec);
		}
		void alignLocalZY(const vec3_t& zVec, const vec3_t& yVec) {
			rotation.alignLocalZY(zVec, yVec);
		}
		void alignLocalZX(const vec3_t& zVec, const vec3_t& xVec) {
			rotation.alignLocalZX(zVec, xVec);
		}
		void alignLocalYZ(const vec3_t& yVec, const vec3_t& zVec) {
			rotation.alignLocalYZ(yVec, zVec);
		}
		void alignLocalYX(const vec3_t& yVec, const vec3_t& xVec) {
			rotation.alignLocalYX(yVec, xVec);
		}
		void alignLocalXZ(const vec3_t& xVec, const vec3_t& zVec) {
			rotation.alignLocalXZ(xVec, zVec);
		}
		void alignLocalXY(const vec3_t& xVec, const vec3_t& yVec) {
			rotation.alignLocalXY(xVec, yVec);
		}

		void lookAt(const vec3_t& focus, const vec3_t& up) {
			glm::vec3 lookDir = glm::normalize(focus - getLocation());

			alignLocalZY(lookDir, up);
		}

		vec3_t getUpVec() const {
			return rotation.getUpVec();
		}
		vec3_t getRightVec() const {
			return rotation.getRightVec();
		}
		vec3_t getLookVec() const {
			return rotation.getLookVec();
		}

		vec3_t getLocalY() const {
			return rotation.getLocalY();
		}
		vec3_t getLocalX() const {
			return rotation.getLocalX();
		}
		vec3_t getLocalZ() const {
			return rotation.getLocalZ();
		}

		glm::tmat3x3<T> getBasis() const {
			return rotation.getBasis();
		}
		glm::tmat4x4<T> getMatrix() const {
			glm::tmat4x4<T> ret;

			ret[0] = glm::tvec4<T>(getLocalX(), 0.0);
			ret[1] = glm::tvec4<T>(getLocalY(), 0.0);
			ret[2] = glm::tvec4<T>(getLocalZ(), 0.0);

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
			return position;
		}
		const quat_t& getRotation() const {
			return rotation.getRotation();
		}

		vec3_t toLocal(const vec3_t& point) const {
			vec3_t modelCoordinate = point - getLocation();
			modelCoordinate = glm::rotate(glm::inverse(getRotation()), modelCoordinate);
			return modelCoordinate;
		}
		vec3_t toWorld(const vec3_t& point) const {
			vec3_t worldCoordinate = glm::rotate(getRotation(), point);
			worldCoordinate += getLocation();
			return worldCoordinate;
		}
		vec3_t toLocalRotation(const vec3_t& axis) const {
			return rotation.toLocalRotation(axis);
		}
		vec3_t toWorldRotation(const vec3_t& axis) const {
			return rotation.toWorldRotation(axis);
		}
		Transform3d toLocal(const Transform3d& form) const {
			Transform3d ret;
			ret.rotation = getRotation() * form.getRotation();
			ret.position = toLocal(form.getLocation());
			return ret;
		}
		Transform3d toWorld(const Transform3d& form) const {
			Transform3d ret;
			ret.position = toWorld(form.getLocation());
			ret.rotation = form.getRotation() * getRotation();
			return ret;
		}

		Ray3d<T> toLocal(const Ray3d<T>& r) const {
			return Ray3d<T>{ toLocal(r.getAxis()), toLocal(r.getOrigin()) };
		}
		Ray3d<T> toWorld(const Ray3d<T>& r) const {
			return Ray3d<T>{ toWorld(r.getAxis()), toWorld(r.getOrigin()) };
		}
		Ray3d<T> toLocalRotation(const Ray3d<T>& r) const {
			return Ray3d<T>{ toLocal(r.getAxis()), r.getOrigin() };
		}
		Ray3d<T> toWorldRotation(const Ray3d<T>& r) const {
			return Ray3d<T>{ toWorld(r.getAxis()), r.getOrigin() };
		}
	
		vec3_t position;
		Orientation<T> rotation;
	};

	using Transform3df = Transform3d<float>;
	using Transform3dd = Transform3d<double>;
};