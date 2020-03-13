#include "Model.hpp"

#include <iostream>

#pragma warning(push, 0)
#pragma warning(disable : 4701)
#include <tiny_obj_loader.h>
#pragma warning(pop)

renderer::Model::Model(const std::string file) :
	Resource(file),
	_vao(GL_TRIANGLES, 0)
{}

renderer::Model::Model(const std::vector<glm::vec3>& positions, const std::vector<glm::vec2>& uvCoords, const std::vector<glm::vec3>& normals) :
	Resource(""),
	_vao(GL_TRIANGLES, positions.size())
{
	storeVboData(positions, uvCoords, normals);
}

renderer::Model::Model(const std::vector<GLuint>& indices, const std::vector<glm::vec3>& positions, const std::vector<glm::vec2>& uvCoords, const std::vector<glm::vec3>& normals) :
	Model(positions, uvCoords, normals)
{
	_vao.setIndexBuffer(indices);
}

void renderer::Model::load() {
	objl::Loader loader;

	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> uvCoords;
	std::vector<glm::vec3> normals;

	if (!loader.LoadFile(getPath()))
		std::terminate();

	for (const auto& vertex : loader.LoadedVertices) {
		positions.emplace_back(vertex.Position.X, vertex.Position.Y, vertex.Position.Z);
		uvCoords.emplace_back(vertex.TextureCoordinate.X, vertex.TextureCoordinate.Y);
		normals.emplace_back(vertex.Normal.X, vertex.Normal.Y, vertex.Normal.Z);
	}

	storeVboData(positions, uvCoords, normals);
	_vao.setIndexBuffer(loader.LoadedIndices);
}

void renderer::Model::storeVboData(const std::vector<glm::vec3>& positions, const std::vector<glm::vec2>& uvCoords, const std::vector<glm::vec3>& normals) {
	if (!positions.empty()) {
		auto positionsVbo = _vao.createVbo();
		_vao.getVbo(positionsVbo).write(positions.size() * sizeof(glm::vec3), &positions[0]);
		_vao.getVbo(positionsVbo).bindAttribPointer(0, 3, GL_FLOAT);
	}

	if (!uvCoords.empty()) {
		auto uvCoordsVbo = _vao.createVbo();
		_vao.getVbo(uvCoordsVbo).write(uvCoords.size() * sizeof(glm::vec2), &uvCoords[0]);
		_vao.getVbo(uvCoordsVbo).bindAttribPointer(1, 2, GL_FLOAT);
	}

	if (!normals.empty()) {
		auto normalsVbo = _vao.createVbo();
		_vao.getVbo(normalsVbo).write(normals.size() * sizeof(glm::vec3), &normals[0]);
		_vao.getVbo(normalsVbo).bindAttribPointer(2, 3, GL_FLOAT);
	}
}

renderer::Vao& renderer::Model::getVao() {
	return _vao;
}
