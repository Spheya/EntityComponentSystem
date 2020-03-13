#include "Shader.hpp"

#include <fstream>
#include <iostream>

#include "ShaderPreprocessor.hpp"

renderer::Shader::Shader(const std::string& path) :
	Resource(path) {
	if (getFileExtension() == ".vert") {
		_type = GL_VERTEX_SHADER;
	} else if (getFileExtension() == ".frag") {
		_type = GL_FRAGMENT_SHADER;
	} else if (getFileExtension() == ".comp") {
		_type = GL_COMPUTE_SHADER;
	} else if (getFileExtension() == ".geom") {
		_type = GL_GEOMETRY_SHADER;
	} else if (getFileExtension() == ".tctrl") {
		_type = GL_TESS_CONTROL_SHADER;
	} else if (getFileExtension() == ".teval") {
		_type = GL_TESS_EVALUATION_SHADER;
	}
}

renderer::Shader::~Shader() {
	glDeleteShader(_id);
}

void renderer::Shader::load() {
	// Load the shader code through the preprocessor
	const std::string defaultShader = ShaderPreprocessor::process(getPath());

	// Compile the code
	_id = loadFromString(defaultShader, _type);
}

GLuint renderer::Shader::getId() const {
	return _id;
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