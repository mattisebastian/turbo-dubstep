# include "flappy_box/view/box_gl_drawable.hpp"

#include <GL/freeglut.h>

# include <algorithm>

using namespace ::flappy_box::view;


BoxGlDrawable::BoxGlDrawable(const std::shared_ptr< ::flappy_box::model::Box >& b )
: _model(b) {

 	glGenTextures(1, &_texture);
	glBindTexture(GL_TEXTURE_2D, _texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	FILE* f = std::fopen("../res/thehead.raw", "r");
	if (f) {
		const int size = 128 * 128 * 3;
		unsigned char raw[size];
		std::fread(raw, 1, size, f);
		std::fclose(f);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, 128, 128, 0, GL_RGB, GL_UNSIGNED_BYTE, raw);
	}
}

BoxGlDrawable::~BoxGlDrawable() {
 	glDeleteTextures(1, &_texture);
}

void BoxGlDrawable::visualize( ::view::GlRenderer& r, ::view::GlutWindow& w ) {

	static const float v[] = {
		 .5, .5,-.5, -.5, .5,-.5, -.5, .5, .5, .5, .5, .5,
		 .5,-.5, .5, -.5,-.5, .5, -.5,-.5,-.5, .5,-.5,-.5,
		 .5, .5, .5, -.5, .5, .5, -.5,-.5, .5, .5,-.5, .5,
		 .5,-.5,-.5, -.5,-.5,-.5, -.5, .5,-.5, .5, .5,-.5,
		 .5, .5,-.5, .5, .5, .5, .5,-.5, .5, .5,-.5,-.5,
		-.5, .5, .5, -.5, .5,-.5, -.5,-.5,-.5, -.5,-.5, .5,
	};
	static const float n[] = {
		 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0,
		 0,-1, 0, 0,-1, 0, 0,-1, 0, 0,-1, 0,
		 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1,
		 0, 0,-1, 0, 0,-1, 0, 0,-1, 0, 0,-1,
		 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0,
		-1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0,
	};
	static const float t[] = {
		0, 0, 0, 1, 1, 1, 1, 0,
		0, 0, 0, 1, 1, 1, 1, 0,
		0, 0, 0, 1, 1, 1, 1, 0,
		0, 0, 0, 1, 1, 1, 1, 0,
		0, 0, 0, 1, 1, 1, 1, 0,
		0, 0, 0, 1, 1, 1, 1, 0,
	};


	glVertexPointer(3, GL_FLOAT, 0, v);
	glNormalPointer(GL_FLOAT, 0, n);
	glTexCoordPointer(2, GL_FLOAT, 0, t);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_CULL_FACE);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _texture);

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	double angle = _model->angle();
	const vec3_type& pos = _model->position();
	const double size = _model->size();

	glPushMatrix();
	glTranslated(pos[0], pos[1], pos[2]);
	glRotated(angle, 0, 1, 0.);
	glRotated(angle * 2, 0.7, 0.4, 0.);
	glScaled(size, size, size);


	glColor3d(1, 1, 1);
	glDrawArrays(GL_QUADS, 0, 24);

	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glColor3d(0, 0.5, 0);
	glLineWidth(3);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_QUADS, 0, 24);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

}