#version 330

attribute vec3 a_position;

instance mat4 modelMatrix;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

varying vec3 v_colour;

void main() {
	v_colour = a_position + 0.5;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(a_position, 1.0);
}
