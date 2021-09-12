find_package(rt-loader CONFIG REQUIRED COMPONENTS glew)

find_package(fmt CONFIG REQUIRED)
find_package(ez-imgui CONFIG REQUIRED)
find_package(nanovg CONFIG REQUIRED)
find_package(ez-window CONFIG REQUIRED)

add_library(test_framework INTERFACE)
target_link_libraries(test_framework INTERFACE 
	fmt::fmt 
	ez::geo 
	ez::window 
	ez::imgui 
	ez::imgui-opengl3 
	rt::loader-glew 
	nanovg::nanovg
)
target_compile_features(test_framework INTERFACE cxx_std_17)