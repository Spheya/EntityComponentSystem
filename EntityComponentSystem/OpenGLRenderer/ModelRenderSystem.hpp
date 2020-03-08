#pragma once

#include <ECS/System.hpp>

#include "ModelRenderComponent.hpp"
#include "Camera.hpp"

namespace renderer {
	class ModelRenderSystem : public ecs::System<ModelRenderComponent> {
	public:
		ModelRenderSystem(Window* window) : Base(ecs::SystemThreadingMode::MAIN_THREAD), _window(window) {}

		void onUpdate(float, const ecs::EntityGroup<EntityData>&, ecs::ChangeBuffer&);

		void updateCamera(const Camera& camera);

	private:
		InstanceData _globalInstanceData;
		Window* _window;
		
	};
}
