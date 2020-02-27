#include "Input.h"
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
	m_parent(window)
{
	if (window != nullptr) {
		glfwSetKeyCallback(window, renderer::keyCallback);
		glfwSetMouseButtonCallback(window, mouseButtonCallback);

		double x, y;
		glfwGetCursorPos(m_parent, &x, &y);
		m_mouseData = MouseData{ glm::vec2(float(x), float(y)), glm::vec2(0.0f) };

	}
}

void renderer::Input::setParent(GLFWwindow* window) {
	glfwSetKeyCallback(window, keyCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);

	m_parent = window;

	if (m_isMouseLocked)
		enableLockedMouse();

	double x, y;
	glfwGetCursorPos(m_parent, &x, &y);
	m_mouseData = MouseData{ glm::vec2(float(x), float(y)), glm::vec2(0.0f) };
}

void renderer::Input::enableLockedMouse() {
	glfwSetInputMode(m_parent, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	m_isMouseLocked = true;

	double x, y;
	glfwGetCursorPos(m_parent, &x, &y);
	m_mouseData = MouseData{ glm::vec2(float(x), float(y)), glm::vec2(0.0f) };

}

void renderer::Input::disableLockedMouse() {
	glfwSetInputMode(m_parent, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	m_isMouseLocked = false;

	double x, y;
	glfwGetCursorPos(m_parent, &x, &y);
	m_mouseData = MouseData{ glm::vec2(float(x), float(y)), glm::vec2(0.0f) };

}

glm::vec2 renderer::Input::getMouseDelta() const {
	return m_mouseData.delta;
}

glm::vec2 renderer::Input::getMousePosition() const {
	return m_mouseData.position;
}

bool renderer::Input::isMouseButtonPressed(unsigned int button) const {
	const auto it = buffer[m_parent].mouseButtons.find(button);
	const auto it2 = m_lastframeMouseButtons.find(button);
	return (it != buffer[m_parent].mouseButtons.end() && it->second) && !(it2 != m_lastframeMouseButtons.end() && it2->second);
}

bool renderer::Input::isMouseButtonDown(unsigned int button) const {
	const auto it = buffer[m_parent].mouseButtons.find(button);
	return it != buffer[m_parent].mouseButtons.end() && it->second;
}

bool renderer::Input::isMouseButtonReleased(unsigned int button) const {
	const auto it = buffer[m_parent].mouseButtons.find(button);
	const auto it2 = m_lastframeMouseButtons.find(button);
	return !(it != buffer[m_parent].mouseButtons.end() && it->second) && (it2 != m_lastframeMouseButtons.end() && it2->second);
}

bool renderer::Input::isKeyPressed(unsigned int key) const {
	const auto it = buffer[m_parent].keyboardKeys.find(key);
	const auto it2 = m_lastframeKeys.find(key);
	return (it != buffer[m_parent].keyboardKeys.end() && it->second) && !(it2 != m_lastframeKeys.end() && it2->second);
}

bool renderer::Input::isKeyDown(unsigned int key) const {
	const auto it = buffer[m_parent].keyboardKeys.find(key);
	return it != buffer[m_parent].keyboardKeys.end() && it->second;
}

bool renderer::Input::isKeyReleased(unsigned int key) const {
	const auto it = buffer[m_parent].keyboardKeys.find(key);
	const auto it2 = m_lastframeKeys.find(key);
	return !(it != buffer[m_parent].keyboardKeys.end() && it->second) && (it2 != m_lastframeKeys.end() && it2->second);
}

void renderer::Input::update(float deltatime) {
	m_lastframeKeys = buffer[m_parent].keyboardKeys;
	m_lastframeMouseButtons = buffer[m_parent].mouseButtons;

	double x, y;
	glfwGetCursorPos(m_parent, &x, &y);
	m_mouseData = MouseData{ glm::vec2(float(x), float(y)), (glm::vec2(float(x), float(y)) - m_mouseData.position) * deltatime};
}
