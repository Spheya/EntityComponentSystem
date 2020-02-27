#include "Transform.hpp"

Transform::Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) :
	_position(position),
	_rotation(rotation),
	_scale(scale) {
	build();
}

void Transform::move(const glm::vec3& delta) {
	_position += delta;
	build();
}

void Transform::setPosition(const glm::vec3& position) {
	_position = position;
	build();
}

const glm::vec3& Transform::getPosition() const {
	return _position;
}

void Transform::rotate(const glm::vec3& delta) {
	_rotation += delta;
	build();
}

void Transform::setRotation(const glm::vec3& rotation) {
	_rotation = rotation;
	build();
}

const glm::vec3& Transform::getRotation() const {
	return _rotation;
}

void Transform::setScale(const glm::vec3& scale) {
	_scale = scale;
	build();
}

const glm::vec3& Transform::getScale() const {
	return _scale;
}

const glm::mat4& Transform::getMatrix() const {
	return _transformation;
}

const glm::mat4& Transform::getInverseMatrix() const {
	return _inverseTransformation;
}

const glm::mat4& Transform::getNormalMatrix() const {
	return _normalTransformation;
}

void Transform::build() {

	const float cosX = cos(_rotation.x);
	const float sinX = sin(_rotation.x);
	const float cosY = cos(_rotation.y);
	const float sinY = sin(_rotation.y);
	const float cosZ = cos(_rotation.z);
	const float sinZ = sin(_rotation.z);

	const glm::mat4 scalePos = glm::mat4(
		_scale.x, 0.0f, 0.0f, 0.0f,
		0.0f, _scale.y, 0.0f, 0.0f,
		0.0f, 0.0f, _scale.z, 0.0f,
		_position.x, _position.y, _position.z, 1.0f
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

	_transformation = scalePos * rotY * rotX * rotZ;
	_inverseTransformation = glm::inverse(_transformation);
}