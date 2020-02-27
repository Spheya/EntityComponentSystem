#pragma once

#include "glm/glm.hpp"

class Transform {
public:
	explicit Transform(const glm::vec3& position = glm::vec3(0.0f), const glm::vec3& rotation = glm::vec3(0.0f), const glm::vec3& scale = glm::vec3(1.0f));

	void move(const glm::vec3& delta);
	void setPosition(const glm::vec3& position);
	[[nodiscard]] const glm::vec3& getPosition() const;
	//glm::vec3& getPosition();

	void rotate(const glm::vec3& delta);
	void setRotation(const glm::vec3& rotation);
	[[nodiscard]] const glm::vec3& getRotation() const;
	//glm::vec3& getRotation();

	void setScale(const glm::vec3& scale);
	[[nodiscard]] const glm::vec3& getScale() const;
	//glm::vec3& getScale();

	[[nodiscard]] const glm::mat4& getMatrix() const;
	[[nodiscard]] const glm::mat4& getInverseMatrix() const;
	[[nodiscard]] const glm::mat4& getNormalMatrix() const;

	/*
	glm::vec3 toObjectSpace(const glm::vec3& position) const;
	Ray toObjectSpace(const Ray& ray) const;
	void toWorldSpace(glm::vec3& position, glm::vec3& normal) const;
	Ray toWorldSpace(Ray ray) const;
	glm::vec3 toWorldSpace(const glm::vec3& position) const;
	*/

	//void bake();

private:
	void build();

	//mutable bool m_dirty = false;

	glm::vec3 m_position = glm::vec3(0.0f);
	glm::vec3 m_rotation = glm::vec3(0.0f);
	glm::vec3 m_scale = glm::vec3(1.0f);

	glm::mat4 m_transformation = glm::mat4();
	glm::mat4 m_inverseTransformation = glm::mat4();
	glm::mat4 m_normalTransformation = glm::mat4();
};

