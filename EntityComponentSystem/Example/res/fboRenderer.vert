#version 330 core

in vec2 position;

out vec2 textureCoords;

void main() {
	textureCoords = position * 0.5 + vec2(0.5);

	gl_Position = vec4(position, 0.0, 1.0);
}
