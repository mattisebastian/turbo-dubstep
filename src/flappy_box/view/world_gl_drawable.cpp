#include "flappy_box/view/world_gl_drawable.hpp"
#include "view/glut_window.hpp"

#include <GL/freeglut.h>

# include <algorithm>

using namespace ::flappy_box::view;


WorldGlDrawable::WorldGlDrawable(const std::shared_ptr< ::flappy_box::model::World >& b)
    : _model( b )
{
	glGenTextures(1, &_texture);
	glBindTexture(GL_TEXTURE_2D, _texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	static const unsigned char raw[] {
		130, 130, 130, 255, 255, 255, 0, 0,
			255, 255, 255, 130, 130, 130, 0, 0,
	};

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, raw);
}

WorldGlDrawable::~WorldGlDrawable()
{
	glDeleteTextures(1, &_texture);
}

void WorldGlDrawable::visualize(::view::GlRenderer& r, ::view::GlutWindow& win)
{
	float w = _model->getWorldHalfWidth();
	float h = _model->getWorldHalfHeight();
	float dx = 2;
	float dy = 2;
	float dz = 2;
	float s = 0.25;

	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _texture);

	glBegin(GL_QUADS);

	glNormal3f(0, 0, 1);
	for (float x = -w; x <= w; x += dx) {
		for (float y = -30; y <= 500; y += dy) {
			glTexCoord2f(x * s, y * s);
			glVertex3f(x, y, -h);
			glTexCoord2f((x + dx) * s, y * s);
			glVertex3f(x + dx, y, -h);
			glTexCoord2f((x + dx) * s, (y + dy) * s);
			glVertex3f(x + dx, y + dy, -h);
			glTexCoord2f(x * s, (y + dy) * s);
			glVertex3f(x, y + dy, -h);
		}
	}
	glNormal3f(0, 0, -1);
	for (float x = -w - dx; x < w + dx; x += dx) {
		for (float y = -30; y <= 500; y += dy) {
			glTexCoord2f(x * s, (y + dy) * s);
			glVertex3f(x, y + dy, h);
			glTexCoord2f((x + dx) * s, (y + dy) * s);
			glVertex3f(x + dx, y + dy, h);
			glTexCoord2f((x + dx) * s, y * s);
			glVertex3f(x + dx, y, h);
			glTexCoord2f(x * s, y * s);
			glVertex3f(x, y, h);
		}
	}
	glNormal3f(1, 0, 0);
	for (float z = -h; z < h; z += dz) {
		for (float y = -60; y <= 500; y += dy) {
			glTexCoord2f(z * s, y * s);
			glVertex3f(-w, y, z);
			glTexCoord2f(z * s, (y + dy) * s);
			glVertex3f(-w, y + dy, z);
			glTexCoord2f((z + dz) * s, (y + dy) * s);
			glVertex3f(-w, y + dy, z + dz);
			glTexCoord2f((z + dz) * s, y * s);
			glVertex3f(-w, y, z + dz);
		}
	}
	glNormal3f(-1, 0, 0);
	for (float z = -h; z < h; z += dz) {
		for (float y = -60; y <= 500; y += dy) {
			glTexCoord2f((z + dz) * s, y * s);
			glVertex3f(w, y, z + dz);
			glTexCoord2f((z + dz) * s, (y + dy) * s);
			glVertex3f(w, y + dy, z + dz);
			glTexCoord2f(z * s, (y + dy) * s);
			glVertex3f(w, y + dy, z);
			glTexCoord2f(z * s, y * s);
			glVertex3f(w, y, z);
		}
	}


	glEnd();

	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	// 5.5.1 glFog
	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogf(GL_FOG_DENSITY, 0.5);
	glFogf(GL_FOG_START, 0.0);
	glFogf(GL_FOG_END, 500);

	GLfloat fogColor[4] = { 1.0, 1.0, 1.0, 1.0 };
	glFogfv(GL_FOG_COLOR, fogColor);

	// 5.5.2 
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);
	glLineWidth(2);
	glColor3f(1, 1, 1);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, win.width(), 0, win.height(), -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();


	char line[32];
	glLoadIdentity();
	glTranslatef(10, 10, 0);
	glScalef(0.15, 0.15, 1);
	glutStrokeString(GLUT_STROKE_MONO_ROMAN, (unsigned char*) "Punkte:");

	glLoadIdentity();
	glTranslatef(70, 10, 0);
	glScalef(0.15, 0.15, 1);
	sprintf(line, "%6d", _model->playerPoints());
	glutStrokeString(GLUT_STROKE_MONO_ROMAN, (unsigned char*)line);

	glLoadIdentity();
	glTranslatef(10, 30, 0);
	glScalef(0.15, 0.15, 1);
	glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*) "Leben:");

	glLoadIdentity();
	glTranslatef(70, 30, 0);
	glScalef(0.15, 0.15, 1);
	sprintf(line, "%6d", _model->lives());
	glutStrokeString(GLUT_STROKE_MONO_ROMAN, (unsigned char*)line);

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	
	/*
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, win.width(), 0, win.height(), -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glColor3f(1, 1, 1);

	glRasterPos2d(100, 100);
	renderBitmapString("HALLO");

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	*/
}

void WorldGlDrawable::renderBitmapString(const char *string)
{
	for (const char *c = string; *c != '\0'; c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
	}
}