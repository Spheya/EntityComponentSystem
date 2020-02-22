

#define ENTITIES_PER_THREAD 100

#include <iostream>

#include <ECS/Engine.hpp>

struct PositionComponent {
	float x, y;
};

struct VelocityComponent {
	float vx, vy;
};

struct IdComponent {
	size_t id;
};

class IdPrintSystem : public ecs::System<IdComponent> {
public:
	IdPrintSystem() : Base(true) {}

	void onUpdate(float, const ecs::EntityGroup<EntityData>& entities, ecs::ChangeBuffer&) {
		for (auto& entity : entities) {
			auto id = entity.getComponent<IdComponent>();
			std::cout << id->id << std::endl;
		}
	}
};

class PhysicsSystem : public ecs::System<PositionComponent, VelocityComponent> {
public:
	PhysicsSystem() : Base(true) {}

	void onUpdate(float, const ecs::EntityGroup<EntityData>& entities, ecs::ChangeBuffer& changeBuffer) override {
		for (auto& entity : entities) {
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

	ecsEngine.registerSystem(std::make_unique<PhysicsSystem>());
	ecsEngine.registerSystem(std::make_unique<IdPrintSystem>());

	for (int i = 0; i < 500; ++i) {
		ecs::Entity& entity = ecsEngine.createEntity();
		ecsEngine.addComponents(entity, PositionComponent{ 0.0f, 0.0f }, VelocityComponent{ 1.0f, 1.0f }, IdComponent{ size_t(i) });
	}

	//for (int i = 0; i < 1; ++i)
		ecsEngine.updateSystems(0.0f, 2);

	std::cout << "done!" << std::endl;
	int pause;
	std::cin >> pause;
}