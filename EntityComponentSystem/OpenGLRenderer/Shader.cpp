#include "Shader.h"
#include <fstream>
#include <iostream>

renderer::Shader::Shader(const std::string& filename, GLenum type) :
	m_type(type)
{
	loadFromFile(filename);
}

renderer::Shader::Shader(Shader&& other) noexcept {
	m_id = other.m_id;
	m_type = other.m_type;
	other.m_id = 0;
}

renderer::Shader&renderer::Shader::operator=(Shader&& other) noexcept {
	m_id = other.m_id;
	m_type = other.m_type;
	other.m_id = 0;

	return *this;
}

renderer::Shader::~Shader() {
	if(m_id != 0)
		glDeleteShader(m_id);
}

void renderer::Shader::loadFromString(const std::string& source) {
	// Delete the shader if it already exists
	if (m_id != 0)
		glDeleteShader(m_id);

	// Create a new shader
	m_id = glCreateShader(m_type);

	// Send the source to the gpu
	GLint length = GLint(source.length());
	auto src = static_cast<const GLchar*>(source.c_str());
	glShaderSource(m_id, 1, &src, &length);

	// Compile
	glCompileShader(m_id);

#ifdef _DEBUG
	// Check if the shader compiled correctly
	GLint compiled;
	glGetShaderiv(m_id, GL_COMPILE_STATUS, &compiled);
	if (compiled == GL_FALSE) {
		// Print the error message
		GLint infoLength = 0;
		glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &infoLength);

		if (infoLength > 1) {
			char* info = new char[infoLength];
			glGetShaderInfoLog(m_id, infoLength, nullptr, info);
			std::cout << "Error while compiling shader: \n" << info << std::endl;
			delete[] info;
		}
		glDeleteShader(m_id);
		m_id = 0;
		std::terminate();
	}
#endif
}

void renderer::Shader::loadFromFile(const std::string& filename) {
	std::ifstream t(filename);
	const std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
	loadFromString(str);
}

bool renderer::Shader::isValid() const {
	return m_id != 0;
}

GLuint renderer::Shader::getId() const {
	return m_id;
}
