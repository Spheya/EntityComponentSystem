
#include <iostream>

#include <Engine.hpp>

struct PositionComponent {
	float x, y;
};

struct VelocityComponent {
	float vx, vy;
};

class PhysicsSystem : public ecs::System<PositionComponent, VelocityComponent> {
	void update(float deltatime, /*ecs::SystemData& data, ecs::EcsChangeBuffer changeBuffer*/) override {
		int i = 0;
		for (auto entity : /*data.*/getEntities()) {
			/*auto& position = std::get<PositionComponent>(entity.components);
			auto& velocity = std::get<VelocityComponent>(entity.);

			position.x += velocity.vx;
			position.y += velocity.vy;

			if (position.x > 1000.0f) {
				//changeBuffer.RemoveComponent<VelocityComponent>();
			}*/
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