#version 330

attribute vec3 a_position;

varying vec3 v_colour;

void main() {
	v_colour = a_position + 0.5;
	gl_Position = vec4(a_position, 1.0);
}
