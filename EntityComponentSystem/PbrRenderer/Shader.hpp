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

		void loadFromFile(const std::string& filename);

		[[nodiscard]] bool isValid() const;

		[[nodiscard]] GLuint getId() const;

	private:
		static GLuint loadFromString(const std::string& source, GLenum type);
		
		GLenum _type = 0;
		GLuint _id = 0;
		GLuint _instancedId = 0;
	};
}
