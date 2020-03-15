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

uniform PointLight pointLights[8];
uniform DirectionalLight directionalLights[8];

layout (location = 0) out vec4 fragColour;
layout (location = 1) out vec4 normalMap;
layout (location = 2) out vec4 reflectionMap;

void main(){
	vec3 colour = vec3(0.0);

	Material mat = Material(
			texture(baseColour, uv).rgb,
			texture(metalness, uv).r,
			texture(roughness, uv).r
	);

	vec3 toCamera = cameraPosition - p;

	vec3 baseReflectivity = mix(vec3(0.04), mat.baseColour, mat.metalness);
	vec3 reflectionEnergy = fresnelSchlick(max(dot(n, normalize(toCamera)), 0.0), baseReflectivity);

	for(int i = 0; i < 8; ++i) {
		colour += calcLight(directionalLights[i], mat, p, n, toCamera, baseReflectivity, reflectionEnergy);
		colour += calcLight(pointLights[i], mat, p, n, toCamera, baseReflectivity, reflectionEnergy);
	}

	colour = colour / (colour + vec3(1.0));
	colour = pow(colour, vec3(1.0 / 2.2));

	fragColour = vec4(colour, 1.0);
	normalMap = vec4(n * 0.5 + vec3(0.5), 1.0);
	reflectionMap = vec4(reflectionEnergy, 1.0);
}
