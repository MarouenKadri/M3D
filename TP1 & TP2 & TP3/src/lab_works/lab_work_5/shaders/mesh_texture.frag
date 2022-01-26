#version 450
in vec3 FragPos;
in vec3 Normal ;
in vec3 lightPos;
in vec2 textureCoords;
/////////////////////////////////////////////////
////////////////////////////////////////////////
uniform vec3 ambient;
uniform vec3 diff;
uniform vec3 spec;
uniform float shininess;
//////////////////////////////////////////////////
/////////////////////////////////////////////////
uniform bool uHasDiffuseMap;
uniform bool uHasAmbientMap;
uniform bool uHasShininessMap;
uniform bool uHasSpecularMap;
///////////////////////////////////////////////////
//////////////////////////////////////////////////
layout( location = 0 ) out vec4 fragColor;
layout(binding = 1) uniform sampler2D uDiffuseMap;
layout(binding = 2) uniform sampler2D uAmbientMap;
layout(binding = 3) uniform sampler2D uSpecularMap;
layout(binding = 4) uniform sampler2D uShininess;
///////////////////////////////////////////////////
///////////////////////////////////////////////////

void main()
{
	
	
	vec3 lightDir  = normalize( lightPos - FragPos );	
	
	vec3 surfaceColor = diff;
	if(uHasDiffuseMap){
		surfaceColor = vec3( texture(uDiffuseMap, textureCoords) );
	} 
	vec3 Ambient = ambient;
	if (uHasAmbientMap){
		Ambient = vec3( texture(uAmbientMap, textureCoords) );
	}
	vec3 specularSurfaceColor = spec;
	if (uHasSpecularMap){
		specularSurfaceColor = texture(uSpecularMap, textureCoords).xxx;
	}


	vec3 Diffuse = max ( dot( normalize(Normal) , lightDir ), 0.f ) * surfaceColor;
	vec3 reflectDir = reflect( - lightDir, normalize(Normal) );
	vec3 Speculair = pow( max( dot( normalize( - FragPos), reflectDir), 0.f), shininess)* specularSurfaceColor;


	vec3 result = Ambient+ Diffuse + Speculair;    //Phong Model
	fragColor = vec4( result, 1.f);

	
}
