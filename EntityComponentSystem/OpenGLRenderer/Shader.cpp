#include "Shader.hpp"

#include <fstream>
#include <iostream>

#include "ShaderPreprocessor.hpp"

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

GLuint renderer::Shader::loadFromString(const std::string& source, GLenum type) {
	GLuint id;

	// Create a new shader
	id = glCreateShader(type);

	// Send the source to the gpu
	GLint length = GLint(source.length());
	auto src = static_cast<const GLchar*>(source.c_str());
	glShaderSource(id, 1, &src, &length);

	// Compile
	glCompileShader(id);

#ifdef _DEBUG
	// Check if the shader compiled correctly
	GLint compiled;
	glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);
	if (compiled == GL_FALSE) {
		// Print the error message
		GLint infoLength = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLength);

		if (infoLength > 1) {
			char* info = new char[infoLength];
			glGetShaderInfoLog(id, infoLength, nullptr, info);
			std::cout << "Error while compiling shader: \n" << info << std::endl;
			delete[] info;
		}
		glDeleteShader(id);
		id = 0;
		std::terminate();
	}
#endif

	return id;
}

void renderer::Shader::loadFromFile(const std::string& filename) {
	// Delete the shader if it already exists
	if (_id != 0)
		glDeleteShader(_id);

	if (_instancedId != 0)
		glDeleteShader(_instancedId);

	// Load the shader code through the preprocessor
	const std::string defaultShader = ShaderPreprocessor::process(filename, false);
	const std::string instancedShader = ShaderPreprocessor::process(filename, true);
	
	// Compile the code
	_id = loadFromString(defaultShader, _type);
	_instancedId = loadFromString(instancedShader, _type);
}

bool renderer::Shader::isValid() const {
	return _id != 0;
}

GLuint renderer::Shader::getId() const {
	return _id;
}
