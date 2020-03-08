#pragma once

#include <vector>

#include <glm/glm.hpp>

#include <Transform.hpp>

#include "Vao.hpp"
#include "InstanceData.hpp"

namespace renderer {
	class Model {
		friend class ModelRenderSystem;

	public:
		Model(const std::vector<glm::vec3>& positions, const std::vector<glm::vec2>& uvCoords = {}, const std::vector<glm::vec3>& normals = {});
		Model(const std::vector<GLuint>& indices, const std::vector<glm::vec3>& positions, const std::vector<glm::vec2>& uvCoords = {}, const std::vector<glm::vec3>& normals = {});

		InstanceData& getInstanceData();
		const InstanceData& getInstanceData() const;

	private:
		Vao& getVao();

		Vao _vao;
	};
}

