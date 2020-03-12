#pragma once

#include <glm/glm.hpp>

namespace renderer {

	struct PointLightSource {
		PointLightSource(const glm::vec3& position, const glm::vec3& colour, float intensity) :
			position(position), colour(colour), intensity(intensity) {}

		glm::vec3 position;
		glm::vec3 colour;
		float intensity;
	};

}