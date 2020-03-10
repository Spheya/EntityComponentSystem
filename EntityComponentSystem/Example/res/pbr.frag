#version 330

#include "pbrShading.glsl"

in vec3 p;
in vec2 uv;
in vec3 n;

uniform sampler2D tex;
uniform vec3 cameraPosition;

uniform SphericalLight sphericalLights[8];
uniform DirectionalLight directionalLights[8];

out vec4 fragColour;

void main(){
	vec3 colour;
	vec3 baseColour = texture(tex, uv).rgb;

	vec3 toCamera = p - cameraPosition;

	for(int i = 0; i < 8; ++i) {
		colour += calcLight(directionalLights[i], baseColour, p, n, toCamera) * directionalLights[i].colour;
		colour += calcLight(sphericalLights[i],   baseColour, p, n, toCamera);
	}

	fragColour = /*texture(tex, uv) */ vec4(colour, 1.0);
}
