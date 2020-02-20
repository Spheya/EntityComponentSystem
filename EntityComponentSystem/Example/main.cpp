
#include <iostream>

#include <Engine.hpp>

struct PositionComponent {
	float x, y;
};

struct VelocityComponent {
	float vx, vy;
};

class PhysicsSystem : public ecs::System<PositionComponent, VelocityComponent> {
	void onUpdate(float, const ecs::EntityGroup<EntityData>& entities, ecs::ChangeBuffer& changeBuffer) override {
		for (auto entity : entities) {
			auto position = entity.getComponent<PositionComponent>();
			auto velocity = entity.getComponent<VelocityComponent>();

			position->x += velocity->vx;
			position->y += velocity->vy;

			if (position->x > 1000.0f) {
				changeBuffer.removeComponent<PositionComponent>(entity.getHandle());
			}
		}
	}
};

int main() {
	ecs::Engine ecsEngine;

	ecsEngine.registerSystem<PhysicsSystem>(std::make_unique<PhysicsSystem>()/*,
											ecs::SystemSettings()
											.parallelize()*/);

	for (int i = 0; i < 100; ++i) {
		ecs::Entity& entity = ecsEngine.createEntity();
		ecsEngine.addComponents(entity, PositionComponent{ 0.0f, 0.0f }, VelocityComponent{ 1.0f, 1.0f });
	}

	for (int i = 0; i < 100000; ++i) {
		ecsEngine.updateSystems(0.0f);
	}

	std::cout << "done!" << std::endl;
}