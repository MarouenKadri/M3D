#version 450

layout( location = 0 ) in vec2 aVertexPosition;
layout( location = 1 ) in vec3 in_color;

out vec3 out_color;

uniform float uTranslationX;

/*
out est déjà donné car un vertex shader rend forcément un vecteur 
à 4 dimension nommé gl_Position. Nous allons donc simplement le modifier
dans le main.
*/

void main() {

	//out_color[0] = in_color[0];
	//out_color[1] = in_color[1];
	//out_color[2] = in_color[2];
	//out_color = vec3(0.f,0.f,1.f);

	out_color = vec3(in_color[0],in_color[1],in_color[2]);
	
	gl_Position = vec4( aVertexPosition[0] + uTranslationX, aVertexPosition[1], 0, 1.f);

}
