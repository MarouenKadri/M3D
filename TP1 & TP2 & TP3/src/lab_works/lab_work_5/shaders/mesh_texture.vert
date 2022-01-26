
#version 450
layout( location = 0 ) in vec3 aVertexPosition;
layout( location = 1 ) in vec3 aVertexNormal;
layout( location = 2 ) in vec2 aVertexTexCoords;
layout( location = 3 ) in vec3 aVertexTangent;
layout( location = 4 ) in vec3 aVertexBitagent;
////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
out vec3 FragPos;
out vec3 anAdaptedVertexNormal;
out vec3 lightPos;
out vec3 Normal ; 
out vec2 textureCoords;
///////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
uniform mat4 uMVPMatrix; // Projection * View * Model
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;
uniform mat3 uNormalMatrix;	
uniform vec3 LightPos;
////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void main()
{
	gl_Position = uMVPMatrix * vec4( aVertexPosition, 1.f );
	FragPos = vec3( uViewMatrix * uModelMatrix * vec4(aVertexPosition, 1.f) );
	lightPos = vec3( uViewMatrix * vec4(LightPos, 1.f) );
	Normal = normalize( uNormalMatrix * aVertexNormal );

	if( dot(normalize(- FragPos), Normal) < 0 ){
		Normal *= -1;
	}

	textureCoords = aVertexTexCoords;
}
