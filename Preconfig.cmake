
if(NOT TARGET glm)
	find_dependency(glm CONFIG)
endif()

if(NOT TARGET ez::math)
	find_dependency(ez-math CONFIG)
endif()