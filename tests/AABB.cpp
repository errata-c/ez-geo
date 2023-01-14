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

#include <catch2/catch_all.hpp>

TEST_CASE("aabb test", "[AABB]") {
	using namespace ez;

	{
		AABB2<float> aabb = AABB2<float>::Between(glm::vec2{ 2 }, glm::vec2{ 0 });

		REQUIRE(approxEq(aabb.min, glm::vec2{ 0, 0 }));
		REQUIRE(approxEq(aabb.max, glm::vec2{ 2, 2 }));
		REQUIRE(approxEq(aabb.center(), glm::vec2{ 1, 1 }));

		//REQUIRE(approxEq(aabb., glm::vec2{ 1, 1 }));
	}

	{
		AABB2<double> aabb = AABB2<double>::Between(glm::dvec2{ 2 }, glm::dvec2{ 0 });

		REQUIRE(approxEq(aabb.min, glm::dvec2{ 0, 0 }));
		REQUIRE(approxEq(aabb.max, glm::dvec2{ 2, 2 }));
		REQUIRE(approxEq(aabb.center(), glm::dvec2{ 1, 1 }));

		//REQUIRE(approxEq(aabb., glm::vec2{ 1, 1 }));
	}

	{
		AABB3<float> aabb = AABB3<float>::Between(glm::vec3{ 2 }, glm::vec3{ 0 });

		REQUIRE(approxEq(aabb.min, glm::vec3{ 0, 0, 0 }));
		REQUIRE(approxEq(aabb.max, glm::vec3{ 2, 2, 2 }));
		REQUIRE(approxEq(aabb.center(), glm::vec3{ 1, 1, 1 }));

		//REQUIRE(approxEq(aabb., glm::vec2{ 1, 1 }));
	}

	{
		AABB3<double> aabb = AABB3<double>::Between(glm::dvec3{ 2 }, glm::dvec3{ 0 });

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


	Rect rect0 = Rect::Between(glm::vec2{ 0,0 }, glm::vec2{ 100, 100 });
	Rect rect1 = Rect::Between(glm::vec2{ 50, 50 }, glm::vec2{ 100, 100 });
	Rect rect2 = Rect::Between(glm::vec2{ 50, 50 }, glm::vec2{ 200, 200 });

	REQUIRE(rect0.merged(rect1) == rect0);
	REQUIRE(rect2.contains(rect1));

	REQUIRE(rect0.contains(glm::vec2{ 0,0 }));
	REQUIRE(rect0.contains(glm::vec2{ 100, 100 } - 0.001f));

	Rect rect3 = rect0.merged(rect2);
	REQUIRE(rect3.contains(rect0));
	REQUIRE(rect3.contains(rect2));
	REQUIRE(rect3.contains(rect1));
}

TEST_CASE("MMRect integer") {
	using Rect = ez::MMRect2<int>;

	Rect rect0 = Rect::Between(glm::ivec2{ 0,0 }, glm::ivec2{ 100, 100 });
	Rect rect1 = Rect::Between(glm::ivec2{ 50, 50 }, glm::ivec2{ 100, 100 });
	Rect rect2 = Rect::Between(glm::ivec2{ 50, 50 }, glm::ivec2{ 200, 200 });

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

TEST_CASE("MMRect 1") {
	using Rect = ez::MMRect<float, 1>;

	Rect rect0 = Rect::Between(0.f, 1.f);
	Rect rect1 = Rect::Between(0.5f, 1.f);
	Rect rect2 = Rect::Between(0.5f, 2.f);

	REQUIRE(rect0.valid());
	REQUIRE(rect1.valid());
	REQUIRE(rect2.valid());

	REQUIRE(rect0.merged(rect1) == rect0);
	REQUIRE(rect2.contains(rect1));

	REQUIRE(rect0.contains(0.5f));
	REQUIRE(rect1.contains(0.75f));
	REQUIRE(rect2.contains(1.f));

	REQUIRE_FALSE(rect0.contains(-1.f));
	REQUIRE_FALSE(rect1.contains(2.f));
	REQUIRE_FALSE(rect2.contains(4.f));

	REQUIRE_FALSE(Rect::Between(0.f, 0.f).valid());

	//REQUIRE(rect0.ex)
}
