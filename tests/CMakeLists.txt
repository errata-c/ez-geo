
find_package(fmt CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(glew CONFIG REQUIRED)
find_package(nanovg CONFIG REQUIRED)

find_package(ez-window CONFIG REQUIRED)

if(NOT TARGET "ez-imgui")
	find_package(ez-imgui CONFIG REQUIRED)
endif()

add_library(test_framework INTERFACE)
target_link_libraries(test_framework INTERFACE fmt::fmt ez::geo ez::window ez::imgui ez::imgui-opengl3 GLEW::GLEW nanovg::nanovg)
target_compile_features(test_framework INTERFACE cxx_std_17)

add_subdirectory("basic")
add_subdirectory("ex")

#add_executable(bezier_multi_test "bezier/multi/multi.cpp")
#target_include_directories(bezier_multi_test PRIVATE "bezier/multi")
#target_link_libraries(bezier_multi_test PRIVATE test_framework)

#add_executable(qpath_test "bezier/qpath.cpp")
#target_include_directories(qpath_test PRIVATE "bezier")
#target_link_libraries(qpath_test PRIVATE test_framework)


#add_executable(bezier_fitting_test "bezier/fitting/fitting.cpp")
#target_include_directories(bezier_fitting_test PRIVATE "bezier/fitting")
#target_link_libraries(bezier_fitting_test PRIVATE test_framework)

#add_executable(bezier_offset_test "bezier/offset/offset.cpp")
#target_include_directories(bezier_offset_test PRIVATE "bezier/offset")
#target_link_libraries(bezier_offset_test PRIVATE test_framework)

#add_executable(bezier_curvature_test "bezier/offset/curvature.cpp")
#target_include_directories(bezier_curvature_test PRIVATE "bezier/offset")
#target_link_libraries(bezier_curvature_test PRIVATE test_framework)