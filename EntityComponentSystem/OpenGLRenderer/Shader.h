#pragma once
#include <unordered_map>
#include <GL/glew.h>

namespace renderer {
	class Shader
	{
	public:
		Shader() = default;
		Shader(const std::string& filename, GLenum type);
		Shader(const Shader&) = delete;
		Shader& operator=(const Shader&) = delete;
		Shader(Shader&& other) noexcept;
		Shader& operator=(Shader&& other) noexcept;
		~Shader();

		void loadFromString(const std::string& source);
		void loadFromFile(const std::string& filename);

		[[nodiscard]] bool isValid() const;

		[[nodiscard]] GLuint getId() const;

	private:
		GLenum m_type = 0;
		GLuint m_id = 0;
	};
}
