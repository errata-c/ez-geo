#include <fmt/core.h>

#include <ez/math/constants.hpp>
#include <ez/geo/Transform.hpp>

#include "util.hpp"

#include <catch2/catch_all.hpp>

using Transform2 = ez::Transform<float, 2>;
using Transform3 = ez::Transform<float, 3>;

TEST_CASE("transform2") {
	Transform2 first, second;

	first.translate(glm::vec2(1, 1));

	second = first.toWorld(second);
	REQUIRE(approxEq(second.origin, first.origin));

	second = first.toLocal(second);
	REQUIRE(approxEq(second.origin, glm::vec2(0)));

	// 90 degree rotation, ccw
	first.rotate(-ez::pi<float>() / 2.f);
	REQUIRE(approxEq(first.localX(), glm::vec2(0, 1)));
	REQUIRE(approxEq(first.localY(), glm::vec2(-1, 0)));

	second = first.toWorld(second);
	REQUIRE(approxEq(second.origin, first.origin));
	REQUIRE(approxEq(glm::angle(first.rotation), glm::angle(second.rotation)));

	second = first.toLocal(second);
	REQUIRE(approxEq(second.origin, glm::vec2(0)));

	second.move(glm::vec2(1, 1));
	// 45 degree rotation, ccw
	second.setRotation(-ez::pi<float>() / 4.f);

	second = first.toWorld(second);
	REQUIRE(approxEq(second.localX(), glm::vec2(-std::sqrt(2.f) / 2.f, std::sqrt(2.f) / 2.f)));
	REQUIRE(approxEq(second.localY(), glm::vec2(-std::sqrt(2.f) / 2.f, -std::sqrt(2.f) / 2.f)));

	REQUIRE(approxEq(second.origin, glm::vec2(0, 2)));

	second.move(glm::vec2(1, 1));
	// 45 degree rotation, ccw
	second.setRotation(-ez::pi<float>() / 4.f);

	auto smat = second.getMatrix();
	auto fmat = first.getMatrix();

	second = first.toWorld(second);

	auto rmat = fmat * smat;
	REQUIRE(approxEq(second.origin, glm::vec2(rmat[2][0], rmat[2][1])));
	REQUIRE(approxEq(second.localX(), glm::vec2(rmat[0][0], rmat[0][1])));
	REQUIRE(approxEq(second.localY(), glm::vec2(rmat[1][0], rmat[1][1])));
}

TEST_CASE("transform3") {
	Transform3 first, second;

	REQUIRE(approxEq(first.localX(), glm::vec3(1, 0, 0)));
	REQUIRE(approxEq(first.localY(), glm::vec3(0, 1, 0)));
	REQUIRE(approxEq(first.localZ(), glm::vec3(0, 0, 1)));

	first.translate(glm::vec3{1, 1, 1});

	second = first.toWorld(second);
	REQUIRE(approxEq(second.origin, first.origin));

	second = first.toLocal(second);
	REQUIRE(approxEq(second.origin, glm::vec3(0)));

	first.move(glm::vec3(0));
	// 90 degree rotation, clockwise around x
	first.rotate(ez::pi<float>() / 2.f, glm::vec3(1, 0, 0));

	REQUIRE(approxEq(first.localX(), glm::vec3(1, 0, 0)));
	REQUIRE(approxEq(first.localY(), glm::vec3(0, 0, 1)));
	REQUIRE(approxEq(first.localZ(), glm::vec3(0, -1, 0)));

	// 90 degree rotation, ccw around the z axis
	first.rotate(-ez::pi<float>() / 2.f, glm::vec3(0, 0, 1));

	REQUIRE(approxEq(first.localX(), glm::vec3(0, -1, 0)));
	REQUIRE(approxEq(first.localY(), glm::vec3(0, 0, 1)));
	REQUIRE(approxEq(first.localZ(), glm::vec3(-1, 0, 0)));

	glm::vec3 rp = first.toLocal(glm::vec3(-1, 0, 0));
	REQUIRE(approxEq(rp, glm::vec3(0, 0, 1)));
	REQUIRE(approxEq(first.toWorld(rp), glm::vec3(-1, 0, 0)));

	rp = first.toLocal(glm::vec3(0, -1, 0));
	REQUIRE(approxEq(rp, glm::vec3(1, 0, 0)));
	REQUIRE(approxEq(first.toWorld(rp), glm::vec3(0, -1, 0)));

}

TEST_CASE("transform 3 alignment") {
	Transform3 form;
	form.alignXY(glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));

	glm::mat3 orient = form.getBasis();

	REQUIRE(approxEq(orient[0], glm::vec3{ 0, 0, -1 }));
	REQUIRE(approxEq(orient[1], glm::vec3{ 0, 1, 0 }));
	REQUIRE(approxEq(orient[2], glm::vec3{ 1, 0, 0 }));

	form.alignXZ(glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
	orient = form.getBasis();

	REQUIRE(approxEq(orient[0], glm::vec3{ 0, 0, -1 }));
	REQUIRE(approxEq(orient[1], glm::vec3{ -1, 0, 0 }));
	REQUIRE(approxEq(orient[2], glm::vec3{ 0, 1, 0 }));

	form.alignYZ(glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
	orient = form.getBasis();

	REQUIRE(approxEq(orient[0], glm::vec3{ 1, 0, 0 }));
	REQUIRE(approxEq(orient[1], glm::vec3{ 0, 0, -1 }));
	REQUIRE(approxEq(orient[2], glm::vec3{ 0, 1, 0 }));

	glm::mat3 mat1 = form.getBasis();
	glm::mat3 mat2 = glm::mat3_cast(form.rotation);

	REQUIRE(approxEq(mat1[0], mat2[0]));
	REQUIRE(approxEq(mat1[1], mat2[1]));
	REQUIRE(approxEq(mat1[2], mat2[2]));
}

TEST_CASE("Local rotations") {
	Transform3 form;

	form.rotate_local(ez::half_pi(), glm::vec3{1, 0, 0});

	glm::mat3 mat = form.getBasis();

	REQUIRE(approxEq(mat[0], glm::vec3{1, 0, 0}));
	REQUIRE(approxEq(mat[1], glm::vec3{0, 0, 1}));
	REQUIRE(approxEq(mat[2], glm::vec3{0, -1, 0}));

	form.rotate_local(ez::half_pi(), glm::vec3{0, 1, 0});

	mat = form.getBasis();

	REQUIRE(approxEq(mat[0], glm::vec3{ 0, 1, 0 }));
	REQUIRE(approxEq(mat[1], glm::vec3{ 0, 0, 1 }));
	REQUIRE(approxEq(mat[2], glm::vec3{ 1, 0, 0 }));
}