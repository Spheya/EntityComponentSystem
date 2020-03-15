#include "ModelRenderSystem.hpp"

renderer::ModelRenderSystem::ModelRenderSystem(Window* window, std::shared_ptr<ShaderProgram> pbrShader, std::shared_ptr<ShaderProgram> fboShader) :
	Base(ecs::SystemThreadingMode::MAIN_THREAD),
	_frameBufferObject(int(window->getWindowSize().x), int(window->getWindowSize().y)),
	_fboModel(GL_TRIANGLE_STRIP, 4),
	_fboShader(std::move(fboShader)),
	_shader(std::move(pbrShader)),
	_window(window)
{
	// TODO: Bind uniforms when binding the shader instead of using instancedata
	_globalInstanceData.store("baseColour", 0);
	_globalInstanceData.store("metalness", 1);
	_globalInstanceData.store("roughness", 2);
	_globalInstanceData.store("normal", 3);

	// Create a model to draw the fbo on
	float positions[] = {
		+1.0f, +1.0f,
		+1.0f, -1.0f,
		-1.0f, +1.0f,
		-1.0f, -1.0f
	};
	auto vbo = _fboModel.createVbo();
	_fboModel.getVbo(vbo).write(sizeof(positions), positions);
	_fboModel.getVbo(vbo).bindAttribPointer(0, 2, GL_FLOAT);

	_fboShader->bind();
	glUniform1i(_fboShader->getUniformLocation("tex"), 0);

	_frameBufferObject.bind();
	_texture = _frameBufferObject.createTexture();
	_normalMap = _frameBufferObject.createTexture();
	_reflectivityMap = _frameBufferObject.createTexture();
	_frameBufferObject.unbind();
}

void renderer::ModelRenderSystem::onUpdate(float, const ecs::EntityGroup<EntityData>& entityGroup, ecs::ChangeBuffer&) {
	_frameBufferObject.bind();
	GLenum buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, buffers);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	renderModels(entityGroup);

	_frameBufferObject.unbind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_CULL_FACE);
	renderFbo();

	glBindTexture(GL_TEXTURE_2D, 0);
}

void renderer::ModelRenderSystem::updateCamera(const Camera& camera) {
	_globalInstanceData.store("projectionMatrix", camera.getProjectionMatrix(*_window));
	_globalInstanceData.store("viewMatrix", camera.getViewMatrix());
	_globalInstanceData.store("cameraPosition", camera.getTransform().getPosition());
}

void renderer::ModelRenderSystem::addLightSource(DirectionalLightSource* light) {
	_directionalLights.push_back(light);
}

void renderer::ModelRenderSystem::addLightSource(PointLightSource* light) {
	_pointLights.push_back(light);
}

void renderer::ModelRenderSystem::removeLightSource(DirectionalLightSource* light) {
	_directionalLights.erase(std::remove(_directionalLights.begin(), _directionalLights.end(), light), _directionalLights.end());
}

void renderer::ModelRenderSystem::removeLightSource(PointLightSource* light) {
	_pointLights.erase(std::remove(_pointLights.begin(), _pointLights.end(), light), _pointLights.end());
}

void renderer::ModelRenderSystem::renderModels(const ecs::EntityGroup<EntityData>& entityGroup) {
	std::unordered_map<GLenum, bool> enabled;

	updateLightSourceInfo();

	for (auto& entity : entityGroup) {
		auto* renderComponent = entity.getComponent<ModelRenderComponent>();

		// Bind the shader
		_shader->bind();
		_globalInstanceData.bindUniforms(*_shader);

		// Enable and disable shader specific GL capabilities
		for (GLenum enable : _shader->getEnables()) {
			bool& val = enabled[enable];
			if (!val) {
				glEnable(enable);
				val = true;
			}
		}
		for (GLenum disable : _shader->getDisables()) {
			bool& val = enabled[disable];
			if (val) {
				glDisable(disable);
				val = false;
			}
		}

		// Dispatch some shader specific opengl calls
		_shader->dispatchPreparation();

		// Bind the textures
		renderComponent->material->baseColour.getTexture()->bind(0);
		renderComponent->material->metalness.getTexture()->bind(1);
		renderComponent->material->roughness.getTexture()->bind(2);
		renderComponent->material->normal.getTexture()->bind(3);

		// Bind the uniforms
		glUniformMatrix4fv(_shader->getUniformLocation("modelMatrix"), 1, GL_FALSE, &renderComponent->transform.getMatrix()[0][0]);

		// Draw the mesh
		auto& vao = renderComponent->model->getVao();
		vao.bind();
		vao.draw();

		// Dispatch some shader specific opengl calls for cleanup
		_shader->dispatchCleanup();
	}
}

void renderer::ModelRenderSystem::raytraceReflections() {}

void renderer::ModelRenderSystem::renderFbo() {
	glDisable(GL_DEPTH_TEST);

	_frameBufferObject.getTexture(_reflectivityMap)->bind(0);

	_fboShader->bind();

	_fboModel.bind();
	_fboModel.draw();
}

void renderer::ModelRenderSystem::updateLightSourceInfo() {
	for (size_t i = 0; i < _directionalLights.size(); ++i) {
		_globalInstanceData.store("directionalLights[" + std::to_string(i) + "].direction", _directionalLights[i]->direction);
		_globalInstanceData.store("directionalLights[" + std::to_string(i) + "].colour",    _directionalLights[i]->colour);
		_globalInstanceData.store("directionalLights[" + std::to_string(i) + "].intensity", _directionalLights[i]->intensity);
	}

	for (size_t i = 0; i < _pointLights.size(); ++i) {
		_globalInstanceData.store("pointLights[" + std::to_string(i) + "].position",  _pointLights[i]->position);
		_globalInstanceData.store("pointLights[" + std::to_string(i) + "].colour",    _pointLights[i]->colour);
		_globalInstanceData.store("pointLights[" + std::to_string(i) + "].intensity", _pointLights[i]->intensity);
	}
}
