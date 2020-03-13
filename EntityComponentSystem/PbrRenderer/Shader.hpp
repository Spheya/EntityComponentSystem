#pragma once
#include <unordered_map>
#include <GL/glew.h>
#include <Resource.hpp>

namespace renderer {
	class Shader : public Resource
	{
	public:
		explicit Shader(const std::string& path);
		~Shader();

		void load();

		[[nodiscard]] GLuint getId() const;

	private:
		static GLuint loadFromString(const std::string& source, GLenum type);
		
		GLenum _type = 0;
		GLuint _id = 0;
	};
}
