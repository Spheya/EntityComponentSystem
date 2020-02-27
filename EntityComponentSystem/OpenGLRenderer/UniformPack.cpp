#include "UniformPack.h"

bool renderer::UniformPack::remove(GLint id) {
	for(auto it = m_uniforms.begin(); it != m_uniforms.end(); ++it) {
		if(it->position == id) {
			m_uniforms.erase(it);
			return true;
		}
	}
	return false;
}

bool renderer::UniformPack::contains(GLint id) const {
	for (auto it = m_uniforms.begin(); it != m_uniforms.end(); ++it)
		if (it->position == id)
			return true;
	return false;
}

void renderer::UniformPack::set(GLint id, const void* data, UniformType type, GLsizei count) {
	// Override uniform if it exists
	for (auto it = m_uniforms.begin(); it != m_uniforms.end(); ++it) {
		if(it->position == id) {
			it->data = data;
			it->type = type;
			it->count = count;
			return;
		}
	}

	// Create a new uniform
	m_uniforms.emplace_back(id, data, type, count);
}

void renderer::UniformPack::apply() const {
	for(auto& uniform : m_uniforms) {
		switch (uniform.type) {
		case UniformType::_1F: 
			glUniform1fv(uniform.position, uniform.count, reinterpret_cast<const GLfloat*>(uniform.data));
			break;
		case UniformType::_2F:
			glUniform2fv(uniform.position, uniform.count, reinterpret_cast<const GLfloat*>(uniform.data));
			break;
		case UniformType::_3F:
			glUniform3fv(uniform.position, uniform.count, reinterpret_cast<const GLfloat*>(uniform.data));
			break;
		case UniformType::_4F:
			glUniform4fv(uniform.position, uniform.count, reinterpret_cast<const GLfloat*>(uniform.data));
			break;

		case UniformType::_1I:
			glUniform1iv(uniform.position, uniform.count, reinterpret_cast<const GLint*>(uniform.data));
			break;
		case UniformType::_2I:
			glUniform2iv(uniform.position, uniform.count, reinterpret_cast<const GLint*>(uniform.data));
			break;
		case UniformType::_3I:
			glUniform3iv(uniform.position, uniform.count, reinterpret_cast<const GLint*>(uniform.data));
			break;
		case UniformType::_4I:
			glUniform4iv(uniform.position, uniform.count, reinterpret_cast<const GLint*>(uniform.data));
			break;

		case UniformType::MATRIX2F:
			glUniformMatrix2fv(uniform.position, uniform.count, false, reinterpret_cast<const GLfloat*>(uniform.data));
			break;
		case UniformType::MATRIX3F:
			glUniformMatrix3fv(uniform.position, uniform.count, false, reinterpret_cast<const GLfloat*>(uniform.data));
			break;
		case UniformType::MATRIX4F:
			glUniformMatrix4fv(uniform.position, uniform.count, false, reinterpret_cast<const GLfloat*>(uniform.data));
			break;
		}
	}
}
