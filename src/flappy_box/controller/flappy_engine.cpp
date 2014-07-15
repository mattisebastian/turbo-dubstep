#include "flappy_box/controller/flappy_engine.hpp"

// model
#include "flappy_box/model/box.hpp"
#include "flappy_box/model/paddle.hpp"
#include "flappy_box/model/world.hpp"
#include "flappy_box/model/game_over.hpp"
// controller
#include "flappy_box/controller/box_object_logic.hpp"
#include "flappy_box/controller/paddle_logic.hpp"
#include "flappy_box/controller/world_logic.hpp"
#include "flappy_box/controller/game_over_logic.hpp"
// view
#include "flappy_box/view/box_gl_drawable.hpp"
#include "flappy_box/view/box_al_audible.hpp"
#include "flappy_box/view/paddle_gl_drawable.hpp"
#include "flappy_box/view/world_gl_drawable.hpp"

#include "view/glut_window.hpp"

# include <AL/alut.h>
# include <GL/freeglut.h>



using namespace ::flappy_box::controller;

static std::function< void () > __current_glut_advance_func = [](){ std::cerr << "Warning: Default function called in __current_glut_advance_func." << std::endl; };

FlappyEngine::FlappyEngine( const std::shared_ptr< ::controller::Logic >& l )
: GlutEngine( l )
, _al_renderer( std::make_shared< ::view::AlRenderer >( game_model() ) )
, _gl_renderer( std::make_shared< ::view::GlRenderer >( game_model() ) )
{}

void FlappyEngine::init( int& argc, char** argv )
{
  GlutEngine::init( argc, argv );

  alutInit( &argc, argv );

  // register the delegate classes for Box 
  game_logic() ->   logic_factory().register_module< flappy_box::model::Box >( []( std::shared_ptr< flappy_box::model::Box > const& b ) { return std::make_shared< BoxObjectLogic >     ( b ); } );
  al_renderer()-> audible_factory().register_module< flappy_box::model::Box >( []( std::shared_ptr< flappy_box::model::Box > const& b ) { return std::make_shared< view::BoxAlAudible > ( b ); } );
  gl_renderer()->drawable_factory().register_module< flappy_box::model::Box >( []( std::shared_ptr< flappy_box::model::Box > const& b ) { return std::make_shared< view::BoxGlDrawable >( b ); } );
  
  // TODO: Register all the other delegate classes
  
  // register delegate classes for Paddle
  game_logic() ->   logic_factory().register_module<flappy_box::model::Paddle>([](std::shared_ptr<flappy_box::model::Paddle> const &p) { return std::make_shared< PaddleLogic >           (p); });
  // al_renderer()-> audible_factory().register_module<flappy_box::model::Paddle>([](std::shared_ptr<flappy_box::model::Paddle> const &p) { return std::make_shared< view::PaddleAlAudible> (p); });
  gl_renderer()->drawable_factory().register_module<flappy_box::model::Paddle>([](std::shared_ptr<flappy_box::model::Paddle> const &p) { return std::make_shared< flappy_box::view::PaddleGlDrawable>(p); });

  // register delegate classes for World
  game_logic()->logic_factory().register_module<flappy_box::model::World>([](std::shared_ptr<flappy_box::model::World> const &p) { return std::make_shared< WorldLogic >(p); });
  //al_renderer()-> audible_factory().register_module<flappy_box::model::World>([](std::shared_ptr<flappy_box::model::World> const &p) { return std::make_shared< view::PaddleAlAudible> (p); });
  gl_renderer()->drawable_factory().register_module<flappy_box::model::World>([](std::shared_ptr<flappy_box::model::World> const &p) { return std::make_shared< flappy_box::view::WorldGlDrawable>(p); });

  // register delegate classes for Game Over
  game_logic()->logic_factory().register_module<flappy_box::model::GameOver>([](std::shared_ptr<flappy_box::model::GameOver> const &p) { return std::make_shared< GameOverLogic >(p); });
  // al_renderer()-> audible_factory().register_module<flappy_box::model::Paddle>([](std::shared_ptr<flappy_box::model::Paddle> const &p) { return std::make_shared< view::PaddleAlAudible> (p); });
  //gl_renderer()->drawable_factory().register_module<flappy_box::model::GameOver>([](std::shared_ptr<flappy_box::model::GameOver> const &p) { return std::make_shared< flappy_box::view::GameOverGlDrawable>(p); });

  
  // create one single cube (to be deleted later...)
  std::shared_ptr< flappy_box::model::Box > box = std::make_shared< flappy_box::model::Box >("Box"); 
  box->setSize(10.0);
  box->setAngle(0);
  game_model()->addGameObject( box );
  
  std::shared_ptr< ::flappy_box::model::Paddle > player_paddle = std::make_shared< ::flappy_box::model::Paddle >("Player Paddle");
  player_paddle->setSize(vec3_type(10.0, 1.0, 2.5));
  //player_paddle->setPosition(vec3_type(0.0, 0.0, -_model->getWorldHalfHeight() + user_paddle->size()[2] * 2.0));
  //player_paddle->setMaxPosition(vec3_type(_model->getWorldHalfWidth() - user_paddle->size()[0] * 0.5, 0.0, _model->getWorldHalfHeight()));
  game_model()->addGameObject(player_paddle);
  // TODO: Aufgabe 3.4 create and add a world object here
  //...
  
}


void FlappyEngine::run()
{
  // Create a window and connect it with a view::GlRenderer and an InputEventHandler.
  auto window = std::make_shared< ::view::GlutWindow >( "flappy_box", 1280, 720, gl_renderer() , shared_from_this() );

  // run game
  GlutEngine::run();

  alutExit();
}

void FlappyEngine::step( ::controller::InputEventHandler::keyboard_event const& ev )
{
  ::controller::GlutEngine::step( ev );
  al_renderer()->auralize_model();
  glutPostRedisplay();
}

