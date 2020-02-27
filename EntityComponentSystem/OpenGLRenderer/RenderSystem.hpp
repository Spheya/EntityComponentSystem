#pragma once
#include <ECS/System.hpp>

#include "RenderComponent.hpp"
#include "Transform.hpp"

namespace renderer {

	class RenderSystem : public ecs::System<RenderComponent> {
	protected:
		void onUpdate(float deltatime, const ecs::EntityGroup<EntityData>& entities, ecs::ChangeBuffer&) override;
		//TODO : Store everything in a custom data structure to prepare for instanced rendering

	private:
		void callGlEnableAndDisable(const ShaderProgram& shader);

		std::unordered_map <GLenum, bool> _enabled;
	};

}
