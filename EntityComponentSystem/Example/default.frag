#version 330

varying vec3 v_colour;

out vec4 fragColour;

void main(){
	fragColour = vec4(v_colour, 1.0);
}
