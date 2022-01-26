#ifndef __LAB_WORK_2_H__
#define __LAB_WORK_2_H__

#include "GL/gl3w.h"
#include "common/base_lab_work.hpp"
#include "define.hpp"
#include <vector>

namespace M3D_ISICG
{
	class LabWork2 : public BaseLabWork
	{
	  public:
		LabWork2() : BaseLabWork() {}
		~LabWork2();

		bool init() override;
		void animate( const float p_deltaTime ) override;
		void render() override;

		void handleEvents( const SDL_Event & p_event ) override;
		void displayUI() override;

		// mes fonctions
		bool creationProgramme();

	  private:
		// ================ Scene data.
		std::vector<Vec2f> vertices_triangle;
		std::vector<int>   sommets_triangles;
		float			   uTranslationX = 0.5;
		float			   luminosity	 = 1.f;
		GLuint			   location_uTranslationX;
		// ================

		// ================ GL data.
		GLuint _program;
		GLuint _vbo, _vboColor, _vao;
		GLuint _ebo;
		time_t _time = 0;
		// ================

		// ================ Settings.
		Vec4f _bgColor = Vec4f( 0.8f, 0.8f, 0.8f, 1.f ); // Background color
		// ================

		static const std::string _shaderFolder;
	};
} // namespace M3D_ISICG

#endif // __LAB_WORK_2_H__
