#pragma once

#include <ECS/System.hpp>

#include "DrawComponent.hpp"

namespace renderer {
	class RenderSystem : public ecs::System<DrawComponent> {
	public:
		RenderSystem() : Base(ecs::SystemThreadingMode::MAIN_THREAD) {}

		void onUpdate(float, const ecs::EntityGroup<EntityData>&, ecs::ChangeBuffer&);
	};
}