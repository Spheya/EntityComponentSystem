#include "InstanceData.hpp"

#include <GL/glew.h>

#include <glm/glm.hpp>
#include "InstanceData.hpp"

void renderer::InstanceData::Uniform<GLfloat>::bind(ShaderProgram& program, const std::string& name) const {
	glUniform1fv(program.getUniformLocation(name), 1, &data);
}

void renderer::InstanceData::Uniform<glm::fvec1>::bind(ShaderProgram& program, const std::string& name) const {
	glUniform1fv(program.getUniformLocation(name), 1, &data.x);
}

void renderer::InstanceData::Uniform<glm::fvec2>::bind(ShaderProgram& program, const std::string& name) const {
	glUniform2fv(program.getUniformLocation(name), 1, &data.x);
}

void renderer::InstanceData::Uniform<glm::fvec3>::bind(ShaderProgram& program, const std::string& name) const {
	glUniform3fv(program.getUniformLocation(name), 1, &data.x);
}

void renderer::InstanceData::Uniform<glm::fvec4>::bind(ShaderProgram& program, const std::string& name) const {
	glUniform4fv(program.getUniformLocation(name), 1, &data.x);
}

void renderer::InstanceData::Uniform<GLint>::bind(ShaderProgram& program, const std::string& name) const {
	glUniform1iv(program.getUniformLocation(name), 1, &data);
}

void renderer::InstanceData::Uniform<glm::ivec1>::bind(ShaderProgram& program, const std::string& name) const {
	glUniform1iv(program.getUniformLocation(name), 1, &data.x);
}

void renderer::InstanceData::Uniform<glm::ivec2>::bind(ShaderProgram& program, const std::string& name) const {
	glUniform2iv(program.getUniformLocation(name), 1, &data.x);
}

void renderer::InstanceData::Uniform<glm::ivec3>::bind(ShaderProgram& program, const std::string& name) const {
	glUniform3iv(program.getUniformLocation(name), 1, &data.x);
}

void renderer::InstanceData::Uniform<glm::ivec4>::bind(ShaderProgram& program, const std::string& name) const {
	glUniform4iv(program.getUniformLocation(name), 1, &data.x);
}

void renderer::InstanceData::Uniform<GLuint>::bind(ShaderProgram& program, const std::string& name) const {
	glUniform1uiv(program.getUniformLocation(name), 1, &data);
}

void renderer::InstanceData::Uniform<glm::uvec1>::bind(ShaderProgram& program, const std::string& name) const {
	glUniform1uiv(program.getUniformLocation(name), 1, &data.x);
}

void renderer::InstanceData::Uniform<glm::uvec2>::bind(ShaderProgram& program, const std::string& name) const {
	glUniform2uiv(program.getUniformLocation(name), 1, &data.x);
}

void renderer::InstanceData::Uniform<glm::uvec3>::bind(ShaderProgram& program, const std::string& name) const {
	glUniform3uiv(program.getUniformLocation(name), 1, &data.x);
}

void renderer::InstanceData::Uniform<glm::uvec4>::bind(ShaderProgram& program, const std::string& name) const {
	glUniform4uiv(program.getUniformLocation(name), 1, &data.x);
}

void renderer::InstanceData::Uniform<glm::mat2>::bind(ShaderProgram& program, const std::string& name) const {
	glUniformMatrix2fv(program.getUniformLocation(name), 1, false, & data[0][0]);
}

void renderer::InstanceData::Uniform<glm::mat3>::bind(ShaderProgram& program, const std::string& name) const {
	glUniformMatrix3fv(program.getUniformLocation(name), 1, false, &data[0][0]);
}

void renderer::InstanceData::Uniform<glm::mat4>::bind(ShaderProgram& program, const std::string& name) const {
	glUniformMatrix4fv(program.getUniformLocation(name), 1, false, &data[0][0]);
}

void renderer::InstanceData::Uniform<glm::mat2x3>::bind(ShaderProgram& program, const std::string& name) const {
	glUniformMatrix2x3fv(program.getUniformLocation(name), 1, false, &data[0][0]);
}

void renderer::InstanceData::Uniform<glm::mat3x2>::bind(ShaderProgram& program, const std::string& name) const {
	glUniformMatrix3x2fv(program.getUniformLocation(name), 1, false, &data[0][0]);
}

void renderer::InstanceData::Uniform<glm::mat2x4>::bind(ShaderProgram& program, const std::string& name) const {
	glUniformMatrix2x4fv(program.getUniformLocation(name), 1, false, &data[0][0]);
}

void renderer::InstanceData::Uniform<glm::mat4x2>::bind(ShaderProgram& program, const std::string& name) const {
	glUniformMatrix4x2fv(program.getUniformLocation(name), 1, false, &data[0][0]);
}

void renderer::InstanceData::Uniform<glm::mat3x4>::bind(ShaderProgram& program, const std::string& name) const {
	glUniformMatrix3x4fv(program.getUniformLocation(name), 1, false, &data[0][0]);
}

void renderer::InstanceData::Uniform<glm::mat4x3>::bind(ShaderProgram& program, const std::string& name) const {
	glUniformMatrix4x3fv(program.getUniformLocation(name), 1, false, &data[0][0]);
}

void renderer::InstanceData::bindUniforms(ShaderProgram& program) {
	for (const auto& uniform : _uniforms)
		uniform.second->bind(program, uniform.first);
}

void renderer::InstanceData::addToInstancedVector(std::vector<uint8_t>& vec, const std::vector<std::string> order) {
	for (const auto& name : order)
		_uniforms.find(name)->second->addToInstancedVector(vec);
}
