
find_package(fmt CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(glew CONFIG REQUIRED)
find_package(nanovg CONFIG REQUIRED)

add_library(test_framework STATIC 
	"interface/Core.cpp" 
	"interface/imgui_impl_glfw.cpp" 
	"interface/imgui_impl_opengl3.cpp")

target_link_libraries(test_framework PUBLIC fmt::fmt ez-geo glfw GLEW::GLEW nanovg::nanovg)
target_compile_features(test_framework PUBLIC cxx_std_17)
target_include_directories(test_framework PUBLIC "interface")
target_compile_definitions(test_framework PRIVATE "IMGUI_IMPL_OPENGL_LOADER_GLEW")

add_subdirectory("basic")

add_executable(bezier_multi_test "bezier/multi/multi.cpp")
target_include_directories(bezier_multi_test PRIVATE "bezier/multi")
target_link_libraries(bezier_multi_test PRIVATE test_framework)

add_executable(qpath_test "bezier/qpath.cpp")
target_include_directories(qpath_test PRIVATE "bezier")
target_link_libraries(qpath_test PRIVATE test_framework)


#add_executable(bezier_fitting_test "bezier/fitting/fitting.cpp")
#target_include_directories(bezier_fitting_test PRIVATE "bezier/fitting")
#target_link_libraries(bezier_fitting_test PRIVATE test_framework)

add_executable(bezier_offset_test "bezier/offset/offset.cpp")
target_include_directories(bezier_offset_test PRIVATE "bezier/offset")
target_link_libraries(bezier_offset_test PRIVATE test_framework)


add_executable(bezier_curvature_test "bezier/offset/curvature.cpp")
target_include_directories(bezier_curvature_test PRIVATE "bezier/offset")
target_link_libraries(bezier_curvature_test PRIVATE test_framework)