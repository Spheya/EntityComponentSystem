#pragma once
#include "Transform.h"
#include "Window.h"

namespace renderer {
	class Camera
	{
	public:
		Camera(float near, float far, float fov, Transform transform = Transform());

		void setTransform(const Transform& transform);
		Transform& getTransform();
		[[nodiscard]] const Transform& getTransform() const;

		[[nodiscard]] const glm::mat4& getViewMatrix() const;
		glm::mat4 getProjectionMatrix(Window& window) const;

		[[nodiscard]] float getNear() const;
		[[nodiscard]] float getFar() const;
		[[nodiscard]] float getFov() const;

		void setNear(float near);
		void setFar(float far);
		void setFov(float fov);

	private:
		Transform m_transform;

		float m_near, m_far, m_fov;

	};
}
