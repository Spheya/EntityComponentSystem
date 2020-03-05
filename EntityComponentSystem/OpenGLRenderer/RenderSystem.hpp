#pragma once

#include <ECS/System.hpp>

#include "DrawComponent.hpp"

namespace renderer {
	class RenderSystem : public ecs::System<DrawComponent> {
	public:
		RenderSystem() : Base(false) {}

		void onUpdate(float, const ecs::EntityGroup<EntityData>&, ecs::ChangeBuffer&);
	};
}