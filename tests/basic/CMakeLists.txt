find_package(Catch2 CONFIG REQUIRED)

add_executable(basic_test "main.cpp")
target_link_libraries(basic_test PRIVATE fmt::fmt ez::geo Catch2::Catch2)
