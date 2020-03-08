#include "Model.hpp"

renderer::Model::Model(const std::vector<glm::vec3>& positions, const std::vector<glm::vec2>& uvCoords, const std::vector<glm::vec3>& normals) :
	_vao(GL_TRIANGLES, positions.size())
{
	if (!positions.empty()) {
		auto positionsVbo = _vao.createVbo();
		_vao.getVbo(positionsVbo).write(positions.size() * sizeof(glm::vec3), &positions[0]);
		_vao.getVbo(positionsVbo).bindAttribPointer(0, 3, GL_FLOAT);
	}

	if (!uvCoords.empty()) {
		auto uvCoordsVbo = _vao.createVbo();
		_vao.getVbo(uvCoordsVbo).write(uvCoords.size() * sizeof(glm::vec2), &uvCoords[0]);
		_vao.getVbo(uvCoordsVbo).bindAttribPointer(0, 2, GL_FLOAT);
	}

	if (!normals.empty()) {
		auto normalsVbo = _vao.createVbo();
		_vao.getVbo(normalsVbo).write(normals.size() * sizeof(glm::vec3), &normals[0]);
		_vao.getVbo(normalsVbo).bindAttribPointer(0, 3, GL_FLOAT);
	}
}

renderer::InstanceData& renderer::Model::getInstanceData() {
	return _instanceData;
}

const renderer::InstanceData& renderer::Model::getInstanceData() const {
	return _instanceData;
}

void renderer::Model::setTransform(const Transform& transform) {
	_transform = transform;
}

Transform& renderer::Model::getTransform() {
	return _transform;
}

const Transform& renderer::Model::getTransform() const {
	return _transform;
}

void renderer::Model::updateInstanceData() {
	_instanceData.store("transformationMatrix", _transform.getMatrix());
}

renderer::Vao& renderer::Model::getVao() {
	return _vao;
}
