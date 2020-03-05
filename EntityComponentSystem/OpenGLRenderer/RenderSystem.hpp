#pragma once

#include <ECS/System.hpp>

#include "DrawComponent.hpp"

namespace renderer {
	class RenderSystem : ecs::System<DrawComponent> {
		RenderSystem() : Base(false) {}

		void onUpdate(float deltatime, const ecs::EntityGroup<EntityData>& entityGroup, ecs::ChangeBuffer&);
	};
}