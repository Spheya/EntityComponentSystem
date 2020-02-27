#include "GlfwGuard.hpp"

#include <GLFW/glfw3.h>
#include <exception>

renderer::GlfwGuard::GlfwGuard() {
	if (!glfwInit())
		std::terminate();
}

renderer::GlfwGuard::~GlfwGuard() {
	glfwTerminate();
}
