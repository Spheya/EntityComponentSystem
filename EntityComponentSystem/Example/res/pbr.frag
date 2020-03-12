#version 330 core

#include "pbrShading.glsl"

in vec3 p;
in vec2 uv;
in vec3 n;

uniform sampler2D baseColour;
uniform sampler2D metalness;
uniform sampler2D roughness;
uniform sampler2D normal;

uniform vec3 cameraPosition;

uniform SphericalLight sphericalLights[8];
uniform DirectionalLight directionalLights[8];

out vec4 fragColour;

void main(){
	vec3 colour = vec3(0.0);

	Material mat = Material(
			texture(baseColour, uv).rgb,
			texture(metalness, uv).r,
			texture(roughness, uv).r
	);

	vec3 toCamera = cameraPosition - p;

	for(int i = 0; i < 8; ++i) {
		colour += calcLight(directionalLights[i], mat, p, n, toCamera) * directionalLights[i].colour;
		colour += calcLight(sphericalLights[i], mat, p, n, toCamera);
	}

	colour = colour / (colour + vec3(1.0));
	colour = pow(colour, vec3(1.0 / 2.2));

	fragColour = vec4(colour, 1.0);
}
