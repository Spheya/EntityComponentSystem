#pragma once

#include <ECS/System.hpp>

#include "ModelRenderComponent.hpp"

namespace renderer {
	class ModelRenderSystem : public ecs::System<ModelRenderComponent> {
	public:
		RenderSystem() : Base(ecs::SystemThreadingMode::MAIN_THREAD) {}

		void onUpdate(float, const ecs::EntityGroup<EntityData>&, ecs::ChangeBuffer&);
	};
}
