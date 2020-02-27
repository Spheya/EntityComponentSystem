#include "Camera.h"
renderer::Camera::Camera(float near, float far, float fov, Transform transform) :
	m_near(near),
	m_far(far),
	m_fov(fov),
	m_transform(transform)
{}

void renderer::Camera::setTransform(const Transform& transform) {
	m_transform = transform;
}

Transform& renderer::Camera::getTransform() {
	return m_transform;
}

const Transform& renderer::Camera::getTransform() const {
	return m_transform;
}

const glm::mat4& renderer::Camera::getViewMatrix() const {
	return m_transform.getInverseMatrix();
}

glm::mat4 renderer::Camera::getProjectionMatrix(Window& window) const {
	const float aspect = float(window.getWindowWidth()) / float(window.getWindowHeight());
	const float angle = tan(m_fov);

	return glm::mat4(
		1.0f/(aspect * angle), 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f/angle, 0.0f, 0.0f,
		0.0f, 0.0f, (-m_near - m_far) / (m_near - m_far), 1.0f,
		0.0f, 0.0f, (2.0f * m_near * m_far) / (m_near - m_far), 0.0f
	);
}

float renderer::Camera::getNear() const {
	return m_near;
}

float renderer::Camera::getFar() const {
	return m_far;
}

float renderer::Camera::getFov() const {
	return m_fov;
}

void renderer::Camera::setNear(float near) {
	m_near = near;
}

void renderer::Camera::setFar(float far) {
	m_far = far;
}

void renderer::Camera::setFov(float fov) {
	m_fov = fov;
}
