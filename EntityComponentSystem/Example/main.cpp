

#define ENTITIES_PER_THREAD 100

#include <iostream>

#include <ECS/Engine.hpp>

#include <GlfwGuard.hpp>
#include <Window.hpp>
#include <RenderSystem.hpp>
#include <ShaderProgram.hpp>
#include <TexturePack.hpp>
#include <UniformPack.hpp>

int main() {
	renderer::GlfwGuard glfwGuard;

	ecs::Engine ecsEngine;

	ecsEngine.registerSystem<renderer::RenderSystem>(std::make_unique<renderer::RenderSystem>());

	//const auto& entity = ecsEngine.createEntity();

	renderer::Window window("Slamanadr");

	while (!window.isCloseRequested()) {

		window.update();
	}
}