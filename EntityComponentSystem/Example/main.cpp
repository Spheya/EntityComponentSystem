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
#include <ResourceManager.hpp>

int main() {
	auto& resourceManager = ResourceManager::getInstance();

	renderer::GlfwGuard glfwGuard;

	// Setup window
	renderer::Window window("Slamanadr");
	window.makeCurrentContext();
	window.enableVsync(false);

	// Load the shader
	// TODO: make the shaderprogram a resource
	std::shared_ptr<renderer::ShaderProgram> shader = std::make_shared<renderer::ShaderProgram>();
		shader->load(std::vector<renderer::Shader*>{
			resourceManager.get<renderer::Shader>("res/pbr.vert"),
			resourceManager.get<renderer::Shader>("res/pbr.frag"),
		},
		std::vector<std::pair<std::string, GLuint>> {
			{"position", 0},
			{"uvCoords", 1},
			{"normal", 2},
		}
	);
	shader->addEnable(GL_DEPTH_TEST);
	shader->addEnable(GL_CULL_FACE);

	// Setup ecs
	ecs::Engine ecsEngine;

	const auto modelRenderSystem = std::make_shared<renderer::ModelRenderSystem>(&window, shader);
	ecsEngine.registerSystem(modelRenderSystem);

	// Load textures
	// TODO: Make materials resources
	auto floorTexture = resourceManager.get<renderer::Texture>("res/floorTexture.png");
	floorTexture->setFilter(GL_NEAREST);

	auto metalBaseColour = resourceManager.get<renderer::Texture>("res/metal_basecolour.jpg");
	auto metalMetalness  = resourceManager.get<renderer::Texture>("res/metal_metalness.jpg");
	auto metalRoughness  = resourceManager.get<renderer::Texture>("res/metal_roughness.jpg");
	auto metalNormal     = resourceManager.get<renderer::Texture>("res/metal_normal.jpg");

	std::shared_ptr<renderer::Material> floorMaterial = std::make_shared<renderer::Material>(
		floorTexture, 0.0f, 0.15f, glm::vec3(0.0f, 0.0f, 1.0f)
	);
	std::shared_ptr<renderer::Material> metalMaterial = std::make_shared<renderer::Material>(
		metalBaseColour, metalMetalness, metalRoughness, metalNormal
	);

	// Load models
	float floorSize = 4000;
	auto floorModel = std::make_shared<renderer::Model>(
		std::vector<GLuint>{ 0, 2, 1, 3, 2, 0 },
		std::vector<glm::vec3>{
			glm::vec3(-0.5f, 0.0f, +0.5f),
			glm::vec3(-0.5f, 0.0f, -0.5f),
			glm::vec3(+0.5f, 0.0f, -0.5f),
			glm::vec3(+0.5f, 0.0f, +0.5f)
		},
		std::vector<glm::vec2>{
			glm::vec2(            0.0f, floorSize * 0.5f),
			glm::vec2(            0.0f,             0.0f),
			glm::vec2(floorSize * 0.5f,             0.0f),
			glm::vec2(floorSize * 0.5f, floorSize * 0.5f)
		},
		std::vector<glm::vec3>{
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f)
		}
	);

	auto testModel = resourceManager.get<renderer::Model>("res/sphere.obj");

	// Create entities
	for (int x = 0; x < 20; ++x) {
		for (int y = 0; y < 1; ++y) {
			auto& entity = ecsEngine.createEntity();
			ecsEngine.addComponent(entity, renderer::ModelRenderComponent(
				Transform(glm::vec3(x * 1.5f - 5.0f * 1.5f, -1.0f + y * 1.5f, -4.0f), glm::vec3(), glm::vec3(0.75f)),
				testModel,
				metalMaterial.get()
			));
		}
	}

	auto& floorEntity = ecsEngine.createEntity();
	ecsEngine.addComponent(floorEntity, renderer::ModelRenderComponent(
		Transform(),
		floorModel.get(),
		floorMaterial.get()
	));
	floorEntity.getComponent<renderer::ModelRenderComponent>()->transform.setScale(glm::vec3(floorSize));
	floorEntity.getComponent<renderer::ModelRenderComponent>()->transform.setPosition(glm::vec3(0.0f, -1.7f, 0.0f));

	// Create light sources
	renderer::DirectionalLightSource sun(glm::vec3(0.2f, -1.0f, -0.5f), glm::vec3(0.95f, 0.90f, 1.0f), 0.5f);
	renderer::PointLightSource light(glm::vec3(0.0f, 2.0f, 5.0f * 1.5f), glm::vec3(1.0f, 0.2f, 0.9f), 500.0f);
	renderer::PointLightSource light2(glm::vec3(0.0f, 2.0f, 5.0f * 1.5f), glm::vec3(1.0f, 0.2f, 0.9f), 500.0f);

	modelRenderSystem->addLightSource(&sun);
	modelRenderSystem->addLightSource(&light);
	modelRenderSystem->addLightSource(&light2);

	// Setup the camera
	renderer::Camera camera(0.1f, 100.0f, 70.0f);
	window.getInput()->enableLockedMouse();

	// Gameloop
	float time = 0.0f;
	while (!window.isCloseRequested()) {
		time += window.getDeltaTime();

		camera.processDebugMovement(*window.getInput(), window.getDeltaTime());
		
		if (window.getInput()->isMouseButtonDown(GLFW_MOUSE_BUTTON_1))
			light.position = camera.getTransform().getPosition();

		if (window.getInput()->isMouseButtonDown(GLFW_MOUSE_BUTTON_2))
			light2.position = camera.getTransform().getPosition();

		modelRenderSystem->updateCamera(camera);

		ecsEngine.updateSystems(window.getDeltaTime());

		window.update();
		window.clear(true, true, false);
		window.setTitle(("Slamanadr - Frame time: " + std::to_string(window.getDeltaTime() * 1000.0f) + "ms - " + std::to_string(unsigned(1.0f / window.getDeltaTime())) + " FPS").c_str());

#ifdef _DEBUG
		GLenum error = glGetError();
		if (error != GL_NO_ERROR) 
			std::cout << "GL ERROR: " << error << std::endl;
#endif
	}
}