#define ENTITIES_PER_THREAD 100

#include <iostream>

#include <ECS/Engine.hpp>

#include <GlfwGuard.hpp>
#include <Window.hpp>
#include <RenderSystem.hpp>
#include <ShaderProgram.hpp>
#include <TexturePack.hpp>
#include <Vbo.hpp>
#include <Camera.hpp>

int main() {
	renderer::GlfwGuard glfwGuard;
	renderer::Window window("Slamanadr");
	window.makeCurrentContext();

	ecs::Engine ecsEngine;

	ecsEngine.registerSystem<renderer::RenderSystem>(std::make_unique<renderer::RenderSystem>());

	std::shared_ptr<renderer::ShaderProgram> shader = std::make_shared<renderer::ShaderProgram>();
	shader->load(std::vector<std::shared_ptr<renderer::Shader>>{
		std::make_shared<renderer::Shader>("default.vert", GL_VERTEX_SHADER),
			std::make_shared<renderer::Shader>("default.frag", GL_FRAGMENT_SHADER)
		},
		std::vector<std::pair<std::string, GLuint>> {
			{"a_position", 1},
		}
	);

	std::shared_ptr<renderer::Mesh> triangle = std::make_shared<renderer::Mesh>(GL_TRIANGLES, 3);
	auto positionsVbo = triangle->createVbo();
	GLfloat positions[] = {
		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f,
		1.0f, 0.0f, 0.0f 
	};
	triangle->getVbo(positionsVbo).write(3 * 3 * sizeof(GL_FLOAT), positions);
	triangle->getVbo(positionsVbo).bindAttribPointer(1, 3, GL_FLOAT);

	auto& entity = ecsEngine.createEntity();
	ecsEngine.addComponent(entity, renderer::DrawComponent(triangle, shader));

	float time = 0;
	while (!window.isCloseRequested()) {
		time += window.getDeltaTime();

		ecsEngine.updateSystems(window.getDeltaTime());
		window.update();
	}
}