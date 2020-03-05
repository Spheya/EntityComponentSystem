#pragma once

#include <memory>

#include "Mesh.hpp"
#include "ShaderProgram.hpp"
#include "TexturePack.hpp"
#include "InstanceData.hpp"

namespace renderer {
	struct DrawComponent {
		DrawComponent(std::shared_ptr<Mesh> mesh, std::shared_ptr<ShaderProgram> shader, std::shared_ptr<TexturePack> textures = nullptr) :
			mesh(mesh), shader(shader), textures(textures)
		{}

		std::shared_ptr<Mesh> mesh;
		std::shared_ptr<ShaderProgram> shader;
		std::shared_ptr<TexturePack> textures;
		InstanceData instanceData;
	};
}