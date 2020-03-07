#pragma once

#include <memory>

#include "Mesh.hpp"
#include "ShaderProgram.hpp"
#include "TexturePack.hpp"
#include "InstanceData.hpp"
#include "Window.hpp"

namespace renderer {
	struct DrawComponent {
		DrawComponent(Window* window, std::shared_ptr<Mesh> mesh, std::shared_ptr<ShaderProgram> shader, std::shared_ptr<TexturePack> textures = nullptr) :
			window(window), mesh(mesh), shader(shader), textures(textures)
		{}

		Window* window;

		std::shared_ptr<Mesh> mesh;
		std::shared_ptr<ShaderProgram> shader;
		std::shared_ptr<TexturePack> textures;
		InstanceData instanceData;
	};
}