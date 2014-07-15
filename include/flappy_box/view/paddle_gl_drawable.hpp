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

		double bigradius;
		double smallradius;
		
		static const unsigned int ring_seg1 = 40;
		static const unsigned int ring_seg2 = 15;

		float vertices[1800];
		float normals[1800];

		int indices[3600];

		GLuint ring_vbuf[3];

    }; // PaddleGlDrawable
  }
}
