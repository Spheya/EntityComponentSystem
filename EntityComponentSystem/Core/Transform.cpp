#include "Transform.h"

Transform::Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) :
	m_position(position),
	m_rotation(rotation),
	m_scale(scale) {
	build();
}

void Transform::move(const glm::vec3& delta) {
	m_position += delta;
	build();
}

void Transform::setPosition(const glm::vec3& position) {
	m_position = position;
	build();
}

const glm::vec3& Transform::getPosition() const {
	return m_position;
}

void Transform::rotate(const glm::vec3& delta) {
	m_rotation += delta;
	build();
}

void Transform::setRotation(const glm::vec3& rotation) {
	m_rotation = rotation;
	build();
}

const glm::vec3& Transform::getRotation() const {
	return m_rotation;
}

void Transform::setScale(const glm::vec3& scale) {
	m_scale = scale;
	build();
}

const glm::vec3& Transform::getScale() const {
	return m_scale;
}

const glm::mat4& Transform::getMatrix() const {
	return m_transformation;
}

const glm::mat4& Transform::getInverseMatrix() const {
	return m_inverseTransformation;
}

const glm::mat4& Transform::getNormalMatrix() const {
	return m_normalTransformation;
}

void Transform::build() {

	const float cosX = cos(m_rotation.x);
	const float sinX = sin(m_rotation.x);
	const float cosY = cos(m_rotation.y);
	const float sinY = sin(m_rotation.y);
	const float cosZ = cos(m_rotation.z);
	const float sinZ = sin(m_rotation.z);

	const glm::mat4 scalePos = glm::mat4(
		m_scale.x, 0.0f, 0.0f, 0.0f,
		0.0f, m_scale.y, 0.0f, 0.0f,
		0.0f, 0.0f, m_scale.z, 0.0f,
		m_position.x, m_position.y, m_position.z, 1.0f
	);
	const glm::mat4 rotX = glm::mat4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cosX, sinX, 0.0f,
		0.0f, -sinX, cosX, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	const glm::mat4 rotY = glm::mat4(
		cosY, 0.0f, -sinY, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		sinY, 0.0f, cosY, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	const glm::mat4 rotZ = glm::mat4(
		cosZ, sinZ, 0.0f, 0.0f,
		-sinZ, cosZ, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);

	m_transformation = scalePos * rotY * rotX * rotZ;
	m_inverseTransformation = glm::inverse(m_transformation);
}