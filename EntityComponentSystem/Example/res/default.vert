#version 330

attribute vec3 position;
attribute vec2 uvCoords;
attribute vec3 normal;

instance mat4 modelMatrix;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

varying vec3 v_colour;

void main() {
	v_colour = position;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
}
