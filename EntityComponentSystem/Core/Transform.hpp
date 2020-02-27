#pragma once

#include "glm/glm.hpp"

class Transform {
public:
	explicit Transform(const glm::vec3& position = glm::vec3(0.0f), const glm::vec3& rotation = glm::vec3(0.0f), const glm::vec3& scale = glm::vec3(1.0f));

	void move(const glm::vec3& delta);
	void setPosition(const glm::vec3& position);
	[[nodiscard]] const glm::vec3& getPosition() const;

	void rotate(const glm::vec3& delta);
	void setRotation(const glm::vec3& rotation);
	[[nodiscard]] const glm::vec3& getRotation() const;

	void setScale(const glm::vec3& scale);
	[[nodiscard]] const glm::vec3& getScale() const;

	[[nodiscard]] const glm::mat4& getMatrix() const;
	[[nodiscard]] const glm::mat4& getInverseMatrix() const;
	[[nodiscard]] const glm::mat4& getNormalMatrix() const;

private:
	void build();

	glm::vec3 _position = glm::vec3(0.0f);
	glm::vec3 _rotation = glm::vec3(0.0f);
	glm::vec3 _scale = glm::vec3(1.0f);

	glm::mat4 _transformation = glm::mat4();
	glm::mat4 _inverseTransformation = glm::mat4();
	glm::mat4 _normalTransformation = glm::mat4();
};

