
find_package(Catch2 CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

add_executable(color_test "main.cpp")
target_link_libraries(color_test PRIVATE 
	fmt::fmt 
	Catch2::Catch2
	ez::geo
)