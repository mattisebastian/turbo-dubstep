#include "flappy_box/view/game_over_gl_drawable.hpp"
#include <view/glut_window.hpp>
#include <GL/freeglut.h>

using namespace ::flappy_box::view;

GameOverGlDrawable::GameOverGlDrawable(const std::shared_ptr< ::flappy_box::model::GameOver >& o)
: _model(o) {
}

GameOverGlDrawable::~GameOverGlDrawable() {
}

void GameOverGlDrawable::visualize(::view::GlRenderer& r, ::view::GlutWindow& win) {
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);
	glLineWidth(2);
	glColor3f(1, 1, 1);

// 	glMatrixMode(GL_PROJECTION);
// 	glPushMatrix();
// 	glLoadIdentity();
// 	glOrtho(-100, 100, -100, 100, -1, 1);
// 	glMatrixMode(GL_MODELVIEW);
// 	glPushMatrix();
// // 
// // 
// // 	glLoadIdentity();
// // 	glTranslatef(-55, 0, 0);
// 	glScalef(0.15, 0.15, 1);
// 	glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*) "GAME OVER!");
// 
// 
// 	glPopMatrix();
// 	glMatrixMode(GL_PROJECTION);
// 	glPopMatrix();
// 	glMatrixMode(GL_MODELVIEW);






	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(win.width() * -0.5,  win.width() * 0.5, win.height() * -0.5, win.height() * 0.5, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();


	glLoadIdentity();
	glTranslatef(-100, -100, 0);
	glScalef(0.15, 0.15, 1);
	glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*) "GAME OVER! \n press [R] to play again");


	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);





	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}