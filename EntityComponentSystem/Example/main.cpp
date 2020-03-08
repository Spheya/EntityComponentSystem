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
	renderer::GlfwGuard glfwGuard;

	renderer::Window window("Slamanadr");
	window.makeCurrentContext();
	window.enableVsync(false);

	glEnable(GL_DEPTH_TEST);

	ecs::Engine ecsEngine;

	renderer::Camera camera(0.1f, 100.0f, 70.0f);

	auto modelRenderSystem = std::make_shared<renderer::ModelRenderSystem>(&window);

	ecsEngine.registerSystem(modelRenderSystem);

	std::shared_ptr<renderer::ShaderProgram> shader = std::make_shared<renderer::ShaderProgram>();
	shader->load(std::vector<std::shared_ptr<renderer::Shader>>{
		std::make_shared<renderer::Shader>("res/default.vert", GL_VERTEX_SHADER),
			std::make_shared<renderer::Shader>("res/default.frag", GL_FRAGMENT_SHADER)
		},
		std::vector<std::pair<std::string, GLuint>> {
			{"position", 0},
			{"uvCoords", 1},
			{"normal", 2},
		}
	);

	std::shared_ptr<renderer::Model> triangle = std::make_shared<renderer::Model>(
		"res/doof.obj"
	);

	auto& entity = ecsEngine.createEntity();
	ecsEngine.addComponent(entity, renderer::ModelRenderComponent(Transform(
		glm::vec3(1.0f, 1.0f, -5.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(1.0f))
	,triangle, shader));

	auto& entity2 = ecsEngine.createEntity();
	ecsEngine.addComponent(entity2, renderer::ModelRenderComponent(Transform(
		glm::vec3(-1.0f, -1.0f, -5.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(1.0f))
		, triangle, shader));



	window.getInput()->enableLockedMouse();

	while (!window.isCloseRequested()) {
		const clock_t begin_time = clock();

		const bool forward = window.getInput()->isKeyDown(GLFW_KEY_W) || window.getInput()->isKeyDown(GLFW_KEY_UP);
		const bool backward = window.getInput()->isKeyDown(GLFW_KEY_S) || window.getInput()->isKeyDown(GLFW_KEY_DOWN);
		const bool left = window.getInput()->isKeyDown(GLFW_KEY_A) || window.getInput()->isKeyDown(GLFW_KEY_LEFT);
		const bool right = window.getInput()->isKeyDown(GLFW_KEY_D) || window.getInput()->isKeyDown(GLFW_KEY_RIGHT);

		const bool up = window.getInput()->isKeyDown(GLFW_KEY_SPACE);
		const bool down = window.getInput()->isKeyDown(GLFW_KEY_LEFT_SHIFT) || window.getInput()->isKeyDown(GLFW_KEY_RIGHT_SHIFT);

		glm::vec3 deltaPos(
			float(right - left),
			float(up - down),
			float(forward - backward)
		);
		deltaPos *= 0.02f;

		if (forward - backward && right - left)
			deltaPos *= 0.8f;

		camera.getTransform().rotate(
			glm::vec3(
				window.getInput()->getMouseDelta().y * -1.0f,
				window.getInput()->getMouseDelta().x * 1.0f,
				0.0f
			)
		);

		camera.getTransform().move(deltaPos * window.getDeltaTime());

		camera.getTransform().move(deltaPos);

		ecsEngine.updateSystems(window.getDeltaTime());
		window.update();
		window.clear(true, true, false);

		entity.getComponent<renderer::ModelRenderComponent>()->transform.rotate(glm::vec3(0.0f, 0.0f, window.getDeltaTime()));
		entity2.getComponent<renderer::ModelRenderComponent>()->transform.rotate(glm::vec3(0.0f, 0.0f, window.getDeltaTime()));

		modelRenderSystem->updateCamera(camera);

		GLenum error = glGetError();
		if (error != GL_NO_ERROR) 
			std::cout << "GL ERROR: " << error << std::endl;

		window.setTitle(("Slamanadr - Frame time: " + std::to_string((clock() - begin_time) / (CLOCKS_PER_SEC / 1000)) + "ms - " + std::to_string(unsigned(1.0f / window.getDeltaTime())) + " FPS").c_str());
	}
}