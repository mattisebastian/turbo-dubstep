# include "flappy_box/view/box_gl_drawable.hpp"

#include <GL/freeglut.h>

# include <algorithm>

using namespace ::flappy_box::view;


BoxGlDrawable::BoxGlDrawable(const std::shared_ptr< ::flappy_box::model::Box >& b )
    : _model( b )
{

}

BoxGlDrawable::~BoxGlDrawable()
{

}

void BoxGlDrawable::visualize( ::view::GlRenderer& r, ::view::GlutWindow& w )
{

    // TODO: Replace old rendering code with new and improved rendering - Aufgabe 5.3

    float vertices[] = {

        0.5, -0.5, -0.5,
        -0.5, -0.5, -0.5,
        -0.5,  -0.5,  0.5,
        0.5,  -0.5,  0.5,

        -0.5,  -0.5,  0.5,
        0.5,  -0.5,  0.5,
        0.5,  0.5,  0.5,
        -0.5,  0.5,  0.5,

        0.5,  0.5,  0.5,
        -0.5,  0.5,  0.5,
        -0.5,  0.5,  -0.5,
        0.5,  0.5,  -0.5,

        -0.5,  0.5,  -0.5,
        0.5,  0.5,  -0.5,
        0.5, -0.5, -0.5,
        -0.5, -0.5, -0.5,

        0.5, -0.5, -0.5,
        0.5, -0.5, 0.5,
        0.5, 0.5, 0.5,
        0.5, 0.5, -0.5,

        -0.5, -0.5, -0.5,
        -0.5, -0.5, 0.5,
        -0.5, 0.5, 0.5,
        -0.5, 0.5, -0.5

    };

    float normals[] = {

        0, -1, 0,
        0, -1, 0,
        0, -1, 0,
        0, -1, 0,

        0, 0, 1,
        0, 0, 1,
        0, 0, 1,
        0, 0, 1,

        0, 1, 0,
        0, 1, 0,
        0, 1, 0,
        0, 1, 0,

        0, 0, -1,
        0, 0, -1,
        0, 0, -1,
        0, 0, -1,

        1, 0, 0,
        1, 0, 0,
        1, 0, 0,
        1, 0, 0,

        -1, 0, 0,
        -1, 0, 0,
        -1, 0, 0,
        -1, 0, 0


    };
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    //glEnable(GL_CULL_FACE);
    
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
    
    // Positionierung der Box
    double angle = _model->angle();
    const vec3_type& pos = _model->position();
    const double size = _model->size();
    glTranslated( pos[0], pos[1], pos[2] );
    glRotated( angle, 0., 1., 0. );
    glScaled( size, size, size );

    glColor3f(1.0, 1.0, 1.0);
    glDrawArrays(GL_QUADS, 0, 24);
    
    // Light
    
    GLfloat light_pos[] = {
    
	1.0 ,-1.0 ,1.0 ,1.0
	
    };
    
    
   glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
   glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
   glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
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
