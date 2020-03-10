#pragma once

#include <ECS/System.hpp>

#include "ModelRenderComponent.hpp"
#include "Camera.hpp"
#include "DirectionalLightSource.hpp"
#include "SphereLightSource.hpp"

namespace renderer {
	class ModelRenderSystem : public ecs::System<ModelRenderComponent> {
	public:
		ModelRenderSystem(Window* window) : Base(ecs::SystemThreadingMode::MAIN_THREAD), _window(window) {}

		void onUpdate(float, const ecs::EntityGroup<EntityData>&, ecs::ChangeBuffer&);

		void updateCamera(const Camera& camera);

		void addLightSource(DirectionalLightSource* light);
		void addLightSource(SphereLightSource* light);

		void removeLightSource(DirectionalLightSource* light);
		void removeLightSource(SphereLightSource* light);

	private:
		InstanceData _globalInstanceData;
		Window* _window;

		std::vector<DirectionalLightSource*> _directionalLights;
		std::vector<SphereLightSource*> _sphericalLights;
		
		void updateLightSourceInfo();
	};
}
