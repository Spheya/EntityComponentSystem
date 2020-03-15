#version 330 core

uniform sampler2D tex;

in vec2 textureCoords;

out vec4 fragColour;

void main(){
	fragColour = vec4(texture(tex, textureCoords).rgb, 1.0);
}
