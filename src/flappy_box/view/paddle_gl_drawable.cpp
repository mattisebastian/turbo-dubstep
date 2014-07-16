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
	GLfloat light_intens[] = { .5, .5, .5, 0};
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_intens);
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

/*
# include "flappy_box/view/paddle_gl_drawable.hpp"

using namespace ::flappy_box::view;

PaddleGlDrawable::PaddleGlDrawable(const std::shared_ptr< ::flappy_box::model::Paddle >& b)
: _model(b),
_s0(40),
_s1(15),
_blades(9),
_angle(0),
_len(20),
_vortex_speed(100)
{
glGenBuffers(3, torusVBO);
glGenBuffers(3, bladeVBO);
glGenBuffers(4, streamVBO);

for (unsigned int i = 0; i < 40; i++)
{
GLfloat a = static_cast<GLfloat>(0.25 * sin(M_PI * static_cast<double>(i) / static_cast<double>(_len - 1.0)) * sin(M_PI * static_cast<double>(i) / static_cast<double>(_len - 1)));
_stream_color[i * 8] = 0.6;
_stream_color[i * 8 + 1] = 0.6;
_stream_color[i * 8 + 2] = 0.6;
_stream_color[i * 8 + 3] = a;
_stream_color[i * 8 + 4] = 0.6;
_stream_color[i * 8 + 5] = 0.6;
_stream_color[i * 8 + 6] = 0.6;
_stream_color[i * 8 + 7] = a;
}

glBindBuffer(GL_ARRAY_BUFFER, streamVBO[0]);
glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* 120, NULL, GL_DYNAMIC_DRAW);

glBindBuffer(GL_ARRAY_BUFFER, streamVBO[1]);
glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* 120, NULL, GL_DYNAMIC_DRAW);

glBindBuffer(GL_ARRAY_BUFFER, streamVBO[2]);
glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* 120, NULL, GL_DYNAMIC_DRAW);

glBindBuffer(GL_ARRAY_BUFFER, streamVBO[3]);
glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* 320, _stream_color, GL_STATIC_DRAW);

for (unsigned int i = 0; i < 3; i++)
{
for (unsigned int j = 0; j < 20; j++)
{
vortex_dat[i][j][0] = _model->position();
vortex_dat[i][j][1] = _model->position();
}
}

}


PaddleGlDrawable::~PaddleGlDrawable()
{
glDeleteBuffers(3, torusVBO);
glDeleteBuffers(3, bladeVBO);
glDeleteBuffers(4, streamVBO);
}

void PaddleGlDrawable::updateVBOs(char type)
{
switch (type)
{
case 't':
{
GLfloat vertices[1800]; // 40 * 15 * 3
GLfloat normals[1800];
GLuint indices[3600]; // 40 * 15 * 3 * 2

for (unsigned int i = 0; i < _s0; i++)
{
double a = 2.0 * M_PI * static_cast<double>(i) / static_cast<double>(_s0);
for (unsigned int k = 0; k < _s1; k++)
{
double b = 2.0 * M_PI * static_cast<double>(k) / static_cast<double>(_s1);
unsigned int idx[3] = { i * _s1 * 3 + k * 3,
i * _s1 * 3 + k * 3 + 1,
i * _s1 * 3 + k * 3 + 2 };

vec3_type n = cos(b) * vec3_type(cos(a), sin(a), 0.0) + sin(b) * vec3_type(0.0, 0.0, 1.0);
vec3_type v = vec3_type(_r0 * cos(a), _r0 * sin(a), 0.0) + _r1 * n;

vertices[idx[0]] = static_cast<GLfloat>(v.x());
vertices[idx[1]] = static_cast<GLfloat>(v.y());
vertices[idx[2]] = static_cast<GLfloat>(v.z());

normals[idx[0]] = static_cast<GLfloat>(n.x());
normals[idx[1]] = static_cast<GLfloat>(n.y());
normals[idx[2]] = static_cast<GLfloat>(n.z());
}
}

for (unsigned int i = 0; i < _s0; i++)
{
for (unsigned int k = 0; k < _s1; k++)
{
unsigned int idx = i * _s1 * 3 * 2 + k * 3 * 2;

indices[idx] = i * _s1 + k;
indices[idx + 1] = (indices[idx] + 15) % 600;
indices[idx + 2] = i * _s1 + ((k + 1) % _s1);

indices[idx + 3] = indices[idx + 2];
indices[idx + 4] = indices[idx + 1];
indices[idx + 5] = (indices[idx + 2] + 15) % 600;
}
}

glBindBuffer(GL_ARRAY_BUFFER, torusVBO[0]);
glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* 1800, vertices, GL_STATIC_DRAW);

glBindBuffer(GL_ARRAY_BUFFER, torusVBO[1]);
glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* 1800, normals, GL_STATIC_DRAW);

glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, torusVBO[2]);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)* 3600, indices, GL_STATIC_DRAW);

break;
}

case 'b':
{
GLfloat vertices[81]; // 3 * 3 * 9
GLfloat normals[81];
GLuint indices[81];

for (unsigned int i = 0; i < _blades; i++)
{
double a1 = 2.0 * M_PI * (static_cast<double>(i)+0.1) / static_cast<double>(_blades),
a2 = 2.0 * M_PI * (static_cast<double>(i)-0.1) / static_cast<double>(_blades);
unsigned int idx[9] = { i * 9,		// xyz(3) für 3(3) Vertices pro blade(i)
i * 9 + 1,
i * 9 + 2,
i * 9 + 3,
i * 9 + 4,
i * 9 + 5,
i * 9 + 6,
i * 9 + 7,
i * 9 + 8 };

vertices[idx[0]] = 0.0;
vertices[idx[1]] = 0.0;
vertices[idx[2]] = 0.0;

vertices[idx[3]] = static_cast<GLfloat>((_r0 - _r1) * cos(a1));
vertices[idx[4]] = static_cast<GLfloat>((_r0 - _r1) * sin(a1));
vertices[idx[5]] = static_cast<GLfloat>(_r1 * 0.5);

vertices[idx[6]] = static_cast<GLfloat>((_r0 - _r1) * cos(a2));
vertices[idx[7]] = static_cast<GLfloat>((_r0 - _r1) * sin(a2));
vertices[idx[8]] = static_cast<GLfloat>(-_r1 * 0.5);

normals[idx[0]] = vertices[idx[4]] * vertices[idx[8]] - vertices[idx[5]] * vertices[idx[7]];
normals[idx[1]] = vertices[idx[5]] * vertices[idx[6]] - vertices[idx[3]] * vertices[idx[8]];
normals[idx[2]] = vertices[idx[3]] * vertices[idx[7]] - vertices[idx[4]] * vertices[idx[6]];

normals[idx[3]] = normals[idx[0]];
normals[idx[4]] = normals[idx[1]];
normals[idx[5]] = normals[idx[2]];

normals[idx[6]] = normals[idx[0]];
normals[idx[7]] = normals[idx[1]];
normals[idx[8]] = normals[idx[2]];

//std::cout << "(" << normals[idx[6]] << ", " << normals[idx[7]] << ", " << normals[idx[8]] << ")" << std::endl;

for (unsigned int k = 0; k < 9; k++)
{
indices[idx[k]] = idx[k];
}
}

glBindBuffer(GL_ARRAY_BUFFER, bladeVBO[0]);
glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* 81, vertices, GL_STATIC_DRAW);

glBindBuffer(GL_ARRAY_BUFFER, bladeVBO[1]);
glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* 81, normals, GL_STATIC_DRAW);

glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bladeVBO[2]);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)* 81, indices, GL_STATIC_DRAW);

break;
}
}

}

void PaddleGlDrawable::visualize(::view::GlRenderer& r, ::view::GlutWindow& w)
{
double timestep_sec = r.game_model()->timestep().count();

_angle += static_cast<GLfloat>(540.0 * timestep_sec);
if (_angle > 720) _angle -= 720;

glPushMatrix();
{

if (_r0 == NULL
|| _r1 == NULL
|| _r0 != static_cast<GLfloat>(_model->size()[0] / 2.0) && _model->size()[0] > _model->size()[1]
|| _r0 != static_cast<GLfloat>(_model->size()[1] / 2.0) && _model->size()[1] >= _model->size()[0]
|| _r1 != static_cast<GLfloat>(_model->size()[2] / 5.0) //|| _r1 != _r0 && _model->size()[2] / 2.0 < _r0 || _r1 != _model->size()[2] / 2.0 && _model->size()[2] / 2.0 >= _r0
)
{
if (_model->size()[0] > _model->size()[1]) _r0 = static_cast<GLfloat>(_model->size()[0] / 2.0);
else _r0 = static_cast<GLfloat>(_model->size()[1] / 2.0);
if (_model->size()[2] / 2.0 < _r0) _r1 = static_cast<GLfloat>(_model->size()[2] / 5.0); //_r0;
else _r1 = static_cast<GLfloat>(_model->size()[2] / 5.0);

updateVBOs('t');
updateVBOs('b');
}

for (unsigned int l = 0; l < 3; l++)
{
GLfloat a = static_cast<GLfloat>((_angle / 180.0 * M_PI) + (2.0 * M_PI * static_cast<GLfloat>(l) / 3.0)); // (Gradmaß->Bogenmaß) + (2 * PI * l/3)

for (unsigned int t = _len - 1; t > 0; --t)
{
vortex_dat[l][t][0] = vortex_dat[l][t - 1][0] + vec3_type(0.0, 0.0, timestep_sec * _vortex_speed);
vortex_dat[l][t][1] = vortex_dat[l][t][0] + (vortex_dat[l][t - 1][1] - vortex_dat[l][t - 1][0]) * 1.075;
}
vortex_dat[l][0][0] = _model->position();
vortex_dat[l][0][1] = vec3_type(_model->position().x() + (_r0 - _r1) * cos(a), _model->position().y() + (_r0 - _r1) * sin(a), _model->position().z() + _r1);
glBindBuffer(GL_ARRAY_BUFFER, streamVBO[l]);
float * dat = static_cast<float*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
for (unsigned int t = 0; t < _len; t++)
{
vec3_type p = vortex_dat[l][t][1];
p[0] -= 0.5 * 1.0;
*(dat++) = static_cast<float>(p[0]);
*(dat++) = static_cast<float>(p[1]);
*(dat++) = static_cast<float>(p[2]);
p[0] += .5;
*(dat++) = static_cast<float>(p[0]);
*(dat++) = static_cast<float>(p[1]);
*(dat++) = static_cast<float>(p[2]);
}
glUnmapBuffer(GL_ARRAY_BUFFER);
}

//depth
glEnable(GL_DEPTH_TEST);
glDepthMask(GL_TRUE);
glDepthFunc(GL_LEQUAL);
glDepthRange(0.0f, 1.0f);

//moving
vec3_type pos(_model->position());
glTranslated(pos[0], pos[1], pos[2]);

//lighting
//glRotated(_angle * 0.25, 0, 0, 1);

glEnable(GL_LIGHTING);
glEnable(GL_LIGHT0);
glEnable(GL_COLOR_MATERIAL);
glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
GLfloat light_pos[] = { 0.0, -10.0, 10.0, 1.0 };
glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
GLfloat light_ambient[4] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat light_diffuse[4] = { 1.0, 1.0, 1.0, 1.0 };
glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

//glRotated(-_angle * 0.25, 0, 0, 1);

glEnableClientState(GL_VERTEX_ARRAY);
glEnableClientState(GL_NORMAL_ARRAY);

//torus
glColor3f(0.75f, 0.75f, 0.75f);

glBindBuffer(GL_ARRAY_BUFFER, torusVBO[0]);
glVertexPointer(3, GL_FLOAT, 0, NULL);
glBindBuffer(GL_ARRAY_BUFFER, torusVBO[1]);
glNormalPointer(GL_FLOAT, 0, NULL);
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, torusVBO[2]);
glDrawElements(GL_TRIANGLES, _s0 * _s1 * 6, GL_UNSIGNED_INT, NULL);

//paddle
glRotated(_angle, 0, 0, 1);
glColor3f(0.55f, 0.55f, 0.55f);

glBindBuffer(GL_ARRAY_BUFFER, bladeVBO[0]);
glVertexPointer(3, GL_FLOAT, 0, NULL);
glBindBuffer(GL_ARRAY_BUFFER, bladeVBO[1]);
glNormalPointer(GL_FLOAT, 0, NULL);
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bladeVBO[2]);
glDrawElements(GL_TRIANGLES, 81, GL_UNSIGNED_INT, NULL);

glDisableClientState(GL_NORMAL_ARRAY);

glDisable(GL_COLOR_MATERIAL);
glDisable(GL_LIGHT0);
glDisable(GL_LIGHTING);

//undo rotated & translation
glRotated(-_angle, 0, 0, 1);
glTranslated(-pos[0], -pos[1], -pos[2]);

//blend
glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

glEnableClientState(GL_COLOR_ARRAY);

//stream
glBindBuffer(GL_ARRAY_BUFFER, streamVBO[3]);
glColorPointer(4, GL_FLOAT, 0, NULL);

glBindBuffer(GL_ARRAY_BUFFER, streamVBO[0]);
glVertexPointer(3, GL_FLOAT, 0, NULL);
glDrawArrays(GL_TRIANGLE_STRIP, 0, 40);

glBindBuffer(GL_ARRAY_BUFFER, streamVBO[1]);
glVertexPointer(3, GL_FLOAT, 0, NULL);
glDrawArrays(GL_TRIANGLE_STRIP, 0, 40);

glBindBuffer(GL_ARRAY_BUFFER, streamVBO[2]);
glVertexPointer(3, GL_FLOAT, 0, NULL);
glDrawArrays(GL_TRIANGLE_STRIP, 0, 40);

glBindBuffer(GL_ARRAY_BUFFER, 0); //reset buffer

glDisableClientState(GL_COLOR_ARRAY);
glDisableClientState(GL_VERTEX_ARRAY);

glDisable(GL_BLEND);
glDisable(GL_DEPTH_TEST);

}
glPopMatrix();


}

*/