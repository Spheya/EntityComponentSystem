#include "RenderSystem.hpp"

void  renderer::RenderSystem::onUpdate(float, const ecs::EntityGroup<EntityData>& entities, ecs::ChangeBuffer&) {
	for(const auto& entity : entities) {
		const auto* renderComponent = entity.getComponent<RenderComponent>();

		renderComponent->textures->bind();
		renderComponent->shader->bind();
		renderComponent->shader->dispatchPreparation();
		renderComponent->uniformPack.apply();

		callGlEnableAndDisable(*renderComponent->shader);
		renderComponent->model->prepareDraw();
		renderComponent->model->draw();
		renderComponent->model->finishDraw();

		renderComponent->shader->dispatchCleanup();
	}
}

void renderer::RenderSystem::callGlEnableAndDisable(const ShaderProgram& shader) {
	// Apply glEnable
	for(GLenum enable : shader.getEnables()) {
		const auto it = _enabled.find(enable);
		if (it == _enabled.end() || !it->second) {
			_enabled[enable] = true;
			glEnable(enable);
		}
	}

	// Apply glDisable
	for(GLenum disable : shader.getDisables()) {
		const auto it = _enabled.find(disable);
		if (it == _enabled.end() || it->second) {
			_enabled[disable] = false;
			glDisable(disable);
		}
	}
}
