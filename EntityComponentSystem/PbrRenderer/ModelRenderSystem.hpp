#pragma once

#include <ECS/System.hpp>

#include "ModelRenderComponent.hpp"
#include "Camera.hpp"
#include "DirectionalLightSource.hpp"
#include "PointLightSource.hpp"
#include <Core/Fbo.hpp>

namespace renderer {
	class ModelRenderSystem : public ecs::System<ModelRenderComponent> {
	public:
		ModelRenderSystem(Window* window, std::shared_ptr<ShaderProgram> pbrShader, std::shared_ptr<ShaderProgram> fboShader);

		void onUpdate(float, const ecs::EntityGroup<EntityData>&, ecs::ChangeBuffer&);

		void updateCamera(const Camera& camera);

		void addLightSource(DirectionalLightSource* light);
		void addLightSource(PointLightSource* light);

		void removeLightSource(DirectionalLightSource* light);
		void removeLightSource(PointLightSource* light);

	private:
		Fbo _frameBufferObject;

		size_t _texture;
		size_t _normalMap;
		size_t _reflectivityMap;

		Vao _fboModel;
		std::shared_ptr<ShaderProgram> _fboShader;

		std::shared_ptr<ShaderProgram> _shader;
		InstanceData _globalInstanceData;
		Window* _window;

		std::vector<DirectionalLightSource*> _directionalLights;
		std::vector<PointLightSource*> _pointLights;
		
		void renderModels(const ecs::EntityGroup<EntityData>& entityGroup);
		void raytraceReflections();
		void renderFbo();

		void updateLightSourceInfo();
	};
}
