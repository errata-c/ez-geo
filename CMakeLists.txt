cmake_minimum_required(VERSION 3.14)

project(EZ_GEO
VERSION 0.4.0
LANGUAGES C CXX)

include(GNUInstallDirs)
find_package(ez-cmake CONFIG REQUIRED)

set(PACK_NAME "ez-geo")
set(CONFIG_DIR "${CMAKE_INSTALL_DATAROOTDIR}/${PACK_NAME}" CACHE STRING "The relative directory to install ${PACK_NAME} package config files.")
set(BUILD_TESTS OFF CACHE BOOL "Build the tests for ${PACK_NAME} library.")

find_package(ez-math CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_library(ez-geo STATIC
"src/IntersectFunctions.cpp"
)

target_include_directories(ez-geo PUBLIC 
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
	"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_compile_features(ez-geo PUBLIC cxx_std_17)

target_link_libraries(ez-geo 
PUBLIC glm ez-math
)

target_compile_definitions(ez-geo PRIVATE
	"$<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>"
	"GLM_ENABLE_EXPERIMENTAL"
	"$<$<PLATFORM_ID:Windows>:NOMINMAX>"
)
#target_compile_options(ez-geo PRIVATE "$<BUILD_INTERFACE:$<$<CXX_COMPILER_ID:MSVC>:/permissive->>")

if(${BUILD_TESTS})
	#add_subdirectory("tests")
endif()


set(CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/ez-geo)

install(
	DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
	TYPE INCLUDE
	FILES_MATCHING
	PATTERN "*.h" PATTERN "*.hpp"
)

install(TARGETS ez-geo
	EXPORT ez-geo-targets
	RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}/$<CONFIG>"
	ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}/$<CONFIG>"
	LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}/$<CONFIG>"
)

install_package(
	NAME "ez-geo"
	EXPORT "ez-geo-targets"
	VERSION "${EZ_GEO_VERSION}"
	COMPATIBILITY "SameMajorVersion"
	DESTINATION "${CONFIG_DIR}"
	PRECONFIG "${CMAKE_CURRENT_SOURCE_DIR}/preconfig.cmake"
)