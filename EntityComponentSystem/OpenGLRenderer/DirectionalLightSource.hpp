#pragma once

#include <glm/glm.hpp>

namespace renderer {

	struct DirectionalLightSource {
		DirectionalLightSource(const glm::vec3& direction, const glm::vec3& colour, float intensity) :
			direction(direction), colour(colour), intensity(intensity)
		{}

		glm::vec3 direction; // Direction where the light shines in
		glm::vec3 colour; // The colour of the light
		float intensity; // Intensity in lux
	};

}