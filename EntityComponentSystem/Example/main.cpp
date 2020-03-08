#define ENTITIES_PER_THREAD 100

#include <iostream>

#include <ECS/Engine.hpp>

#include <GlfwGuard.hpp>
#include <Window.hpp>
#include <Vao.hpp>
#include <ShaderProgram.hpp>
#include <TexturePack.hpp>
#include <Vbo.hpp>
#include <Camera.hpp>
#include <ModelRenderSystem.hpp>

int main() {
	renderer::GlfwGuard glfwGuard;

	renderer::Window window("Slamanadr");
	window.makeCurrentContext();
	window.enableVsync(false);

	ecs::Engine ecsEngine;

	ecsEngine.registerSystem(std::make_unique<renderer::ModelRenderSystem>());

	std::shared_ptr<renderer::ShaderProgram> shader = std::make_shared<renderer::ShaderProgram>();
	shader->load(std::vector<std::shared_ptr<renderer::Shader>>{
		std::make_shared<renderer::Shader>("default.vert", GL_VERTEX_SHADER),
			std::make_shared<renderer::Shader>("default.frag", GL_FRAGMENT_SHADER)
		},
		std::vector<std::pair<std::string, GLuint>> {
			{"a_position", 0},
		}
	);

	std::shared_ptr<renderer::Model> triangle = std::make_shared<renderer::Model>(
		std::vector<glm::vec3>{	
			glm::vec3(-0.5f, -0.5f, 0.0f),
			glm::vec3(0.0f, 0.5f, 0.0f),
			glm::vec3(0.5f, -0.5f, 0.0f) 
		}
	);

	auto& entity = ecsEngine.createEntity();
	ecsEngine.addComponent(entity, renderer::ModelRenderComponent(triangle, shader));

	while (!window.isCloseRequested()) {
		const clock_t begin_time = clock();

		ecsEngine.updateSystems(window.getDeltaTime());
		window.update();
		window.clear(true, true, false);

		GLenum error = glGetError();
		if (error != GL_NO_ERROR) 
			std::cout << "GL ERROR: " << error << std::endl;

		window.setTitle(("Slamanadr - Frame time: " + std::to_string((clock() - begin_time) / (CLOCKS_PER_SEC / 1000)) + "ms - " + std::to_string(unsigned(1.0f / window.getDeltaTime())) + " FPS").c_str());
	}
}