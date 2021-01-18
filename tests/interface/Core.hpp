#pragma once
#include "imgui.h"
#include <stdio.h>
#include "imgui_impl_glfw.hpp"
#include "imgui_impl_opengl3.hpp"
#include <GLFW/glfw3.h>

#define NANOVG_GL3_IMPLEMENTATION
#include <nanovg_gl.h>

#include <glm/vec2.hpp>
#include <string_view>

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

class Core {
public:
	Core(std::string_view title, glm::ivec2 size);

	~Core();

	bool beginFrame();
	void endFrame();

	NVGcontext* vg;
private:
	GLFWwindow* window;
};
