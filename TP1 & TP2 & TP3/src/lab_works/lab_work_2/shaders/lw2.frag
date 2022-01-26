#version 450

in vec3 out_color;
uniform float uLuminosity;

layout( location = 0 ) out vec4 fragcolor;


void main() {

	//printf("test");// << "r:" << aVectorColor2[0] << " g:" << aVectorColor2[1] << " b:" << aVectorColor2[2] << "\n";
	fragcolor = vec4(out_color * vec3(uLuminosity), 1);

}