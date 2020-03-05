

#define ENTITIES_PER_THREAD 100

#include <iostream>

#include <ECS/Engine.hpp>

#include <GlfwGuard.hpp>
#include <Window.hpp>
#include <RenderSystem.hpp>
#include <ShaderProgram.hpp>
#include <TexturePack.hpp>
#include <UniformPack.hpp>
#include <Util.hpp>
#include <Vbo.hpp>
#include <Camera.hpp>

int main() {
	renderer::GlfwGuard glfwGuard;
	renderer::Window window("Slamanadr");
	window.makeCurrentContext();

	ecs::Engine ecsEngine;

	ecsEngine.registerSystem<renderer::RenderSystem>(std::make_unique<renderer::RenderSystem>());

	// TODO: Make this easier
	std::shared_ptr<renderer::Model> model = std::make_shared<renderer::Model>(6 * 6, GL_TRIANGLES);
	model->bindVao();
	const auto vboHandle = model->createVbo<renderer::element::Vec3>(1, GL_STATIC_DRAW);
	auto& vbo = model->getVbo<renderer::element::Vec3>(vboHandle);
	vbo.bind();
	vbo.store(renderer::Util::UNIT_CUBE_VERTICES);
	vbo.unbind();
	renderer::Vao::unbind();

	// This is fine
	std::shared_ptr<renderer::TexturePack> texturePack = std::make_shared<renderer::TexturePack>();

	// This is fine as well, only add a custom preprocessor
	std::shared_ptr<renderer::ShaderProgram> shader = std::make_shared<renderer::ShaderProgram>();
	shader->load(std::vector<std::shared_ptr<renderer::Shader>>{
		std::make_shared<renderer::Shader>("default.vert", GL_VERTEX_SHADER),
			std::make_shared<renderer::Shader>("default.frag", GL_FRAGMENT_SHADER)
		},
		std::vector<std::pair<std::string, GLuint>> {
			{"a_position", 1},
		}
	);
	shader->addGlEnable(GL_DEPTH_TEST);

	renderer::Camera camera(0.1f, 100.0f, 70.0f);
	glm::mat4 m = camera.getProjectionMatrix(window);

	// TODO: Completely rewrite this
	Transform transform;
	renderer::UniformPack pack;
	pack.set(shader->getUniformLocation("u_modelMatrix"), &transform.getMatrix(), renderer::UniformPack::UniformType::MATRIX4F);
	pack.set(shader->getUniformLocation("u_viewMatrix"), &camera.getViewMatrix(), renderer::UniformPack::UniformType::MATRIX4F);
	pack.set(shader->getUniformLocation("u_projectionMatrix"), &m, renderer::UniformPack::UniformType::MATRIX4F);


	auto& entity = ecsEngine.createEntity();

	renderer::RenderComponent component(model, texturePack, shader, pack);
	ecsEngine.addComponent<renderer::RenderComponent>(entity, std::move(component));

	float time = 0;
	while (!window.isCloseRequested()) {
		time += window.getDeltaTime();

		transform.rotate(glm::vec3(window.getDeltaTime() * 1.5f));
		transform.setPosition(glm::vec3(sinf(time * 2.0f) * 2.5f, 0.0f, 4.0f));

		ecsEngine.updateSystems(window.getDeltaTime());
		window.update();
	}
}