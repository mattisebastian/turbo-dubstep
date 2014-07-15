# include "flappy_box/view/paddle_gl_drawable.hpp"

# include <algorithm>

using namespace ::flappy_box::view;


PaddleGlDrawable::PaddleGlDrawable(const std::shared_ptr< ::flappy_box::model::Paddle >& b )
: _model( b )
{
	updateVBOs();

	glGenBuffers(3, this->ring_vbuf);
	glBindBuffer(GL_ARRAY_BUFFER, *ring_vbuf);
	glVertexPointer(3, GL_FLOAT, 3, NULL);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
}

PaddleGlDrawable::~PaddleGlDrawable()
{

}

void PaddleGlDrawable::visualize( ::view::GlRenderer& r, ::view::GlutWindow& w )
{
	::glEnableClientState(GL_VERTEX_ARRAY);
	::glEnableClientState(GL_NORMAL_ARRAY);
	::glColor3f(.6f, .9f, .7f);
	::glBindBuffer(GL_ARRAY_BUFFER, this->ring_vbuf[0]);
	::glVertexPointer(3, GL_FLOAT, 0, NULL);
	::glBindBuffer(GL_ARRAY_BUFFER, this->ring_vbuf[1]);
	::glNormalPointer(GL_FLOAT, 0, NULL);
	::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, this->ring_vbuf[2]);
	::glDrawElements(GL_TRIANGLES, ring_seg1 * ring_seg2 * 6, GL_UNSIGNED_INT, NULL);
}

void PaddleGlDrawable::updateVBOs()
{
	// ring_seg1 ... Außenradius (großer Radius)
	// ring_seg2 ... Radius Kreissegmente (kleiner Radius)
	double bigradius = std::max(_model->size()[0], _model->size()[2]) / 2;
	double smallradius = std::min(_model->size()[1] / 2, bigradius);

	double alpha = 0;
	double beta = 0;

	for (int i = 0; i < ring_seg1; i++)
	{
		alpha = 2.0f * M_PI * static_cast<double>(i) / static_cast<double>(ring_seg1);

		// Basisindex pro Ring (0, 15, 30, ... 585)
		int r = ring_seg2 * i;

		for (int j = 0; j < ring_seg2; j++)
		{
			beta = 2.0f * M_PI * static_cast<double>(j) / static_cast<double>(ring_seg2);
			int index = (i*ring_seg1 + j*ring_seg2) * 3;
			
			// Vertex-Daten speichern
			ring_vertices[index] = bigradius * cos(alpha) + smallradius*(cos(beta)*cos(alpha));
			ring_vertices[index + 1] = bigradius * sin(alpha) + smallradius*(cos(beta)*sin(alpha));
			ring_vertices[index + 2] = smallradius * sin(beta);

			// Normalen-Daten speichern
			ring_normals[index] = cos(beta) * cos(alpha);
			ring_normals[index + 1] = cos(beta) * sin(alpha);
			ring_normals[index + 2] = sin(beta);

			// Indizes speichern
			index = ring_seg2 * i * 6;

			ring_indices[index] = (j + r);
			ring_indices[index + 1] = (j + 15 + r) % 600;
			ring_indices[index + 2] = ((j + 1) % 15) + r;
			ring_indices[index + 3] = ((j + 1) % 15) + r;
			ring_indices[index + 4] = (j + 15 + r) % 600;
			ring_indices[index + 5] = (((j + 1) % 15) + 15 + r) % 600;
		}
	}

}