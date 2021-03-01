#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <fmt/core.h>
#include <fmt/ostream.h>
#include <fmt/format.h>

#include <ez/geo/AABB.hpp>
#include <ez/geo/Circle.hpp>
#include <ez/geo/Line.hpp>
#include <ez/geo/Orientation.hpp>
#include <ez/geo/Plane.hpp>
#include <ez/geo/Ray.hpp>
#include <ez/geo/Rect.hpp>
#include <ez/geo/Sphere.hpp>
#include <ez/geo/Transform.hpp>

static constexpr float epsf = 1e-4f;
static constexpr double eps = 1e-6;

bool approxEq(float a, float b) {
	return std::abs(a - b) <= epsf;
}
bool approxEq(double a, double b) {
	return std::abs(a - b) <= eps;
}
bool approxEq(const glm::vec2 & a, const glm::vec2 & b) {
	return approxEq(a.x, b.x) && approxEq(a.y, b.y);
}
bool approxEq(const glm::dvec2& a, const glm::dvec2& b) {
	return approxEq(a.x, b.x) && approxEq(a.y, b.y);
}
bool approxEq(const glm::vec3& a, const glm::vec3& b) {
	return approxEq(a.x, b.x) && approxEq(a.y, b.y) && approxEq(a.z, b.z);
}
bool approxEq(const glm::dvec3& a, const glm::dvec3& b) {
	return approxEq(a.x, b.x) && approxEq(a.y, b.y) && approxEq(a.z, b.z);
}

TEST_CASE("aabb test", "[AABB]") {
	fmt::print("Begin aabb test\n");
	using namespace ez;

	{
		AABB2<float> aabb = AABB2<float>::between(glm::vec2{ 2 }, glm::vec2{ 0 });

		REQUIRE(approxEq(aabb.min, glm::vec2{ 0, 0 }));
		REQUIRE(approxEq(aabb.max, glm::vec2{ 2, 2 }));
		REQUIRE(approxEq(aabb.center(), glm::vec2{ 1, 1 }));

		//REQUIRE(approxEq(aabb., glm::vec2{ 1, 1 }));
	}

	{
		AABB2<double> aabb = AABB2<double>::between(glm::dvec2{ 2 }, glm::dvec2{ 0 });

		REQUIRE(approxEq(aabb.min, glm::dvec2{ 0, 0 }));
		REQUIRE(approxEq(aabb.max, glm::dvec2{ 2, 2 }));
		REQUIRE(approxEq(aabb.center(), glm::dvec2{ 1, 1 }));

		//REQUIRE(approxEq(aabb., glm::vec2{ 1, 1 }));
	}

	{
		AABB3<float> aabb = AABB3<float>::between(glm::vec3{ 2 }, glm::vec3{ 0 });

		REQUIRE(approxEq(aabb.min, glm::vec3{ 0, 0, 0 }));
		REQUIRE(approxEq(aabb.max, glm::vec3{ 2, 2, 2 }));
		REQUIRE(approxEq(aabb.center(), glm::vec3{ 1, 1, 1 }));

		//REQUIRE(approxEq(aabb., glm::vec2{ 1, 1 }));
	}

	{
		AABB3<double> aabb = AABB3<double>::between(glm::dvec3{ 2 }, glm::dvec3{ 0 });

		REQUIRE(approxEq(aabb.min, glm::dvec3{ 0, 0, 0 }));
		REQUIRE(approxEq(aabb.max, glm::dvec3{ 2, 2, 2 }));
		REQUIRE(approxEq(aabb.center(), glm::dvec3{ 1, 1, 1 }));

		//REQUIRE(approxEq(aabb., glm::vec2{ 1, 1 }));
	}

	fmt::print("End aabb test\n\n");
}

TEST_CASE("circle test", "[Circle]") {
	using namespace ez;

	{
		Circle<float> circle{ 1, glm::vec2{ 0, 0 } };

		REQUIRE(approxEq(circle.origin, glm::vec2{ 0, 0 }));
	}
}

// Test MMRect
TEST_CASE("MMRect floating point") {
	using Rect = ez::MMRect2<float>;
	using IRect = ez::MMRect2<int>;


	Rect rect0 = Rect::between(glm::vec2{0,0}, glm::vec2{100, 100});
	Rect rect1 = Rect::between(glm::vec2{50, 50}, glm::vec2{100, 100});
	Rect rect2 = Rect::between(glm::vec2{50, 50}, glm::vec2{200, 200});

	REQUIRE(rect0.merged(rect1) == rect0);
	REQUIRE(rect2.contains(rect1));

	REQUIRE(rect0.contains(glm::vec2{0,0}));
	REQUIRE(rect0.contains(glm::vec2{100, 100}));

	Rect rect3 = rect0.merged(rect2);
	REQUIRE(rect3.contains(rect0));
	REQUIRE(rect3.contains(rect2));
	REQUIRE(rect3.contains(rect1));
}

TEST_CASE("MMRect integer") {
	using Rect = ez::MMRect2<int>;

	Rect rect0 = Rect::between(glm::ivec2{ 0,0 }, glm::ivec2{ 100, 100 });
	Rect rect1 = Rect::between(glm::ivec2{ 50, 50 }, glm::ivec2{ 100, 100 });
	Rect rect2 = Rect::between(glm::ivec2{ 50, 50 }, glm::ivec2{ 200, 200 });

	REQUIRE(rect0.merged(rect1) == rect0);
	REQUIRE(rect2.contains(rect1));

	REQUIRE(rect0.contains(rect0.min));
	REQUIRE(rect0.contains(glm::ivec2{ 99, 99 }));
	REQUIRE_FALSE(rect0.contains(rect0.max));

	Rect rect3 = rect0.merged(rect2);
	REQUIRE(rect3.contains(rect0));
	REQUIRE(rect3.contains(rect2));
	REQUIRE(rect3.contains(rect1));
}

// Test Line


// Test Transform