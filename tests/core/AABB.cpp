#include <catch2/catch.hpp>

#include <fmt/core.h>
#include <fmt/ostream.h>
#include <fmt/format.h>

#include <ez/geo/AABB.hpp>
#include <ez/geo/Circle.hpp>
#include <ez/geo/Line.hpp>
#include <ez/geo/Plane.hpp>
#include <ez/geo/Ray.hpp>
#include <ez/geo/Rect.hpp>
#include <ez/geo/Sphere.hpp>
#include <ez/geo/Transform.hpp>

#include "util.hpp"

TEST_CASE("aabb test", "[AABB]") {
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


	Rect rect0 = Rect::between(glm::vec2{ 0,0 }, glm::vec2{ 100, 100 });
	Rect rect1 = Rect::between(glm::vec2{ 50, 50 }, glm::vec2{ 100, 100 });
	Rect rect2 = Rect::between(glm::vec2{ 50, 50 }, glm::vec2{ 200, 200 });

	REQUIRE(rect0.merged(rect1) == rect0);
	REQUIRE(rect2.contains(rect1));

	REQUIRE(rect0.contains(glm::vec2{ 0,0 }));
	REQUIRE(rect0.contains(glm::vec2{ 100, 100 }));

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
