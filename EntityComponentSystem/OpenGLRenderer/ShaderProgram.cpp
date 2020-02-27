#include "Shader.h"
#include "ShaderProgram.h"
#include <iostream>
#include <cassert>

renderer::ShaderProgram::ShaderProgram(std::vector<std::shared_ptr<Shader>> shaders, const std::vector<std::pair<std::string, GLuint>>& attributes) {
	load(std::move(shaders), attributes);
}

renderer::ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept {
	m_program = other.m_program;
	m_shaders = std::move(other.m_shaders);
	other.m_program = 0;
}

renderer::ShaderProgram& renderer::ShaderProgram::operator=(ShaderProgram&& other) noexcept {
	m_program = other.m_program;
	m_shaders = std::move(other.m_shaders);
	other.m_program = 0;

	return *this;
}

renderer::ShaderProgram::~ShaderProgram() {
	if (m_program != 0)
		glDeleteProgram(m_program);
}

void renderer::ShaderProgram::load(std::vector<std::shared_ptr<Shader>> shaders, const std::vector<std::pair<std::string, GLuint>>& attributes) {
	if(m_program != 0)
		glDeleteProgram(m_program);

	m_program = glCreateProgram();

	GLint maxAttribs;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttribs);

	for (const auto& shader : shaders)
		glAttachShader(m_program, shader->getId());

	// Bind the attribute locations
	for (const auto& attribute : attributes)
		glBindAttribLocation(m_program, attribute.second, attribute.first.c_str());

	glLinkProgram(m_program);

#ifdef _DEBUG
	// Check if the shaders linked correctly
	GLint linked;
	glGetProgramiv(m_program, GL_LINK_STATUS, &linked);
	if (!linked) {
		// Print the error message
		GLint infoLength = 0;
		glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &infoLength);

		if (infoLength > 1) {
			char* info = new char[infoLength];
			glGetProgramInfoLog(m_program, infoLength, nullptr, info);
			std::cout << "Error while linking shaders: \n" << info << std::endl;
			delete[] info;
		}
		std::terminate();
	}
	// Check if the program is valid
	glValidateProgram(m_program);
	GLint validated;
	glGetProgramiv(m_program, GL_VALIDATE_STATUS, &validated);
	if (!validated) {
		std::cout << "Error while validating shaderprogram." << std::endl;
		std::terminate();
	}
#endif

	m_shaders = std::move(shaders);
}

bool renderer::ShaderProgram::isValid() const {
	return m_program != 0;
}

void renderer::ShaderProgram::bind() const {
	glUseProgram(m_program);
}

void renderer::ShaderProgram::bindDefaultShader() {
	glUseProgram(0);
}

void renderer::ShaderProgram::dispatchPreparation() const {
	if (m_preparationFunction)
		(*m_preparationFunction)();
}

void renderer::ShaderProgram::dispatchCleanup() const {
	if (m_cleanupFunction)
		(*m_cleanupFunction)();
}

GLint renderer::ShaderProgram::getUniformLocation(const std::string& uniform) const {
	return glGetUniformLocation(m_program, uniform.c_str());
}

void renderer::ShaderProgram::addPreparationFunction(std::unique_ptr<std::function<void()>> preparation) {
	m_preparationFunction = std::move(preparation);
}

void renderer::ShaderProgram::removePreparationFunction() {
	m_preparationFunction = nullptr;
}

void renderer::ShaderProgram::addCleanupFunction(std::unique_ptr<std::function<void()>> cleanup) {
	m_cleanupFunction = std::move(cleanup);
}

void renderer::ShaderProgram::removeCleanupFunction() {
	m_cleanupFunction = nullptr;
}

void renderer::ShaderProgram::addGlEnable(GLenum value) {
	assert(std::find(m_enable.begin(), m_enable.end(), value) == m_enable.end());
	m_enable.push_back(value);
}

void renderer::ShaderProgram::removeGlEnable(GLenum value) {
	m_enable.erase(std::find(m_enable.begin(), m_enable.end(), value));
}

void renderer::ShaderProgram::addGlDisable(GLenum value) {
	assert(std::find(m_disable.begin(), m_disable.end(), value) == m_disable.end());
	m_disable.push_back(value);
}

void renderer::ShaderProgram::removeGlDisable(GLenum value) {
	m_disable.erase(std::find(m_disable.begin(), m_disable.end(), value));
}

const std::vector<GLenum>& renderer::ShaderProgram::getEnables() const {
	return m_enable;
}

const std::vector<GLenum>& renderer::ShaderProgram::getDisables() const {
	return m_disable;
}
