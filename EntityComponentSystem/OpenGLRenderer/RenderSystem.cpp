#include "RenderSystem.hpp"

void renderer::RenderSystem::onUpdate(float deltatime, const ecs::EntityGroup<EntityData>& entityGroup, ecs::ChangeBuffer&) {
	std::unordered_map<GLenum, bool> enabled;

	for (auto& entity : entityGroup) {
		auto* drawComponent = entity.getComponent<DrawComponent>();

		// Bind the sahder
		drawComponent->shader->bind();
		
		// Enable and disable shader specific GL capabilities
		for (GLenum enable : drawComponent->shader->getEnables()) {
			bool& val = enabled[enable];
			if (!val) {
				glEnable(enable);
				val = true;
			}
		}
		for (GLenum disable : drawComponent->shader->getDisables()) {
			bool& val = enabled[disable];
			if (val) {
				glDisable(disable);
				val = false;
			}
		}
		
		// Dispatch some shader specific opengl calls
		drawComponent->shader->dispatchPreparation();
		
		// Bind the textures
		drawComponent->textures->bind();

		// Bind the uniforms
		drawComponent->instanceData.bindUniforms(*drawComponent->shader);

		// Draw the mesh
		drawComponent->mesh->draw();

		// Dispatch some shader specific opengl calls for cleanup
		drawComponent->shader->dispatchCleanup();
	}

}
