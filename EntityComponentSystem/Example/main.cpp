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

	// Setup window
	renderer::Window window("Slamanadr");
	window.makeCurrentContext();
	window.enableVsync(false);

	// Setup ecs
	ecs::Engine ecsEngine;

	const auto modelRenderSystem = std::make_shared<renderer::ModelRenderSystem>(&window);
	ecsEngine.registerSystem(modelRenderSystem);

	// Load the shader
	std::shared_ptr<renderer::ShaderProgram> shader = std::make_shared<renderer::ShaderProgram>();
	shader->load(std::vector<std::shared_ptr<renderer::Shader>>{
		std::make_shared<renderer::Shader>("res/pbr.vert", GL_VERTEX_SHADER),
			std::make_shared<renderer::Shader>("res/pbr.frag", GL_FRAGMENT_SHADER)
		},
		std::vector<std::pair<std::string, GLuint>> {
			{"position", 0},
			{"uvCoords", 1},
			{"normal", 2},
		}
	);
	shader->addEnable(GL_DEPTH_TEST);
	shader->addEnable(GL_CULL_FACE);

	// Load textures
	auto floorTexture = std::make_shared<renderer::Texture>();
	floorTexture->loadFromFile("res/floorTexture.png", GL_NEAREST);

	auto whiteTexture = std::make_shared<renderer::Texture>();
	whiteTexture->loadFromFile("res/white.png");

	std::shared_ptr<renderer::TexturePack> floorTexturePack = std::make_shared<renderer::TexturePack>();
	const auto floorTextureId = floorTexturePack->add(floorTexture);

	std::shared_ptr<renderer::TexturePack> whiteTexturePack = std::make_shared<renderer::TexturePack>();
	const auto whiteTextureId = whiteTexturePack->add(whiteTexture);

	// Load models
	float floorSize = 40;
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

	auto testModel = std::make_shared<renderer::Model>("res/doof.obj");

	// Create entities
	for (int i = 0; i < 10; ++i) {
		auto& entity = ecsEngine.createEntity();
		ecsEngine.addComponent(entity, renderer::ModelRenderComponent(
			Transform(glm::vec3(i * 1.5f - 5.0f * 1.5f, -1.0f, -4.0f), glm::vec3(), glm::vec3(0.5f)),
			testModel,
			shader,
			whiteTexturePack
		));
	}

	auto& floorEntity = ecsEngine.createEntity();
	ecsEngine.addComponent(floorEntity, renderer::ModelRenderComponent(
		Transform(),
		floorModel,
		shader,
		floorTexturePack
	));
	floorEntity.getComponent<renderer::ModelRenderComponent>()->instanceData.store("tex", (int)floorTextureId);
	floorEntity.getComponent<renderer::ModelRenderComponent>()->transform.setScale(glm::vec3(floorSize));
	floorEntity.getComponent<renderer::ModelRenderComponent>()->transform.setPosition(glm::vec3(0.0f, -1.7f, 0.0f));

	// Create light sources
	renderer::DirectionalLightSource sun(glm::vec3(0.2f, -1.0f, 0.3f), glm::vec3(1.0f), 0.7f);
	renderer::SphereLightSource light(glm::vec3(0.0f, 2.0f, -3.0f), 0.3f, glm::vec3(1.0f, 0.7f, 0.9f), 120.0f);

	modelRenderSystem->addLightSource(&sun);
	modelRenderSystem->addLightSource(&light);

	// Setup the camera
	renderer::Camera camera(0.1f, 100.0f, 70.0f);
	window.getInput()->enableLockedMouse();

	// Gameloop
	float time = 0.0f;
	while (!window.isCloseRequested()) {
		time += window.getDeltaTime();

		camera.processDebugMovement(*window.getInput(), window.getDeltaTime());
		
		light.position = glm::vec3(sin(time) * 8, 1.0f, -4.0f + cos(time) * 4.0f);

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