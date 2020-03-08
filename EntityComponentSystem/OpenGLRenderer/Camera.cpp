#include "Camera.hpp"

#include <MathConstants.hpp>
#include <algorithm>

renderer::Camera::Camera(float near, float far, float fov, Transform transform) :
	_near(near),
	_far(far),
	_fov(fov),
	_transform(transform)
{}

void renderer::Camera::setTransform(const Transform& transform) {
	_transform = transform;
}

Transform& renderer::Camera::getTransform() {
	return _transform;
}

const Transform& renderer::Camera::getTransform() const {
	return _transform;
}

const glm::mat4& renderer::Camera::getViewMatrix() const {
	return _transform.getInverseMatrix();
}

glm::mat4 renderer::Camera::getProjectionMatrix(Window& window) const {
	const float aspect = float(window.getWindowWidth()) / float(window.getWindowHeight());
	const float angle = tan(_fov);

	return glm::mat4(
		1.0f/(aspect * angle), 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f/angle, 0.0f, 0.0f,
		0.0f, 0.0f, -(-_near - _far) / (_near - _far), -1.0f,
		0.0f, 0.0f, (2.0f * _near * _far) / (_near - _far), 0.0f
	);
}

float renderer::Camera::getNear() const {
	return _near;
}

float renderer::Camera::getFar() const {
	return _far;
}

float renderer::Camera::getFov() const {
	return _fov;
}

void renderer::Camera::setNear(float near) {
	_near = near;
}

void renderer::Camera::setFar(float far) {
	_far = far;
}

void renderer::Camera::setFov(float fov) {
	_fov = fov;
}

void renderer::Camera::processDebugMovement(const Input& input, float deltatime) {
	// Gather inputs
	const bool forward =  input.isKeyDown(GLFW_KEY_W) || input.isKeyDown(GLFW_KEY_UP);
	const bool backward = input.isKeyDown(GLFW_KEY_S) || input.isKeyDown(GLFW_KEY_DOWN);
	const bool left =     input.isKeyDown(GLFW_KEY_A) || input.isKeyDown(GLFW_KEY_LEFT);
	const bool right =    input.isKeyDown(GLFW_KEY_D) || input.isKeyDown(GLFW_KEY_RIGHT);

	const bool up =		  input.isKeyDown(GLFW_KEY_SPACE);
	const bool down =	  input.isKeyDown(GLFW_KEY_LEFT_SHIFT) || input.isKeyDown(GLFW_KEY_RIGHT_SHIFT);

	// Calculate camera rotations
	_transform.rotate(
		glm::vec3(
			input.getMouseDelta().y * -1.0f,
			input.getMouseDelta().x * -1.0f,
			0.0f
		)
	);

	// Clamp the rotation
	_transform.setRotation(glm::vec3(
		std::clamp(_transform.getRotation().x, -MathConstants::HALF_PI, MathConstants::HALF_PI),
		_transform.getRotation().y,
		_transform.getRotation().z
	));

	// Calculate movement over each axis
	glm::vec3 deltaPos(
		float(right - left),
		float(up - down),
		float(backward - forward)
	);
	deltaPos *= 6.0f;

	// Slow the camera down when moving over 2 axes
	if (forward - backward && right - left)
		deltaPos *= 0.8f;

	// Apply rotation to the camera movement
	deltaPos = glm::vec3(
		cos(-_transform.getRotation().y) * deltaPos.x + sin(_transform.getRotation().y) * deltaPos.z,
		deltaPos.y,
		cos(_transform.getRotation().y) * deltaPos.z + sin(-_transform.getRotation().y) * deltaPos.x
	);

	_transform.move(deltaPos * deltatime);
}
