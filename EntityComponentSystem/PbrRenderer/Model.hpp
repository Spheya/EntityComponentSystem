#pragma once

#include <vector>
#include <string>

#include <glm/glm.hpp>

#include <Core/Transform.hpp>

#include <Core/Vao.hpp>
#include "InstanceData.hpp"

namespace renderer {
	class Model {
		friend class ModelRenderSystem;

	public:
		Model(const std::string file = "");
		Model(const std::vector<glm::vec3>& positions, const std::vector<glm::vec2>& uvCoords = {}, const std::vector<glm::vec3>& normals = {});
		Model(const std::vector<GLuint>& indices, const std::vector<glm::vec3>& positions, const std::vector<glm::vec2>& uvCoords = {}, const std::vector<glm::vec3>& normals = {});

	private:
		void storeVboData(const std::vector<glm::vec3>& positions, const std::vector<glm::vec2>& uvCoords, const std::vector<glm::vec3>& normals);

		Vao& getVao();
		const Vao& getVao() const;

		Vao _vao;
	};
}

