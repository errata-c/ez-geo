#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <fmt/core.h>
#include <fmt/ostream.h>
#include <fmt/format.h>

#include <ez/geo/Color.hpp>

TEST_CASE("u8 color") {
	using Color = ez::ColorU;

	REQUIRE(Color{} == Color{ 0, 0, 0, 255 });

	// ARGB format for u32
	REQUIRE(Color::fromU32(0xFF00'0000) == Color{ 0, 0, 0, 255 });
	REQUIRE(Color::fromU32(0x00FF'0000) == Color{ 255, 0, 0, 0 });
	REQUIRE(Color::fromU32(0x0000'FF00) == Color{ 0, 255, 0, 0 });
	REQUIRE(Color::fromU32(0x0000'00FF) == Color{ 0, 0, 255, 0 });

	REQUIRE(Color::fromHex("") == Color{ 0,0,0,255 });
	REQUIRE(Color::fromHex("F") == Color{ 255 });
	REQUIRE(Color::fromHex("FF") == Color{ 255 });
	REQUIRE(Color::fromHex("FFF") == Color{ 255, 255, 255 });
	REQUIRE(Color::fromHex("0FFF") == Color{ 255, 255, 255, 0 });

	REQUIRE(Color::fromHex("0xFF00'00FF") == Color{ 0, 0, 255, 255 });

	REQUIRE(Color::fromHex("#FF00FF") == Color{ 255, 0, 255 });
	REQUIRE(Color::fromHex("F0F") == Color{ 255, 0, 255 });

	REQUIRE(Color{ 255, 255, 255 } == Color{ ez::ColorF{1.f, 1.f, 1.f} });
}

TEST_CASE("float color") {
	using Color = ez::ColorF;
	using ColorU = ez::ColorU;

	// Check all 6 sections, [0, 60] [60, 120] [120, 180] [180, 240] [240, 300] [300, 360]

	REQUIRE(ColorU{ Color::fromHSV(22, 0.59, 0.79) } == ColorU{ 201, 126, 83 });
	REQUIRE(ColorU{ Color::fromHSV(117, 0.54, 0.76) } == ColorU{ 94, 194, 89 });
	REQUIRE(ColorU{ Color::fromHSV(157, 0.70, 0.71) } == ColorU{ 54, 181, 132 });

	REQUIRE(ColorU{ Color::fromHSV(235, 0.82, 0.35) } == ColorU{ 16, 22, 89 });
	REQUIRE(ColorU{ Color::fromHSV(262, 0.92, 0.63) } == ColorU{ 67, 13, 161 });
	REQUIRE(ColorU{ Color::fromHSV(329, 0.65, 0.91) } == ColorU{ 232, 81, 159 });
}