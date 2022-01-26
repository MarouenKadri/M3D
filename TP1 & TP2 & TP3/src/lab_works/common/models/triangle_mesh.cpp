#include "triangle_mesh.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <iostream>

namespace M3D_ISICG
{
	TriangleMesh::TriangleMesh( const std::string &				  p_name,
								const std::vector<Vertex> &		  p_vertices,
								const std::vector<unsigned int> & p_indices,
								const Material &				  p_material ) :
		_name( p_name ),
		_vertices( p_vertices ), _indices( p_indices ), _material( p_material )
	{
		_vertices.shrink_to_fit();
		_indices.shrink_to_fit();
		_setupGL();
	}

	void TriangleMesh::render( const GLuint p_glProgram ) const
	{
		glBindVertexArray( _vao ); 

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		glProgramUniform3fv( p_glProgram, glGetUniformLocation( p_glProgram, "ambient" ), 1, glm::value_ptr(_material._ambient) );
		glProgramUniform3fv( p_glProgram, glGetUniformLocation( p_glProgram, "diff" ), 1, glm::value_ptr(_material._diffuse) );
		glProgramUniform3fv( p_glProgram, glGetUniformLocation( p_glProgram, "spec" ), 1, glm::value_ptr(_material._specular) );
		glProgramUniform1f( p_glProgram, glGetUniformLocation( p_glProgram, "shininess" ), _material._shininess );
		glProgramUniform3fv( p_glProgram, glGetUniformLocation( p_glProgram, "LightPos" ), 1, glm::value_ptr(Vec3f(0.f, 15.f, 0.f)) );
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		glProgramUniform1i( p_glProgram, glGetUniformLocation( p_glProgram, "uHasDiffuseMap" ), _material._hasDiffuseMap );
		glProgramUniform1i( p_glProgram, glGetUniformLocation( p_glProgram, "uHasAmbientMap" ), _material._hasAmbientMap );
		glProgramUniform1i( p_glProgram, glGetUniformLocation( p_glProgram, "uHasShininess" ), _material._hasShininessMap );
		glProgramUniform1i( p_glProgram, glGetUniformLocation( p_glProgram, "uHasSpecularMap" ), _material._hasSpecularMap );
     /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
     ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		if (_material._hasDiffuseMap) {
			glBindTextureUnit( 1, _material._diffuseMap._id );
		}
		 if ( _material._hasAmbientMap )
		{
			glBindTextureUnit( 2, _material._ambientMap._id );
		}
		
		if (_material._hasSpecularMap) {
			glBindTextureUnit( 3, _material._specularMap._id );
		}
     ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

		glDrawElements( GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0 );
		
	 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
     /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		glBindTextureUnit( 0, _material._diffuseMap._id );
		glBindTextureUnit( 0, _material._ambientMap._id );
		glBindTextureUnit( 0, _material._shininessMap._id );
		glBindTextureUnit( 0, _material._specularMap._id );
     //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		glBindVertexArray( 0 ); 
	}

	void TriangleMesh::cleanGL()
	{
		glDisableVertexArrayAttrib( _vao, 0 );
		glDisableVertexArrayAttrib( _vao, 1 );
		glDisableVertexArrayAttrib( _vao, 2 );
		glDisableVertexArrayAttrib( _vao, 3 );
		glDisableVertexArrayAttrib( _vao, 4 );
		glDeleteVertexArrays( 1, &_vao );
		glDeleteBuffers( 1, &_vbo );
		glDeleteBuffers( 1, &_ebo );
		
	}

	void TriangleMesh::_setupGL()
	{		
		 glCreateBuffers( 1, &_vbo );
		glNamedBufferData( _vbo, sizeof( Vertex ) * _vertices.size(), _vertices.data(), GL_STATIC_DRAW );

		glCreateBuffers( 1, &_ebo );
		glNamedBufferData( _ebo, sizeof( unsigned int ) * _indices.size(), _indices.data(), GL_STATIC_DRAW );
		glCreateVertexArrays( 1, &_vao );
		glVertexArrayElementBuffer( _vao, _ebo );

		//____________Enable my attributes_________________________
		glEnableVertexArrayAttrib( _vao, 0 );
		glEnableVertexArrayAttrib( _vao, 1 );
		glEnableVertexArrayAttrib( _vao, 2 );
		glEnableVertexArrayAttrib( _vao, 3 );
		glEnableVertexArrayAttrib( _vao, 4 );
		//________Binding the vbo to be read by vao______________________
		glVertexArrayVertexBuffer( _vao, 0, _vbo, 0, sizeof( Vertex ) );
		//_______________set up The format of my attributes______________
		glVertexArrayAttribFormat( _vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof( Vertex, _position ) );
		glVertexArrayAttribFormat( _vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof( Vertex, _normal ) );
		glVertexArrayAttribFormat( _vao, 2, 2, GL_FLOAT, GL_FALSE, offsetof( Vertex, _texCoords ) );
		glVertexArrayAttribFormat( _vao, 3, 3, GL_FLOAT, GL_FALSE, offsetof( Vertex, _tangent ) );
		glVertexArrayAttribFormat( _vao, 4, 3, GL_FLOAT, GL_FALSE, offsetof( Vertex, _bitangent ) );

		//__________make all attributes read from the same buffer_______________________
		glVertexArrayAttribBinding( _vao, 0, 0 );
		glVertexArrayAttribBinding( _vao, 1, 0 );
		glVertexArrayAttribBinding( _vao, 2, 0 );
		glVertexArrayAttribBinding( _vao, 3, 0 );
		glVertexArrayAttribBinding( _vao, 4, 0 );
		glBindTextureUnit( 1, _material._diffuseMap._id );


		
	}
} // namespace M3D_ISICG
