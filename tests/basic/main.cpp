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
#include <ez/geo/Bezier.hpp>

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

TEST_CASE("bezier length test", "[Bezier]") {
	// Length of this cubic bezier should be exactly 4

	fmt::print("Begin length test\n");
	std::array<glm::dvec2, 4>  d{
		glm::dvec2{-1, -1},
		glm::dvec2{-1, 1},
		glm::dvec2{1, 1},
		glm::dvec2{1, -1},
	};

	std::array<glm::vec2, 4>  f{
		glm::vec2{-1, -1},
		glm::vec2{-1, 1},
		glm::vec2{1, 1},
		glm::vec2{1, -1},
	};

	float flen = 4.f;
	double dlen = 4.0;

	{
		float len = ez::Bezier::length(f[0], f[1], f[2], f[3]);
		fmt::print("Single precision flatten length: {}\n", flen);
		fmt::print("Single precision ez:Bezier::length: {}\n", len);
		REQUIRE(approxEq(flen, len));
	}

	{
		double len = ez::Bezier::length(d[0], d[1], d[2], d[3]);
		fmt::print("Double precision flatten length: {}\n", dlen);
		fmt::print("Double precision ez:Bezier::length: {}\n", len);
		REQUIRE(approxEq(dlen, len));
	}

	fmt::print("End length test\n\n");
}

TEST_CASE("bezier interpolate test", "[Bezier]") {
	std::array<glm::dvec2, 4>  d{
		glm::dvec2{-1, -1},
		glm::dvec2{-1, 1},
		glm::dvec2{1, 1},
		glm::dvec2{1, -1},
	};

	glm::dvec2 p0 = ez::Bezier::interpolate<1>(d.begin(), 0.5);

	glm::dvec2 p1 = ez::Bezier::interpolate<2>(d.begin(), 0.5);

	glm::dvec2 p2 = ez::Bezier::interpolate<3>(d.begin(), 0.5);

	glm::dvec2 p3 = ez::Bezier::interpolate<4>(d.begin(), 0.5);

	glm::dvec2 res0, res1, res2, res3;
	res0 = d[0];
	res1 = (d[0] + d[1]) * 0.5;
	glm::dvec2 i0 = (d[1] + d[2]) * 0.5;
	res2 = (i0 + res1) * 0.5;
	glm::dvec2 i1 = (d[2] + d[3]) * 0.5;
	glm::dvec2 i2 = (i0 + i1) * 0.5;
	res3 = (res2 + i2) * 0.5;

	REQUIRE(approxEq(p0, res0));
	REQUIRE(approxEq(p1, res1));
	REQUIRE(approxEq(p2, res2));
	REQUIRE(approxEq(p3, res3));
}

// Test Line2d
// Test Line3d
// Test Orientation