#pragma once

#include <ECS/System.hpp>

#include "ModelRenderComponent.hpp"
#include "Camera.hpp"
#include "DirectionalLightSource.hpp"
#include "PointLightSource.hpp"

namespace renderer {
	class ModelRenderSystem : public ecs::System<ModelRenderComponent> {
	public:
		ModelRenderSystem(Window* window, std::shared_ptr<ShaderProgram> pbrShader) :
			Base(ecs::SystemThreadingMode::MAIN_THREAD), _shader(pbrShader), _window(window)
		{
			_globalInstanceData.store("baseColour", 0);
			_globalInstanceData.store("metalness", 1);
			_globalInstanceData.store("roughness", 2);
			_globalInstanceData.store("normal", 3);
		}

		void onUpdate(float, const ecs::EntityGroup<EntityData>&, ecs::ChangeBuffer&);

		void updateCamera(const Camera& camera);

		void addLightSource(DirectionalLightSource* light);
		void addLightSource(PointLightSource* light);

		void removeLightSource(DirectionalLightSource* light);
		void removeLightSource(PointLightSource* light);

	private:
		std::shared_ptr<ShaderProgram> _shader;
		InstanceData _globalInstanceData;
		Window* _window;

		std::vector<DirectionalLightSource*> _directionalLights;
		std::vector<PointLightSource*> _pointLights;
		
		void updateLightSourceInfo();
	};
}
