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
#include <ShaderPreprocessor.hpp>

int main() {
	std::cout << renderer::ShaderPreprocessor::process("default.vert", false);


	renderer::GlfwGuard glfwGuard;

	renderer::Window window("Slamanadr");
	window.makeCurrentContext();
	window.enableVsync(false);

	ecs::Engine ecsEngine;

	renderer::Camera camera(0.1f, 100.0f, 70.0f);

	auto modelRenderSystem = std::make_shared<renderer::ModelRenderSystem>(&window);
	modelRenderSystem->updateCamera(camera);

	ecsEngine.registerSystem(modelRenderSystem);

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
		std::vector<GLuint> {
			0, 1, 2, 3, 2, 0
		},

		std::vector<glm::vec3>{	
			glm::vec3(-0.5f, -0.5f, 0.0f),
			glm::vec3(-0.5f, 0.5f, 0.0f),
			glm::vec3(0.5f, 0.5f, 0.0f),
			glm::vec3(0.5f, -0.5f, 0.0f) 
		}
	);

	auto& entity = ecsEngine.createEntity();
	ecsEngine.addComponent(entity, renderer::ModelRenderComponent(Transform(
		glm::vec3(1.0f, 1.0f, -2.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(1.0f))
	,triangle, shader));

	auto& entity2 = ecsEngine.createEntity();
	ecsEngine.addComponent(entity2, renderer::ModelRenderComponent(Transform(
		glm::vec3(-1.0f, -1.0f, -2.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(1.0f))
		, triangle, shader));

	while (!window.isCloseRequested()) {
		const clock_t begin_time = clock();

		ecsEngine.updateSystems(window.getDeltaTime());
		window.update();
		window.clear(true, true, false);

		entity.getComponent<renderer::ModelRenderComponent>()->transform.rotate(glm::vec3(0.0f, 0.0f, window.getDeltaTime()));
		entity2.getComponent<renderer::ModelRenderComponent>()->transform.rotate(glm::vec3(0.0f, 0.0f, window.getDeltaTime()));

		GLenum error = glGetError();
		if (error != GL_NO_ERROR) 
			std::cout << "GL ERROR: " << error << std::endl;

		window.setTitle(("Slamanadr - Frame time: " + std::to_string((clock() - begin_time) / (CLOCKS_PER_SEC / 1000)) + "ms - " + std::to_string(unsigned(1.0f / window.getDeltaTime())) + " FPS").c_str());
	}
}