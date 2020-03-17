#include "Shader.hpp"
#include "ShaderProgram.hpp"
#include <iostream>
#include <cassert>

renderer::ShaderProgram::ShaderProgram(const std::vector<Shader*>& shaders, const std::vector<std::pair<std::string, GLuint>>& attributes) {
	load(shaders, attributes);
}

renderer::ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept {
	_program = other._program;
	other._program = 0;
}

renderer::ShaderProgram& renderer::ShaderProgram::operator=(ShaderProgram&& other) noexcept {
	_program = other._program;
	other._program = 0;

	return *this;
}

renderer::ShaderProgram::~ShaderProgram() {
	if (_program != 0)
		glDeleteProgram(_program);
}

void renderer::ShaderProgram::load(const std::vector<Shader*>& shaders, const std::vector<std::pair<std::string, GLuint>>& attributes) {
	if(_program != 0)
		glDeleteProgram(_program);

	_program = glCreateProgram();

	GLint maxAttribs;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttribs);

	for (const auto& shader : shaders)
		glAttachShader(_program, shader->getId());

	// Bind the attribute locations
	for (const auto& attribute : attributes)
		glBindAttribLocation(_program, attribute.second, attribute.first.c_str());

	glLinkProgram(_program);

#ifdef _DEBUG
	// Check if the shaders linked correctly
	GLint linked;
	glGetProgramiv(_program, GL_LINK_STATUS, &linked);
	if (!linked) {
		// Print the error message
		GLint infoLength = 0;
		glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &infoLength);

		if (infoLength > 1) {
			char* info = new char[infoLength];
			glGetProgramInfoLog(_program, infoLength, nullptr, info);
			std::cout << "Error while linking shaders: \n" << info << std::endl;
			delete[] info;
		}
		std::terminate();
	}
	// Check if the program is valid
	glValidateProgram(_program);
	GLint validated;
	glGetProgramiv(_program, GL_VALIDATE_STATUS, &validated);
	if (!validated) {
		std::cout << "Error while validating shaderprogram." << std::endl;
		std::terminate();
	}
#endif
}

bool renderer::ShaderProgram::isValid() const {
	return _program != 0;
}

void renderer::ShaderProgram::bind() const {
	glUseProgram(_program);
}

void renderer::ShaderProgram::bindDefaultShader() {
	glUseProgram(0);
}

void renderer::ShaderProgram::dispatchPreparation() const {
	if (_preparationFunction)
		(*_preparationFunction)();
}

void renderer::ShaderProgram::dispatchCleanup() const {
	if (_cleanupFunction)
		(*_cleanupFunction)();
}

GLint renderer::ShaderProgram::getUniformLocation(const std::string& uniform) {
	auto it = _uniforms.find(uniform);

	if (it == _uniforms.end()) {
		GLint location = glGetUniformLocation(_program, uniform.c_str());
		_uniforms.emplace(uniform, location);
		return location;
	}

	return it->second;
}

void renderer::ShaderProgram::addPreparationFunction(std::unique_ptr<std::function<void()>> preparation) {
	_preparationFunction = std::move(preparation);
}

void renderer::ShaderProgram::removePreparationFunction() {
	_preparationFunction = nullptr;
}

void renderer::ShaderProgram::addCleanupFunction(std::unique_ptr<std::function<void()>> cleanup) {
	_cleanupFunction = std::move(cleanup);
}

void renderer::ShaderProgram::removeCleanupFunction() {
	_cleanupFunction = nullptr;
}

void renderer::ShaderProgram::addEnable(GLenum value) {
	assert(std::find(_enable.begin(), _enable.end(), value) == _enable.end());
	_enable.push_back(value);
}

void renderer::ShaderProgram::removeEnable(GLenum value) {
	_enable.erase(std::find(_enable.begin(), _enable.end(), value));
}

void renderer::ShaderProgram::addDisable(GLenum value) {
	assert(std::find(_disable.begin(), _disable.end(), value) == _disable.end());
	_disable.push_back(value);
}

void renderer::ShaderProgram::removeDisable(GLenum value) {
	_disable.erase(std::find(_disable.begin(), _disable.end(), value));
}

const std::vector<GLenum>& renderer::ShaderProgram::getEnables() const {
	return _enable;
}

const std::vector<GLenum>& renderer::ShaderProgram::getDisables() const {
	return _disable;
}
