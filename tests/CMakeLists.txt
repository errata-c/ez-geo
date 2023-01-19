cmake_minimum_required(VERSION 3.24)
project(EZ_GEO_TESTS)

FetchContent_Declare(
	Catch2
	GIT_REPOSITORY "https://github.com/catchorg/Catch2.git"
	GIT_TAG "v3.2.1"
	FIND_PACKAGE_ARGS CONFIG
)
FetchContent_Declare(
	fmt
	GIT_REPOSITORY "https://github.com/fmtlib/fmt.git"
	GIT_TAG "9.1.0"
	FIND_PACKAGE_ARGS CONFIG
)
FetchContent_MakeAvailable(Catch2 fmt)


add_executable(core_tests 
	"AABB.cpp"
	"transform.cpp"
	"all_compile.cpp"
)
target_link_libraries(core_tests PRIVATE 
	fmt::fmt
	ez::geo
	Catch2::Catch2WithMain
)
