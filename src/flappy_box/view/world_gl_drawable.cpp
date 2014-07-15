# include "flappy_box/view/world_gl_drawable.hpp"

#include <GL/freeglut.h>

# include <algorithm>

using namespace ::flappy_box::view;


WorldGlDrawable::WorldGlDrawable(const std::shared_ptr< ::flappy_box::model::World >& b)
    : _model( b )
{
	//float pixels[];
}

WorldGlDrawable::~WorldGlDrawable()
{
	//glDeleteTextures();
}

void WorldGlDrawable::visualize(::view::GlRenderer& r, ::view::GlutWindow& w)
{
	
}