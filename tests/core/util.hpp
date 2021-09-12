#pragma once
#include <cmath>

#include <fmt/core.h>

static constexpr float epsf = 1e-4f;
static constexpr double eps = 1e-6;

inline bool approxEq(float a, float b) {
	return std::abs(a - b) <= epsf;
}
inline bool approxEq(double a, double b) {
	return std::abs(a - b) <= eps;
}
inline bool approxEq(const glm::vec2& a, const glm::vec2& b) {
	return approxEq(a.x, b.x) && approxEq(a.y, b.y);
}
inline bool approxEq(const glm::dvec2& a, const glm::dvec2& b) {
	return approxEq(a.x, b.x) && approxEq(a.y, b.y);
}
inline bool approxEq(const glm::vec3& a, const glm::vec3& b) {
	return approxEq(a.x, b.x) && approxEq(a.y, b.y) && approxEq(a.z, b.z);
}
inline bool approxEq(const glm::dvec3& a, const glm::dvec3& b) {
	return approxEq(a.x, b.x) && approxEq(a.y, b.y) && approxEq(a.z, b.z);
}
