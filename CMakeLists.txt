cmake_minimum_required(VERSION 3.16)

project(EZ_GEO
	VERSION 0.4.0
	LANGUAGES C CXX
)

include(FetchContent)

FetchContent_Declare(
	ez-cmake
	GIT_REPOSITORY "https://github.com/errata-c/ez-cmake.git"
	GIT_TAG "main"
)
FetchContent_MakeAvailable(ez-cmake)

option(EZ_GEO_BUILD_TESTS "Build the test executables" OFF)
set(EZ_GEO_CONFIG_DIR "share/ez-geo" CACHE STRING "The relative directory to install package config files.")

find_package(ez-math CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_library(ez-geo STATIC
	"src/IntersectFunctions.cpp"
)

target_include_directories(ez-geo PUBLIC 
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
	"$<INSTALL_INTERFACE:include>"
)

target_compile_features(ez-geo PUBLIC cxx_std_17)

target_link_libraries(ez-geo PUBLIC
	glm::glm
	ez::math
)

target_compile_definitions(ez-geo PRIVATE
	"$<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>"
	"GLM_ENABLE_EXPERIMENTAL"
	"$<$<PLATFORM_ID:Windows>:NOMINMAX>"
)
set_target_properties(ez-geo PROPERTIES EXPORT_NAME "geo")
add_library(ez::geo ALIAS "ez-geo")

if(EZ_GEO_BUILD_TESTS)
	add_subdirectory("tests")
endif()

install(
	DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
	TYPE INCLUDE
	FILES_MATCHING
	PATTERN "*.h" PATTERN "*.hpp"
)

install(TARGETS ez-geo
	EXPORT ez-geo-targets
	RUNTIME DESTINATION "bin/$<CONFIG>"
	ARCHIVE DESTINATION "lib/$<CONFIG>"
	LIBRARY DESTINATION "lib/$<CONFIG>"
)

install_package(
	NAME "ez-geo"
	NAMESPACE "ez::"
	EXPORT "ez-geo-targets"
	VERSION "${EZ_GEO_VERSION}"
	COMPATIBILITY "SameMajorVersion"
	DESTINATION "${EZ_GEO_CONFIG_DIR}"
	PRECONFIG "cmake/preconfig.cmake"
)