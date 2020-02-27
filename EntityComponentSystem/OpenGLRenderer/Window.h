#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Input.h"

//TODO: Allow for multiple windows

namespace renderer {
	class Window {
	public:
		enum class Mode {
			WINDOWED,
			BORDERLESS,
			FULLSCREEN
		};

		Window(const char* title, Mode windowMode = Mode::WINDOWED, GLFWmonitor* monitor = nullptr);
		~Window() = default;

		Window(Window&&) = default;
		Window& operator=(Window&&) = default;

		Window(Window&) = delete;
		Window& operator=(Window&) = delete;

		void destroy();
		[[nodiscard]] bool isCloseRequested() const;
		void update();
		static void clear(bool colour, bool depth, bool stencil);

		void makeCurrentContext();

		void setTitle(const char* title);

		[[nodiscard]] int getWindowWidth() const;
		[[nodiscard]] int getWindowHeight() const;
		[[nodiscard]] glm::vec2 getWindowSize() const;

		[[nodiscard]] float getDeltaTime() const;
		[[nodiscard]] float getTime() const;

		void setMode(Mode windowMode, GLFWmonitor* monitor = nullptr);

		[[nodiscard]] glm::mat3 getOrthographicProjectionMatrix() const;

		[[nodiscard]] const Input* getInput() const;
		[[nodiscard]] const std::shared_ptr<Input>& getInput();

	private:
		void setWindow(GLFWwindow* window);

		static GLFWwindow* createBorderlessWindow(const char* title, int width, int height, int redBits, int greenBits, int blueBits, int refreshRate);
		static GLFWwindow* createWindowedWindow(const char* title, int width, int height, int redBits, int greenBits, int blueBits, int refreshRate);
		static GLFWwindow* createFullscreenWindow(const char* title, int width, int height, int redBits, int greenBits, int blueBits, int refreshRate, GLFWmonitor* monitor);

		bool m_glewSetup = false;

		float m_deltaTime = 0.0f;
		float m_time = 0.0f;

		GLFWwindow* m_window = nullptr;

		int m_width = 0, m_height = 0;
		const char* m_title;

		std::shared_ptr<Input> m_input;
	};
}
