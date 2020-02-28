#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Input.hpp"

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

		bool _glewSetup = false;

		float _deltaTime = 0.0f;
		float _time = 0.0f;

		GLFWwindow* _window = nullptr;

		int _width = 0, _height = 0;
		const char* _title;

		std::shared_ptr<Input> _input;
	};
}
