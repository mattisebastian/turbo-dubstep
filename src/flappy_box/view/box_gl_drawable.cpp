# include "flappy_box/view/box_gl_drawable.hpp"

#include <GL/freeglut.h>

# include <algorithm>

using namespace ::flappy_box::view;


BoxGlDrawable::BoxGlDrawable(const std::shared_ptr< ::flappy_box::model::Box >& b )
    : _model( b )
{
    //float pixels[];
    char _data [128][128][3];
    for(int i = 0; i <= 128; i++)
    {
        for(int j = 0; j <= 128; j++)
        {
            _data[i][j][0] = 127;
            _data[i][j][1] = 255;
            _data[i][j][2] = 0;
        }
    }
    
      glTexImage2D(
        GL_TEXTURE_2D,
        0, // not sure here
        GL_RGB,
        128,
        128,
        0,
        GL_RGB,
        GL_BYTE,
        _data
    );
    // 5.3.3 Textur
    
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    
    }


BoxGlDrawable::~BoxGlDrawable()
{
    glDeleteTextures(1, &texture_id);

}

void BoxGlDrawable::visualize(::view::GlRenderer& r, ::view::GlutWindow& w)
{
    // Ich habe die Arrays nach außen verschoben als static member.

    // TODO: Replace old rendering code with new and improved rendering - Aufgabe 5.3

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    // 5.3.3 Textur
    
    static const float texture [] = {
    0, 0, 0,
    1, 1, 1,
    1, 0, 0,
    0, 0, 1,
    
    1, 1, 1,
    0, 0, 0,
    0, 1, 1,
    1, 1, 0,
    
    0, 0, 0,
    1, 1, 1,
    1, 0, 0,
    0, 0, 1,
    
    1, 1, 1,
    0, 0, 0,
    0, 1, 1,
    1, 1, 0,
};
    
    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(
        3,
        GL_FLOAT,
        0,
        texture
    );
  

    glEnable(GL_CULL_FACE);

    static const float vertices[] = {


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

static const float  normals[] = {

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
        glRotated(angle, 0., 1., 0);
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
    
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);


}





