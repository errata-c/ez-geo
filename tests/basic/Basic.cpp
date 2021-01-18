#include <fmt/format.h>

#include <ez/geo/AABB2d.hpp>
#include <ez/geo/AABB3d.hpp>
#include <ez/geo/Circle.hpp>
#include <ez/geo/Line2d.hpp>
#include <ez/geo/Line3d.hpp>
#include <ez/geo/Orientation.hpp>
#include <ez/geo/Plane.hpp>
#include <ez/geo/Ray3d.hpp>
#include <ez/geo/Rect.hpp>
#include <ez/geo/Sphere.hpp>
#include <ez/geo/Transform2d.hpp>
#include <ez/geo/Transform3d.hpp>
#include <ez/geo/Bezier.hpp>

int main() {
	fmt::print("Basic\n");
	
	using namespace ez;

	AABB2d<float> aabb2df{ glm::vec2{2, 2} };
	AABB2d<double> aabb2dd{ glm::dvec2{2, 2} };

	AABB3d<float> aabb3df{ glm::vec3{2, 2, 2} };
	AABB3d<double> aabb3dd{ glm::dvec3{2, 2, 2} };

	Circle<float> circlef{ 1.f, glm::vec2{0, 0} };
	Circle<double> circled{ 1.0, glm::dvec2{0, 0} };

	Line2d<float> line2df{ glm::vec2{}, glm::vec2{} };
	Line2d<double> line2dd{ glm::dvec2{}, glm::dvec2{} };

	Line3d<float> line3df(glm::vec3{}, glm::vec3{});
	Line3d<double> line3dd(glm::dvec3{}, glm::dvec3{});

	Orientation<float> orientf{};
	Orientation<double> orientd{};

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

	glm::dvec2 p0 = ez::Bezier::interpolate<1>(d.begin(), 0.5);

	glm::dvec2 p2 = ez::Bezier::interpolate<2>(d.begin(), 0.5);

	glm::dvec2 p3 = ez::Bezier::interpolate<3>(d.begin(), 0.5);

	glm::dvec2 p4 = ez::Bezier::interpolate<4>(d.begin(), 0.5);

	float p5 = ez::Bezier::interpolate(0.f, 1.f, 0.5f);

	fmt::print("Expected value: 0.5, actual value: {}\n", p5);

	float len0 = ez::Bezier::length(glm::vec2{0,0}, glm::vec2{1, 1}, glm::vec2{2, 0});
	float len1 = ez::Bezier::length(0.f, 1.f, 3.f);

	float len2 = ez::Bezier::length(0.f, 1.f);

	fmt::print("len0 == {}, len1 == {}, len2 == {}\n", len0, len1, len2);

	return 0;
}