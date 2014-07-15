# include "flappy_box/view/box_gl_drawable.hpp"

#include <GL/freeglut.h>

# include <algorithm>

using namespace ::flappy_box::view;


BoxGlDrawable::BoxGlDrawable(const std::shared_ptr< ::flappy_box::model::Box >& b )
    : _model( b )
{
	//float pixels[];
}

BoxGlDrawable::~BoxGlDrawable()
{
	//glDeleteTextures();
}

void BoxGlDrawable::visualize(::view::GlRenderer& r, ::view::GlutWindow& w)
{
	// Ich habe die Arrays nach außen verschoben als static member.

	// TODO: Replace old rendering code with new and improved rendering - Aufgabe 5.3

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnable(GL_CULL_FACE);

	glVertexPointer(
		3, // 3 components per vertex
		GL_FLOAT,
		0, // our data is not tightly packed
		vertices
		);

	glNormalPointer(
		GL_FLOAT,
		0,
		normals
		);

	glPushMatrix();
	{
		// Positionierung der Box
		double angle = _model->angle();
		const vec3_type& pos = _model->position();
		const double size = _model->size();
		glTranslated(pos[0], pos[1], pos[2]);
		glRotated(30, 0., 1., 0);
		glScaled(size, size, size);

		glColor3f(0.5, 0., 1.0);
		glDrawArrays(GL_QUADS, 0, 24);

		// 5.3.2
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(3);
		glColor3f(1., 1., 1.);
		glDrawArrays(GL_QUADS, 0, 24);
		// Zurückschalten
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	}
	//glPopMatrix();

    // Light
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    
    GLfloat light_pos[] = {-1.0, -1.0, 1.0, 1.0};
    
    float light_color[] = {1.0, 1.0, 1.0, 1.0};
   
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color );
    //glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 128);
   
	glPopMatrix();
//     


//   glPushMatrix();
//   {
//     double angle = _model->angle();
//     const vec3_type& pos = _model->position();
//     const double size = _model->size();
//     glTranslated( pos[0], pos[1], pos[2] );
//     glRotated( angle, 0., 1., 0. );
//     glScaled( size, size, size );
//
//     glColor3d( 0.25, 0.25, 0.25 );
//
//     ::glBegin(GL_LINES);
//
//     ::glVertex3d(-0.5, -0.5, -0.5);
//     ::glVertex3d(-0.5, -0.5,  0.5);
//     ::glVertex3d(-0.5,  0.5, -0.5);
//     ::glVertex3d(-0.5,  0.5,  0.5);
//     ::glVertex3d( 0.5, -0.5, -0.5);
//     ::glVertex3d( 0.5, -0.5,  0.5);
//     ::glVertex3d( 0.5,  0.5, -0.5);
//     ::glVertex3d( 0.5,  0.5,  0.5);
//
//     ::glVertex3d(-0.5, -0.5, -0.5);
//     ::glVertex3d(-0.5,  0.5, -0.5);
//     ::glVertex3d( 0.5, -0.5, -0.5);
//     ::glVertex3d( 0.5,  0.5, -0.5);
//     ::glVertex3d(-0.5, -0.5,  0.5);
//     ::glVertex3d(-0.5,  0.5,  0.5);
//     ::glVertex3d( 0.5, -0.5,  0.5);
//     ::glVertex3d( 0.5,  0.5,  0.5);
//
//     ::glVertex3d(-0.5, -0.5, -0.5);
//     ::glVertex3d( 0.5, -0.5, -0.5);
//     ::glVertex3d(-0.5, -0.5,  0.5);
//     ::glVertex3d( 0.5, -0.5,  0.5);
//     ::glVertex3d(-0.5,  0.5, -0.5);
//     ::glVertex3d( 0.5,  0.5, -0.5);
//     ::glVertex3d(-0.5,  0.5,  0.5);
//     ::glVertex3d( 0.5,  0.5,  0.5);
//
//     ::glEnd();
//
//   }
//   glPopMatrix();
}

float BoxGlDrawable::vertices[72] = {


	// front
	0.5, -0.5, -0.5,
	0.5, -0.5, 0.5,
	-0.5, -0.5, 0.5,
	-0.5, -0.5, -0.5,

	// top
	-0.5, -0.5, 0.5,
	0.5, -0.5, 0.5,
	0.5, 0.5, 0.5,
	-0.5, 0.5, 0.5,

	// back
	0.5, 0.5, 0.5,
	0.5, 0.5, -0.5,
	-0.5, 0.5, -0.5,
	-0.5, 0.5, 0.5,

	// bottom
	-0.5, 0.5, -0.5,
	0.5, 0.5, -0.5,
	0.5, -0.5, -0.5,
	-0.5, -0.5, -0.5,

	// right
	0.5, -0.5, -0.5,
	0.5, 0.5, -0.5,
	0.5, 0.5, 0.5,
	0.5, -0.5, 0.5,

	// left
	-0.5, -0.5, -0.5,
	-0.5, -0.5, 0.5,
	-0.5, 0.5, 0.5,
	-0.5, 0.5, -0.5

};

float BoxGlDrawable::normals[72] = {

	// front
	0, -1, 0,
	0, -1, 0,
	0, -1, 0,
	0, -1, 0,

	// top
	0, 0, 1,
	0, 0, 1,
	0, 0, 1,
	0, 0, 1,

	// top
	0, 1, 0,
	0, 1, 0,
	0, 1, 0,
	0, 1, 0,

	// back
	0, 0, -1,
	0, 0, -1,
	0, 0, -1,
	0, 0, -1,

	// right
	1, 0, 0,
	1, 0, 0,
	1, 0, 0,
	1, 0, 0,

	// left
	-1, 0, 0,
	-1, 0, 0,
	-1, 0, 0,
	-1, 0, 0


};