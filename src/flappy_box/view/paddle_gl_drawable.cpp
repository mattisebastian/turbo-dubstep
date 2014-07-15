# include "flappy_box/view/paddle_gl_drawable.hpp"

# include <algorithm>

using namespace ::flappy_box::view;


PaddleGlDrawable::PaddleGlDrawable(const std::shared_ptr< ::flappy_box::model::Paddle >& b )
: _model( b )
{
	std::cout << "PaddleGlDrawable" << std::endl;

	glGenBuffers(3, this->ring_vbuf);
	updateVBOs();
}

PaddleGlDrawable::~PaddleGlDrawable()
{
	glDeleteBuffers(3, this->ring_vbuf);
}

void PaddleGlDrawable::visualize( ::view::GlRenderer& r, ::view::GlutWindow& w )
{
	//if (_model->size() != _size) updateVBOs();

	vec3_type pos = _model->position();
	//float ang = _model->angle();

	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	
	glPushMatrix();
	glTranslated(pos[0], pos[1], pos[2]);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glColor3f(.6f, .9f, .7f);
	glBindBuffer(GL_ARRAY_BUFFER, this->ring_vbuf[0]);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, this->ring_vbuf[1]);
	glNormalPointer(GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, this->ring_vbuf[2]);
	glDrawElements(GL_TRIANGLES, ring_seg1 * ring_seg2 * 6, GL_UNSIGNED_INT, NULL);

	//glDisable(GL_LIGHTING);

	glPopMatrix();

	/*
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	*/
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	
	/*float l_pos[] = {
	float(pos[0]),
	float(pos[1] - 1 * 1.5),
	float(pos[2] + 1 * 3),
	1
	};
	float l_amb[] = { 0, 0, 0, 1 };
	float l_dif[] = { 1, 1, 1, 1 };
	glLightfv(GL_LIGHT0, GL_POSITION, l_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, l_amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, l_dif);
	*/

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
			
			int index = (i * ring_seg2 * 3) + j * 3;
			
			// Vertex-Daten speichern
			ring_vertices[index] = static_cast<float>(bigradius * cos(alpha) + smallradius*(cos(beta)*cos(alpha)));
			ring_vertices[index + 1] = static_cast<float>(bigradius * sin(alpha) + smallradius*(cos(beta)*sin(alpha)));
			ring_vertices[index + 2] = static_cast<float>(smallradius * sin(beta));

			// Normalen-Daten speichern
			ring_normals[index] = static_cast<float>(cos(beta) * cos(alpha));
			ring_normals[index + 1] = static_cast<float>(cos(beta) * sin(alpha));
			ring_normals[index + 2] = static_cast<float>(sin(beta));

			// Indizes speichern
			index = ring_seg2 * i * 6;

			ring_indices[index] = (j + r);
			ring_indices[index + 1] = (j + 15 + r) % 600;
			ring_indices[index + 2] = ((j + 1) % 15) + r;
			ring_indices[index + 3] = ((j + 1) % 15) + r;
			ring_indices[index + 4] = (j + 15 + r) % 600;
			ring_indices[index + 5] = (((j + 1) % 15) + 15 + r) % 600;

			/* print indices
			for (int u = 0; u < 6; u++)
				std::cout << ring_indices[index + u] << "\t";
			std::cout << std::endl;*/
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, ring_vbuf[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ring_vertices), ring_vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, ring_vbuf[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ring_normals), ring_normals, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ring_vbuf[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ring_indices), ring_indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}