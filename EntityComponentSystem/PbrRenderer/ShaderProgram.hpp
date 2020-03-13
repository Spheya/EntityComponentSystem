#pragma once
#include <unordered_map>
#include <GL/glew.h>

#include "Shader.hpp"
#include <memory>
#include <functional>

namespace renderer {
	class ShaderProgram
	{
	public:
		ShaderProgram() = default;
		explicit ShaderProgram(const std::vector<Shader*>& shaders, const std::vector<std::pair<std::string, GLuint>>& attributes);
		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;
		ShaderProgram(ShaderProgram&& other) noexcept;
		ShaderProgram& operator=(ShaderProgram&& other) noexcept;
		~ShaderProgram();

		void load(const std::vector<Shader*>& shaders, const std::vector<std::pair<std::string, GLuint>>& attributes);
		[[nodiscard]] bool isValid() const;

		void bind() const;
		static void bindDefaultShader();

		void dispatchPreparation() const;
		void dispatchCleanup() const;

		[[nodiscard]] GLint getUniformLocation(const std::string& uniform);

		void addPreparationFunction(std::unique_ptr<std::function<void()>> preparation);
		void removePreparationFunction();

		void addCleanupFunction(std::unique_ptr<std::function<void()>> cleanup);
		void removeCleanupFunction();

		void addEnable(GLenum value);
		void removeEnable(GLenum value);

		void addDisable(GLenum value);
		void removeDisable(GLenum value);

		[[nodiscard]] const std::vector<GLenum>& getEnables() const;
		[[nodiscard]] const std::vector<GLenum>& getDisables() const;

	private:
		GLuint _program = 0;

		std::unique_ptr<std::function<void()>> _preparationFunction;
		std::unique_ptr<std::function<void()>> _cleanupFunction;

		std::vector<GLenum> _disable;
		std::vector<GLenum> _enable;

		std::unordered_map<std::string, GLint> _uniforms;
	};
}
