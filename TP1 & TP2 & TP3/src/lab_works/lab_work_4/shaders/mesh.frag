#version 450
layout( location = 0 ) out vec4 fragColor;
////////////////////////////////////////
///////////////////////////////////////
in vec3 FragPos;
in vec3 Normal ;
in vec3 lightPos;
////////////////////////////////////
///////////////////////////////////
uniform vec3 ambient;
uniform vec3 diff;
uniform vec3 spec;
uniform float shininess;
/////////////////////////////////
////////////////////////////////


void main()
{

	
	vec3 lightDir  = normalize( lightPos - FragPos );	
	vec3 Ambient = ambient;
	vec3 Diffuse = max ( dot( normalize(Normal) , lightDir ), 0.f ) * diff;

	vec3 reflectDir = reflect( - lightDir, normalize(Normal) );
	vec3 Speculair = pow( max( dot( normalize( - FragPos), reflectDir), 0.f), shininess)* spec;

	vec3 result = Ambient+ Diffuse + Speculair;    //Phong Model

	fragColor = vec4( result, 1.f);
}
