#include "Window.hpp"

#include <iostream>
#include <GL/glew.h>

renderer::Window::Window(const char* title, Mode windowMode, GLFWmonitor* monitor) :
	_title(title),
	_input(std::make_shared<Input>(nullptr))
{
	setMode(windowMode, monitor);
}

void renderer::Window::destroy() {
	glfwDestroyWindow(_window);
}

bool renderer::Window::isCloseRequested() const {
	return glfwWindowShouldClose(_window);
}

void renderer::Window::update() {
	glfwSwapBuffers(_window);
	glfwPollEvents();

	// Calculate the deltatime
	const float time = float(glfwGetTime());
	if(_time != 0.0f)
		_deltaTime = time - _time;
	_time = time;

	// Prepare input for the next frame
	_input->update(_deltaTime);
}

void renderer::Window::clear(bool colour, bool depth, bool stencil) {
	glClear((colour ? GL_COLOR_BUFFER_BIT : 0x0) | (depth ? GL_DEPTH_BUFFER_BIT : 0x0) | (stencil ? GL_STENCIL_BUFFER_BIT : 0x0));
}

void renderer::Window::enableVsync(bool enabled) {
	glfwSwapInterval(enabled ? 1 : 0);
}

void renderer::Window::makeCurrentContext() {
	glfwMakeContextCurrent(_window);
	glfwSwapInterval(1);

	if(!_glewSetup) {
		const GLenum glewError = glewInit();
		if (glewError != GLEW_OK) {
			std::cout << "Something went wrong when setting up GLEW! Error code - " << glewError << std::endl;
			std::terminate();
		}
		_glewSetup = true;
	}
}

void renderer::Window::detachCurrentContext() {
	glfwMakeContextCurrent(nullptr);
}

void renderer::Window::setTitle(const char* title) {
	_title = title;
	glfwSetWindowTitle(_window, title);
}

int renderer::Window::getWindowWidth() const {
	return _width;
}

int renderer::Window::getWindowHeight() const {
	return _height;
}

glm::vec2 renderer::Window::getWindowSize() const {
	return glm::vec2(_width, _height);
}

float renderer::Window::getDeltaTime() const {
	return _deltaTime;
}

float renderer::Window::getTime() const {
	return _time;
}

void renderer::Window::setMode(Mode windowMode, GLFWmonitor* monitor) {
	if (monitor == nullptr)
		monitor = glfwGetPrimaryMonitor();

	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	if (windowMode == Mode::WINDOWED) {
		_width = 640;
		_height = 480;
	}
	else {
		_width = mode->width;
		_height = mode->height;
	}

	if (_window)
		glfwDestroyWindow(_window);

	switch (windowMode) {
	case Mode::WINDOWED:
		setWindow(createWindowedWindow(_title, _width, _height, mode->redBits, mode->greenBits, mode->blueBits, mode->refreshRate));
		break;
	case Mode::BORDERLESS:
		setWindow(createBorderlessWindow(_title, _width, _height, mode->redBits, mode->greenBits, mode->blueBits, mode->refreshRate));
		break;
	case Mode::FULLSCREEN:
		setWindow(createFullscreenWindow(_title, _width, _height, mode->redBits, mode->greenBits, mode->blueBits, mode->refreshRate, monitor));
		break;
	}

	// Center the window
	int originX, originY;
	glfwGetMonitorPos(monitor, &originX, &originY);
	glfwSetWindowPos(_window, originX + (mode->width - _width) / 2, originY + (mode->height - _height) / 2);
}

glm::mat3 renderer::Window::getOrthographicProjectionMatrix() const {
	return glm::mat3(
		2.0f / _width, 0.0f, 0.0f,
		0.0f, 2.0f / _height, 0.0f,
		0.0f, 0.0f, 0.999f
	);
}

const renderer::Input* renderer::Window::getInput() const {
	return _input.get();
}

const std::shared_ptr<renderer::Input>& renderer::Window::getInput() {
	return _input;
}

void renderer::Window::setWindow(GLFWwindow* window) {
	_window = window;
	_input->setParent(window);

	if (!_window) {
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