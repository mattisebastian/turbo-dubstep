# include "flappy_box/view/paddle_gl_drawable.hpp"

#include <GL/freeglut.h>

# include <algorithm>

using namespace ::flappy_box::view;


PaddleGlDrawable::PaddleGlDrawable(const std::shared_ptr< ::flappy_box::model::Paddle >& b )
: _model( b )
{
  
}

PaddleGlDrawable::~PaddleGlDrawable()
{

}

void PaddleGlDrawable::visualize( ::view::GlRenderer& r, ::view::GlutWindow& w )
{

   
}
