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
	glColor3d(0.25, 0.25, 0.25);
	glBegin(GL_QUADS);
	glVertex3d(_model->getWorldHalfWidth(), -100, _model->getWorldHalfHeight());
	glVertex3d(_model->getWorldHalfWidth(), 500, _model->getWorldHalfHeight());
	glVertex3d(_model->getWorldHalfWidth(), 500, -_model->getWorldHalfHeight());
	glVertex3d(_model->getWorldHalfWidth(), -100, -_model->getWorldHalfHeight());
	glEnd();

	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogf(GL_FOG_START, 0.0);
	glFogf(GL_FOG_END, 500);

	GLfloat fogColor[4] = { 1.0, 1.0, 1.0, 1.0 };
	glFogfv(GL_FOG_COLOR, fogColor);

}