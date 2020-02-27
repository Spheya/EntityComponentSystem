#include "Shader.hpp"
#include <fstream>
#include <iostream>

renderer::Shader::Shader(const std::string& filename, GLenum type) :
	_type(type)
{
	loadFromFile(filename);
}

renderer::Shader::Shader(Shader&& other) noexcept {
	_id = other._id;
	_type = other._type;
	other._id = 0;
}

renderer::Shader&renderer::Shader::operator=(Shader&& other) noexcept {
	_id = other._id;
	_type = other._type;
	other._id = 0;

	return *this;
}

renderer::Shader::~Shader() {
	if(_id != 0)
		glDeleteShader(_id);
}

void renderer::Shader::loadFromString(const std::string& source) {
	// Delete the shader if it already exists
	if (_id != 0)
		glDeleteShader(_id);

	// Create a new shader
	_id = glCreateShader(_type);

	// Send the source to the gpu
	GLint length = GLint(source.length());
	auto src = static_cast<const GLchar*>(source.c_str());
	glShaderSource(_id, 1, &src, &length);

	// Compile
	glCompileShader(_id);

#ifdef _DEBUG
	// Check if the shader compiled correctly
	GLint compiled;
	glGetShaderiv(_id, GL_COMPILE_STATUS, &compiled);
	if (compiled == GL_FALSE) {
		// Print the error message
		GLint infoLength = 0;
		glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &infoLength);

		if (infoLength > 1) {
			char* info = new char[infoLength];
			glGetShaderInfoLog(_id, infoLength, nullptr, info);
			std::cout << "Error while compiling shader: \n" << info << std::endl;
			delete[] info;
		}
		glDeleteShader(_id);
		_id = 0;
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
	return _id != 0;
}

GLuint renderer::Shader::getId() const {
	return _id;
}
