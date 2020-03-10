#version 330

in vec3 position;
in vec2 uvCoords;
in vec3 normal;

instance mat4 modelMatrix;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

out vec3 p;
out vec2 uv;
out vec3 n;


void main() {
	vec4 worldPosition = modelMatrix * vec4(position, 1.0);

	p = worldPosition.xyz;
	uv = uvCoords;
	n = normal;

	gl_Position = projectionMatrix * viewMatrix * worldPosition;
}
