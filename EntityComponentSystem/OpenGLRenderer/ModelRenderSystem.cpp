#include "ModelRenderSystem.hpp"

void renderer::ModelRenderSystem::onUpdate(float, const ecs::EntityGroup<EntityData>& entityGroup, ecs::ChangeBuffer&) {
	std::unordered_map<GLenum, bool> enabled;

	updateLightSourceInfo();

	for (auto& entity : entityGroup) {
		auto* renderComponent = entity.getComponent<ModelRenderComponent>();

		// Bind the shader
		renderComponent->shader->bind();
		_globalInstanceData.bindUniforms(*renderComponent->shader);

		// Enable and disable shader specific GL capabilities
		for (GLenum enable : renderComponent->shader->getEnables()) {
			bool& val = enabled[enable];
			if (!val) {
				glEnable(enable);
				val = true;
			}
		}
		for (GLenum disable : renderComponent->shader->getDisables()) {
			bool& val = enabled[disable];
			if (val) {
				glDisable(disable);
				val = false;
			}
		}

		// Dispatch some shader specific opengl calls
		renderComponent->shader->dispatchPreparation();

		// Bind the textures
		if (renderComponent->textures)
			renderComponent->textures->bind();

		// Bind the uniforms
		renderComponent->instanceData.store("modelMatrix", renderComponent->transform.getMatrix());
		renderComponent->instanceData.bindUniforms(*renderComponent->shader);

		// Draw the mesh
		auto& vao = renderComponent->model->getVao();
		vao.bind();
		vao.draw();

		// Dispatch some shader specific opengl calls for cleanup
		renderComponent->shader->dispatchCleanup();
	}
}

void renderer::ModelRenderSystem::updateCamera(const Camera& camera) {
	_globalInstanceData.store("projectionMatrix", camera.getProjectionMatrix(*_window));
	_globalInstanceData.store("viewMatrix", camera.getViewMatrix());
	_globalInstanceData.store("cameraPosition", camera.getTransform().getPosition());
}

void renderer::ModelRenderSystem::addLightSource(DirectionalLightSource* light) {
	_directionalLights.push_back(light);
}

void renderer::ModelRenderSystem::addLightSource(SphereLightSource* light) {
	_sphericalLights.push_back(light);
}

void renderer::ModelRenderSystem::removeLightSource(DirectionalLightSource* light) {
	_directionalLights.erase(std::remove(_directionalLights.begin(), _directionalLights.end(), light), _directionalLights.end());
}

void renderer::ModelRenderSystem::removeLightSource(SphereLightSource* light) {
	_sphericalLights.erase(std::remove(_sphericalLights.begin(), _sphericalLights.end(), light), _sphericalLights.end());
}

void renderer::ModelRenderSystem::updateLightSourceInfo() {
	for (size_t i = 0; i < _directionalLights.size(); ++i) {
		_globalInstanceData.store("directionalLights[" + std::to_string(i) + "].direction", _directionalLights[i]->direction);
		_globalInstanceData.store("directionalLights[" + std::to_string(i) + "].colour", _directionalLights[i]->colour);
		_globalInstanceData.store("directionalLights[" + std::to_string(i) + "].intensity", _directionalLights[i]->intensity);
	}

	for (size_t i = 0; i < _sphericalLights.size(); ++i) {
		_globalInstanceData.store("sphericalLights[" + std::to_string(i) + "].position", _sphericalLights[i]->position);
		_globalInstanceData.store("sphericalLights[" + std::to_string(i) + "].radius", _sphericalLights[i]->radius);
		_globalInstanceData.store("sphericalLights[" + std::to_string(i) + "].colour", _sphericalLights[i]->colour);
		_globalInstanceData.store("sphericalLights[" + std::to_string(i) + "].intensity", _sphericalLights[i]->intensity);
	}
}
