#pragma once
#include <memory>
#include <unordered_map>
#include <GLFW/glfw3.h>
#include <atomic>
#include <glm/glm.hpp>

//TODO: Add gamepad support
//TODO: Check if any key is pressed
//TODO: Check if any mouse button is pressed
//TODO: Add timer since last mouse movement
//TODO: Add timer since last key press
//TODO: Write custom enum for the keys instead of using the GLFW one
//TODO: Custom cursors

namespace renderer {
	class Input
	{
		friend class Window;
		friend void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		friend void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	public:
		explicit Input(GLFWwindow* window);
		Input(Input&) = delete;
		Input& operator=(Input&) = delete;
		Input(Input&&) = default;
		Input& operator=(Input&&) = default;
		~Input() = default;

		void setParent(GLFWwindow* window);

		void enableLockedMouse();
		void disableLockedMouse();

		[[nodiscard]] glm::vec2 getMouseDelta() const;
		[[nodiscard]] glm::vec2 getMousePosition() const;

		[[nodiscard]] bool isMouseButtonPressed(unsigned int button) const;
		[[nodiscard]] bool isMouseButtonDown(unsigned int button) const;
		[[nodiscard]] bool isMouseButtonReleased(unsigned int button) const;

		[[nodiscard]] bool isKeyPressed(unsigned int key) const;
		[[nodiscard]] bool isKeyDown(unsigned int key) const;
		[[nodiscard]] bool isKeyReleased(unsigned int key) const;

	private:
		struct MouseData {
			glm::vec2 position;
			glm::vec2 delta;
		};

		struct BufferData {
			std::unordered_map<unsigned int, bool> keyboardKeys;
			std::unordered_map<unsigned int, bool> mouseButtons;
		};

		static std::unordered_map<GLFWwindow*, BufferData> buffer;

		std::unordered_map<unsigned int, bool> m_lastframeKeys;

		std::unordered_map<unsigned int, bool> m_lastframeMouseButtons;
		MouseData m_mouseData;
		bool m_isMouseLocked = false;

		GLFWwindow* m_parent;

		void update(float deltatime);
	};
}