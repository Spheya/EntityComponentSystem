#include "Window.h"

#include <iostream>
#include <GL/glew.h>

renderer::Window::Window(const char* title, Mode windowMode, GLFWmonitor* monitor) :
	m_title(title),
	m_input(std::make_shared<Input>(nullptr))
{
	setMode(windowMode, monitor);
}

void renderer::Window::destroy() {
	glfwDestroyWindow(m_window);
}

bool renderer::Window::isCloseRequested() const {
	return glfwWindowShouldClose(m_window);
}

void renderer::Window::update() {
	glfwSwapBuffers(m_window);
	glfwPollEvents();

	// Calculate the deltatime
	const float time = float(glfwGetTime());
	m_deltaTime = time - m_time;
	m_time = time;

	// Prepare input for the next frame
	m_input->update(m_deltaTime);
}

void renderer::Window::clear(bool colour, bool depth, bool stencil) {
	glClear((colour ? GL_COLOR_BUFFER_BIT : 0x0) | (depth ? GL_DEPTH_BUFFER_BIT : 0x0) | (stencil ? GL_STENCIL_BUFFER_BIT : 0x0));
}

void renderer::Window::makeCurrentContext() {
	glfwMakeContextCurrent(m_window);
	glfwSwapInterval(1);

	if(!m_glewSetup) {
		const GLenum glewError = glewInit();
		if (glewError != GLEW_OK) {
			std::cout << "Something went wrong when setting up GLEW! Error code - " << glewError << std::endl;
			std::terminate();
		}
		m_glewSetup = true;
	}
}

void renderer::Window::setTitle(const char* title) {
	m_title = title;
	glfwSetWindowTitle(m_window, title);
}

int renderer::Window::getWindowWidth() const {
	return m_width;
}

int renderer::Window::getWindowHeight() const {
	return m_height;
}

glm::vec2 renderer::Window::getWindowSize() const {
	return glm::vec2(m_width, m_height);
}

float renderer::Window::getDeltaTime() const {
	return m_deltaTime;
}

float renderer::Window::getTime() const {
	return m_time;
}

void renderer::Window::setMode(Mode windowMode, GLFWmonitor* monitor) {
	if (monitor == nullptr)
		monitor = glfwGetPrimaryMonitor();

	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	if (windowMode == Mode::WINDOWED) {
		m_width = 640;
		m_height = 480;
	}
	else {
		m_width = mode->width;
		m_height = mode->height;
	}

	if (m_window)
		glfwDestroyWindow(m_window);

	switch (windowMode) {
	case Mode::WINDOWED:
		setWindow(createWindowedWindow(m_title, m_width, m_height, mode->redBits, mode->greenBits, mode->blueBits, mode->refreshRate));
		break;
	case Mode::BORDERLESS:
		setWindow(createBorderlessWindow(m_title, m_width, m_height, mode->redBits, mode->greenBits, mode->blueBits, mode->refreshRate));
		break;
	case Mode::FULLSCREEN:
		setWindow(createFullscreenWindow(m_title, m_width, m_height, mode->redBits, mode->greenBits, mode->blueBits, mode->refreshRate, monitor));
		break;
	}

	// Center the window
	int originX, originY;
	glfwGetMonitorPos(monitor, &originX, &originY);
	glfwSetWindowPos(m_window, originX + (mode->width - m_width) / 2, originY + (mode->height - m_height) / 2);
}

glm::mat3 renderer::Window::getOrthographicProjectionMatrix() const {
	return glm::mat3(
		2.0f / m_width, 0.0f, 0.0f,
		0.0f, 2.0f / m_height, 0.0f,
		0.0f, 0.0f, 0.999f
	);
}

const renderer::Input* renderer::Window::getInput() const {
	return m_input.get();
}

const std::shared_ptr<renderer::Input>& renderer::Window::getInput() {
	return m_input;
}

void renderer::Window::setWindow(GLFWwindow* window) {
	m_window = window;
	m_input->setParent(window);

	if (!m_window) {
		// Something really went wrong when setting up the window
		glfwTerminate();
		std::terminate();
	}
}

GLFWwindow* renderer::Window::createBorderlessWindow(const char* title, int width, int height, int redBits, int greenBits, int blueBits, int refreshRate) {
	glfwWindowHint(GLFW_RED_BITS, redBits);
	glfwWindowHint(GLFW_GREEN_BITS, greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, refreshRate);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_DECORATED, GL_FALSE);

	return glfwCreateWindow(width, height, title, nullptr, nullptr);
}

GLFWwindow* renderer::Window::createWindowedWindow(const char* title, int width, int height, int redBits, int greenBits, int blueBits, int refreshRate) {
	glfwWindowHint(GLFW_RED_BITS, redBits);
	glfwWindowHint(GLFW_GREEN_BITS, greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, refreshRate);

	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwWindowHint(GLFW_DECORATED, GL_TRUE);

	return glfwCreateWindow(width, height, title, nullptr, nullptr);
}

GLFWwindow* renderer::Window::createFullscreenWindow(const char* title, int width, int height, int redBits, int greenBits, int blueBits, int refreshRate, GLFWmonitor* monitor) {
	glfwWindowHint(GLFW_RED_BITS, redBits);
	glfwWindowHint(GLFW_GREEN_BITS, greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, refreshRate);

	return glfwCreateWindow(width, height, title, monitor, nullptr);
}