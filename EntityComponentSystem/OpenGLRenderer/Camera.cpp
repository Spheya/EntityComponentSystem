#include "Camera.hpp"
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
