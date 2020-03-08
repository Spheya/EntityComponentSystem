#include "ModelRenderSystem.hpp"

void renderer::ModelRenderSystem::onUpdate(float, const ecs::EntityGroup<EntityData>& entityGroup, ecs::ChangeBuffer&) {
	std::unordered_map<GLenum, bool> enabled;

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
}
