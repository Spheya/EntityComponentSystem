#include "Input.hpp"
#include <functional>

void renderer::keyCallback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/) {
	if (action == GLFW_PRESS)
		Input::buffer[window].keyboardKeys[key] = true;

	if (action == GLFW_RELEASE)
		Input::buffer[window].keyboardKeys[key] = false;
}

void renderer::mouseButtonCallback(GLFWwindow* window, int button, int action, int /*mods*/) {
	if (action == GLFW_PRESS)
		Input::buffer[window].mouseButtons[button] = true;

	if (action == GLFW_RELEASE)
		Input::buffer[window].mouseButtons[button] = false;
}

std::unordered_map<GLFWwindow*, renderer::Input::BufferData> renderer::Input::buffer;

renderer::Input::Input(GLFWwindow* window) :
	_parent(window)
{
	if (window != nullptr) {
		glfwSetKeyCallback(window, renderer::keyCallback);
		glfwSetMouseButtonCallback(window, mouseButtonCallback);

		double x, y;
		glfwGetCursorPos(_parent, &x, &y);
		_mouseData = MouseData{ glm::vec2(float(x), float(y)), glm::vec2(0.0f) };

	}
}

void renderer::Input::setParent(GLFWwindow* window) {
	glfwSetKeyCallback(window, keyCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);

	_parent = window;

	if (_isMouseLocked)
		enableLockedMouse();

	double x, y;
	glfwGetCursorPos(_parent, &x, &y);
	_mouseData = MouseData{ glm::vec2(float(x), float(y)), glm::vec2(0.0f) };
}

void renderer::Input::enableLockedMouse() {
	glfwSetInputMode(_parent, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	_isMouseLocked = true;

	double x, y;
	glfwGetCursorPos(_parent, &x, &y);
	_mouseData = MouseData{ glm::vec2(float(x), float(y)), glm::vec2(0.0f) };

}

void renderer::Input::disableLockedMouse() {
	glfwSetInputMode(_parent, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	_isMouseLocked = false;

	double x, y;
	glfwGetCursorPos(_parent, &x, &y);
	_mouseData = MouseData{ glm::vec2(float(x), float(y)), glm::vec2(0.0f) };

}

glm::vec2 renderer::Input::getMouseDelta() const {
	return _mouseData.delta;
}

glm::vec2 renderer::Input::getMousePosition() const {
	return _mouseData.position;
}

bool renderer::Input::isMouseButtonPressed(unsigned int button) const {
	const auto it = buffer[_parent].mouseButtons.find(button);
	const auto it2 = _lastframeMouseButtons.find(button);
	return (it != buffer[_parent].mouseButtons.end() && it->second) && !(it2 != _lastframeMouseButtons.end() && it2->second);
}

bool renderer::Input::isMouseButtonDown(unsigned int button) const {
	const auto it = buffer[_parent].mouseButtons.find(button);
	return it != buffer[_parent].mouseButtons.end() && it->second;
}

bool renderer::Input::isMouseButtonReleased(unsigned int button) const {
	const auto it = buffer[_parent].mouseButtons.find(button);
	const auto it2 = _lastframeMouseButtons.find(button);
	return !(it != buffer[_parent].mouseButtons.end() && it->second) && (it2 != _lastframeMouseButtons.end() && it2->second);
}

bool renderer::Input::isKeyPressed(unsigned int key) const {
	const auto it = buffer[_parent].keyboardKeys.find(key);
	const auto it2 = _lastframeKeys.find(key);
	return (it != buffer[_parent].keyboardKeys.end() && it->second) && !(it2 != _lastframeKeys.end() && it2->second);
}

bool renderer::Input::isKeyDown(unsigned int key) const {
	const auto it = buffer[_parent].keyboardKeys.find(key);
	return it != buffer[_parent].keyboardKeys.end() && it->second;
}

bool renderer::Input::isKeyReleased(unsigned int key) const {
	const auto it = buffer[_parent].keyboardKeys.find(key);
	const auto it2 = _lastframeKeys.find(key);
	return !(it != buffer[_parent].keyboardKeys.end() && it->second) && (it2 != _lastframeKeys.end() && it2->second);
}

void renderer::Input::update(float deltatime) {
	_lastframeKeys = buffer[_parent].keyboardKeys;
	_lastframeMouseButtons = buffer[_parent].mouseButtons;

	double x, y;
	glfwGetCursorPos(_parent, &x, &y);
	_mouseData = MouseData{ glm::vec2(float(x), float(y)), (glm::vec2(float(x), float(y)) - _mouseData.position) * deltatime};
}
