#version 330

in vec3 position;
in vec2 uvCoords;
in vec3 normal;

instance mat4 modelMatrix;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

out vec2 uv;

void main() {
	uv = uvCoords;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
}
