#pragma once

#include <glm/glm.hpp>

namespace renderer {

	struct SphereLightSource {
		SphereLightSource(const glm::vec3& position, float radius, const glm::vec3& colour, float intensity) :
			position(position), radius(radius), colour(colour), intensity(intensity) {}

		glm::vec3 position;
		float radius;
		glm::vec3 colour;
		float intensity;
	};

}