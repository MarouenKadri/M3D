#include "lab_work_2.hpp"
#include "imgui.h"
#include "utils/read_file.hpp"
#include <chrono>
#include <iostream>
#include <time.h>

namespace M3D_ISICG
{
	//const std::string LabWork2::_shaderFolder = "src/lab_works/lab_work_2/shaders/";
	const std::string LabWork2::_shaderFolder = "C:/Users/alstm/source/repos/TP1+2/src/lab_works/lab_work_2/shaders/";

	// TP3 Q2.2 : ce n'est pas initScene() mais createCube() !
	LabWork2::~LabWork2()
	{
		glDeleteProgram( _program );
		glDeleteBuffers( 1, &_vbo );
		glDeleteBuffers( 1, &_vboColor );

		glDisableVertexArrayAttrib( _vao, 0 );
		glDeleteVertexArrays( sizeof( float ) * 3, &_vao );
		glDeleteVertexArrays( sizeof( float ) * 3, &_vbo );
	}

	bool LabWork2::init()
	{
		std::cout << "Initializing lab work 1..." << std::endl;
		std::cout << "Création du programme..." << std::endl;
		// Set the color used by glClear to clear the color buffer (in render()).
		glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );

		if ( !creationProgramme() )
		{
			std ::cerr << " Error création programme : " << std ::endl;
			return false;
		}
		std::cout << "Création du programme done !" << std::endl;

		std::cout << "Initialisation des données..." << std::endl;

		// création des points du triangle sur le CPU
		vertices_triangle = { Vec2f( -0.6, 0.6 ), Vec2f( 0.6, 0.6 ), Vec2f( 0.6, -0.6 ), Vec2f( -0.6, -0.6 ) };
		sommets_triangles = { 0, 1, 2, 0, 2, 3 };

		std::vector<float> colors = { 1.f, 0.f, 0.f,   // rouge
									  0.f, 1.f, 0.f,   // vert
									  0.f, 0.f, 1.f,   // bleu
									  1.f, 0.f, 1.f }; // magenta

		std::cout << " r: " << colors[ 0 ] << "\n";

		location_uTranslationX = glGetUniformLocation( _program, "uTranslationX" );
		glProgramUniform1f( _program, location_uTranslationX, uTranslationX );

		// création du VBO, buffers qui va contenir les points du triangle dans le GPU
		glCreateBuffers( 1, &_vbo );
		glNamedBufferData( _vbo, vertices_triangle.size() * sizeof( Vec2f ), vertices_triangle.data(), GL_STATIC_DRAW );

		// création de l'ebo (element buffer object)
		glCreateBuffers( 1, &_ebo );
		glNamedBufferData( _ebo, sommets_triangles.size() * sizeof( int ), sommets_triangles.data(), GL_STATIC_DRAW );

		// création du vboColor
		glCreateBuffers( 1, &_vboColor );
		glNamedBufferData( _vboColor, sizeof( float ) * 3 * 4, colors.data(), GL_STATIC_DRAW );

		// création du VAO
		GLuint indexVAO	  = 0;
		GLuint indexColor = 1;
		glCreateVertexArrays( 1, &_vao );
		glEnableVertexArrayAttrib( _vao, indexVAO );
		glEnableVertexArrayAttrib( _vao, indexColor );
		glVertexArrayAttribFormat( _vao, indexVAO, 2, GL_FLOAT, GL_FALSE, 0 ); // voir cours

		// liaison VAO avec le VBO
		glVertexArrayVertexBuffer( _vao, indexVAO, _vbo, 0, sizeof( Vec2f ) ); // voir cours
		glVertexArrayAttribBinding( _vao, 0, indexVAO );					   // voir cours

		// liaison VAO avec le EBO
		glVertexArrayElementBuffer( _vao, _ebo );

		// liaison vao avec vboColor
		glVertexArrayVertexBuffer( _vao, indexColor, _vboColor, 0, sizeof( float ) * 3 );
		glVertexArrayAttribBinding( _vao, 1, indexColor );

