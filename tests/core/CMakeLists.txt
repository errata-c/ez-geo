
find_package(Catch2 CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

add_executable(core_tests 
	"main.cpp"
	"AABB.cpp"
	"transform.cpp"
	"all_compile.cpp"
)
target_link_libraries(core_tests PRIVATE 
	fmt::fmt
	ez::geo
	Catch2::Catch2
)
