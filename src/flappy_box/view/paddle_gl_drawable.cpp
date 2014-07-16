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

	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	//glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	
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

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	// Licht
	GLfloat light_pos[] = { 1.0, -1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, new float[] { .5, .5, .5, 0});
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glDisable(GL_LIGHTING);

	glPopMatrix();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_NORMALIZE);
	glDisable(GL_COLOR_MATERIAL);
	//glDisable(GL_CULL_FACE);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);


}

void PaddleGlDrawable::updateVBOs()
{
	// ring_seg1 ... Außenradius (großer Radius)
	// ring_seg2 ... Radius Kreissegmente (kleiner Radius)
	double r0 = std::max(_model->size()[0], _model->size()[2]) / 2;
	double r1 = std::min(_model->size()[1] / 2, r0);

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
			
			// Normalen-Daten speichern
			ring_normals[index] = cosf(beta) * cosf(alpha);
			ring_normals[index + 1] = cosf(beta) * sinf(alpha);
			ring_normals[index + 2] = sinf(beta);

			// Vertex-Daten speichern
			ring_vertices[index] = r0 * cosf(alpha) + r1 * ring_normals[index];
			ring_vertices[index + 1] = r0 * sinf(alpha) + r1 * ring_normals[index + 1];
			ring_vertices[index + 2] = r1 * ring_normals[index + 2];
			
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