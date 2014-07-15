# pragma once

# include "view/gl_renderer.hpp"

# include "flappy_box/model/box.hpp"
#include "GL/glew.h"
#include "GL/freeglut.h"

namespace flappy_box
{
namespace view
{
class BoxGlDrawable : public ::view::GlRenderer::Drawable
{
public:

    BoxGlDrawable( const std::shared_ptr< model::Box >& );
    virtual ~BoxGlDrawable();
    virtual void visualize( ::view::GlRenderer&, ::view::GlutWindow& ) override;

private:
    std::shared_ptr< model::Box const > _model;

    static float vertices[72];
    static float normals[72];
    GLuint _texture;
    
    


}; // BoxGlDrawable
}
}
