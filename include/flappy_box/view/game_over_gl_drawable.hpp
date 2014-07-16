#pragma once

#include "view/gl_renderer.hpp"

#include "flappy_box/model/game_over.hpp"

#include <GL/freeglut.h>

namespace flappy_box {
namespace view {
class GameOverGlDrawable : public ::view::GlRenderer::Drawable {
public:

	GameOverGlDrawable(const std::shared_ptr< model::GameOver>& );
	virtual ~GameOverGlDrawable();
	virtual void visualize(::view::GlRenderer&, ::view::GlutWindow& ) override;

private:
	std::shared_ptr<model::GameOver const> _model;
};
}
}