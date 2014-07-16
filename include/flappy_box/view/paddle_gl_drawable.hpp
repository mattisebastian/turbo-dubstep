# pragma once

# include "view/gl_renderer.hpp"

# include "flappy_box/model/paddle.hpp"
#include <GL/glew.h>
#include <GL/freeglut.h>

namespace flappy_box
{
	namespace view
	{
		class PaddleGlDrawable : public ::view::GlRenderer::Drawable
		{
		public:

			PaddleGlDrawable( const std::shared_ptr< model::Paddle >& );
			virtual ~PaddleGlDrawable();
			virtual void visualize( ::view::GlRenderer&, ::view::GlutWindow& ) override;

		private:
			std::shared_ptr< model::Paddle const > _model;

			// 5.4.1
			void updateVBOs();

			static const unsigned int ring_seg1 = 40;
			static const unsigned int ring_seg2 = 15;

			float ring_vertices[1800];
			float ring_normals[1800];
			int ring_indices[3600];

			GLuint ring_vbuf[3];

		}; // PaddleGlDrawable
	}
}

/*# pragma once

# include "view/gl_renderer.hpp"
# include "flappy_box/model/paddle.hpp"
# include "GL\glew.h"
# include "GL\freeglut.h"

namespace flappy_box
{
	namespace view
	{
		class PaddleGlDrawable : public ::view::GlRenderer::Drawable
		{
		public:

			PaddleGlDrawable(const std::shared_ptr< model::Paddle >&);
			virtual ~PaddleGlDrawable();
			virtual void visualize(::view::GlRenderer&, ::view::GlutWindow&) override;
			void updateVBOs(char type);


		private:

			std::shared_ptr< model::Paddle const > _model;
			const GLuint _s0, _s1, _blades, _len;
			GLuint torusVBO[3], bladeVBO[3], streamVBO[4];
			GLfloat _r0, _r1, _angle, _stream_color[4 * 2 * 40], _vortex_speed;
			vec3_type vortex_dat[3][20][2];

		}; // PaddleGlDrawable
	}
}
*/