# include "flappy_box/view/paddle_gl_drawable.hpp"

#include <GL/glew.h>
#include <GL/freeglut.h>


# include <algorithm>

using namespace ::flappy_box::view;


PaddleGlDrawable::PaddleGlDrawable(const std::shared_ptr< ::flappy_box::model::Paddle >& b )
: _model( b )
{
	updateVBOs();
	GLuint bu[3];
	glGenBuffers(3, bu);
	//glVertexPointer();
	//glBindBuffer(GL_ARRAY_BUFFER, bu);
}

PaddleGlDrawable::~PaddleGlDrawable()
{

}

void PaddleGlDrawable::visualize( ::view::GlRenderer& r, ::view::GlutWindow& w )
{
	/*
	::glEnableClientState(GL_VERTEX_ARRAY);
	::glEnableClientState(GL_NORMAL_ARRAY);
	::glColor3f(.6f, .9f, .7f);
	::glBindBuffer(GL_ARRAY_BUFFER, this->ring_vbuf[0]);
	::glVertexPointer(3, GL_FLOAT, 0, NULL);
	::glBindBuffer(GL_ARRAY_BUFFER, this->ring_vbuf[1]);
	::glNormalPointer(GL_FLOAT, 0, NULL);
	::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, this->ring_vbuf[2]);
	::glDrawElements(GL_TRIANGLES, ring_seg1 * ring_seg2 * 6, GL_UNSIGNED_INT, NULL);
   */
}

void PaddleGlDrawable::updateVBOs()
{
	bigradius = std::max(_model->size()[0], _model->size()[2]) / 2;
	smallradius = std::min(_model->size()[1] / 2, bigradius);

	double alpha = 0;
	double beta = 0;

	for (int i = 0; i < bigradius_partitions; i++)
	{
		alpha = 2.0f * M_PI * static_cast<double>(i) / static_cast<double>(bigradius_partitions);

		// Basisindex pro Ring (0, 15, 30, ... 585)
		int r = smallradius_partitions * i;

		for (int j = 0; j < smallradius_partitions; j++)
		{
			beta = 2.0f * M_PI * static_cast<double>(j) / static_cast<double>(smallradius_partitions);
			int index = (i*bigradius_partitions + j*smallradius_partitions) * 3;
			
			// Vertex-Daten speichern
			vertices[index] = bigradius * cos(alpha) + smallradius*(cos(beta)*cos(alpha));
			vertices[index+1] = bigradius * sin(alpha) + smallradius*(cos(beta)*sin(alpha));
			vertices[index+2] = smallradius * sin(beta);

			// Normalen-Daten speichern
			normals[index] = cos(beta) * cos(alpha);
			normals[index+1] = cos(beta) * sin(alpha);
			normals[index+2] = sin(beta);

			// Indizes speichern
			index = smallradius_partitions * i * 6;

			indices[index] = (j) + r;
			indices[index + 1] = (j + 15) + r % 600;
			indices[index + 2] = (j + 1) % 15 + r;
			indices[index + 3] = (j + 1) % 15 + r;
			indices[index + 4] = (j + 15) + r % 600;
			indices[index + 5] = (j + 16) + r % 600;
		}
	}
	for (int i = 0; i < bigradius_partitions; i++)
	{
		for (int j = 0; j < smallradius_partitions; j++)
		{
			int index = i*bigradius_partitions + j*smallradius_partitions;
			indices[index] = i;
			indices[index + 1] = i + 15;
			indices[i + 2] = i + 1;
			indices[i + 3] = i + 1;
			indices[i + 4] = i + 15;
			indices[i + 5] = i + 16;
		}
	}

}