		std::cout << "Initialisation des données done!" << std::endl;

		glUseProgram( _program );

		std::cout << "Done!" << std::endl;

		return true;
	}

	void LabWork2::animate( const float p_deltaTime ) {
		//std::cout << " p_deltaTime: " << p_deltaTime << "\n";
		_time += p_deltaTime;
		uTranslationX = sin(_time * 0.00001) * 0.5;
		//std::cout << " uTranslationX: " << uTranslationX << "\n";
		glProgramUniform1f( _program, location_uTranslationX, uTranslationX );
	}

	void LabWork2::render()
	{
		auto clock_start = std::chrono::system_clock::now();
		glClear( GL_COLOR_BUFFER_BIT );
		glBindVertexArray( _vao );
		glDrawElements( GL_TRIANGLES, sommets_triangles.size(), GL_UNSIGNED_INT, 0 );
		glBindVertexArray( 0 );
		auto clock_end	 = std::chrono::system_clock::now();
		auto p_deltaTime = clock_end - clock_start;
		animate( p_deltaTime.count());
	}

	void LabWork2::handleEvents( const SDL_Event & p_event ) {}

	void LabWork2::displayUI()
	{
		ImGui::Begin( "Settings lab work 1" );
		//ImGui::Text( "No setting available!" );
		if ( ImGui::SliderFloat( "Luminosity", &luminosity, 10.f, 160.f, "%01.f" ) )
		{
			glProgramUniform1f( _program, glGetUniformLocation( _program, "uLumunosity" ), luminosity );
		}
		ImGui::End();
	}

	/*
	 *	MES FONCTIONS
	 */
	bool LabWork2::creationProgramme()
	{
		// lecture des fichiers des 2 shaders: Vertex Shader et Fragment Shader
		const std::string vertexShaderStr = readFile( _shaderFolder + "lw1.vert" );
		const std::string fragShaderStr	  = readFile( _shaderFolder + "lw1.frag" );
		const GLchar *	  vSrc			  = vertexShaderStr.c_str();
		const GLchar *	  fSrc			  = fragShaderStr.c_str();

		GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );
		GLuint fragShader	= glCreateShader( GL_FRAGMENT_SHADER );

		glShaderSource( vertexShader, 1, &vSrc, NULL );
		glShaderSource( fragShader, 1, &fSrc, NULL );

		/// ???
		glCompileShader( vertexShader );
		glCompileShader( fragShader );

		// Check if compilation is ok.
		GLint vert_compiled, frag_compiled;
		glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &vert_compiled );
		glGetShaderiv( fragShader, GL_COMPILE_STATUS, &frag_compiled );
		if ( !vert_compiled || !frag_compiled )
		{
			GLchar log[ 1024 ];
			if ( !vert_compiled )
			{
				glGetShaderInfoLog( vertexShader, sizeof( log ), NULL, log );
				std ::cerr << " Error compiling vertex shader : " << log << std ::endl;
			}
			if ( !frag_compiled )
			{
				glGetShaderInfoLog( fragShader, sizeof( log ), NULL, log );
				std ::cerr << " Error compiling fragment shader : " << log << std ::endl;
			}
			glDeleteShader( vertexShader );
			glDeleteShader( fragShader );
			return false;
		}

		_program = glCreateProgram();
		glAttachShader( _program, vertexShader );
		glAttachShader( _program, fragShader );

		glLinkProgram( _program );
		// Check if link is ok.
		GLint linked;
		glGetProgramiv( _program, GL_LINK_STATUS, &linked );
		if ( !linked )
		{
			GLchar log[ 1024 ];
			glGetProgramInfoLog( _program, sizeof( log ), NULL, log );
			std ::cerr << " Error linking program : " << log << std ::endl;
			return false;
		}

		glDeleteShader( vertexShader );
		glDeleteShader( fragShader );
	}

} // namespace M3D_ISICG
