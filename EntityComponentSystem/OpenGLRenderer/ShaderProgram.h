#pragma once
#include <unordered_map>
#include <GL/glew.h>

#include "Shader.h"
#include <memory>
#include <functional>

namespace renderer {
	class ShaderProgram
	{
	public:
		ShaderProgram() = default;
		explicit ShaderProgram(std::vector<std::shared_ptr<Shader>> shaders, const std::vector<std::pair<std::string, GLuint>>& attributes);
		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;
		ShaderProgram(ShaderProgram&& other) noexcept;
		ShaderProgram& operator=(ShaderProgram&& other) noexcept;
		~ShaderProgram();

		void load(std::vector<std::shared_ptr<Shader>> shaders, const std::vector<std::pair<std::string, GLuint>>& attributes);
		[[nodiscard]] bool isValid() const;

		void bind() const;
		static void bindDefaultShader();

		void dispatchPreparation() const;
		void dispatchCleanup() const;

		[[nodiscard]] GLint getUniformLocation(const std::string& uniform) const;

		void addPreparationFunction(std::unique_ptr<std::function<void()>> preparation);
		void removePreparationFunction();

		void addCleanupFunction(std::unique_ptr<std::function<void()>> cleanup);
		void removeCleanupFunction();

		void addGlEnable(GLenum value);
		void removeGlEnable(GLenum value);

		void addGlDisable(GLenum value);
		void removeGlDisable(GLenum value);

		[[nodiscard]] const std::vector<GLenum>& getEnables() const;
		[[nodiscard]] const std::vector<GLenum>& getDisables() const;

	private:
		GLuint m_program = 0;

		std::unique_ptr<std::function<void()>> m_preparationFunction;
		std::unique_ptr<std::function<void()>> m_cleanupFunction;

		std::vector<GLenum> m_disable;
		std::vector<GLenum> m_enable;

		// This data isn't being used, but owning the shaders makes sure they are kept alive
		std::vector<std::shared_ptr<Shader>> m_shaders;
	};
}
