# include "flappy_box/controller/box_object_logic.hpp"
# include <AL/alut.h>
# include <thread>


# include <GL/freeglut.h>

using namespace ::flappy_box::controller;

BoxObjectLogic::BoxObjectLogic(const std::shared_ptr< flappy_box::model::Box >& b )
: ::controller::Logic::ObjectLogic()
, _model( b )
{}

bool BoxObjectLogic::advance( ::controller::Logic& l, ::controller::InputEventHandler::keyboard_event const& ev )
{
  
  double timestep_sec = l.game_model()->timestep().count();
  vec3_type p_alt;
  vec3_type v_alt;
  vec3_type a_alt;
  vec3_type a_grav(0,0,-1,5);
  vec3_type f_ext = _model->externalForce();
  double d = 0.8;
  double size = _model->size();
  /*TODO: Volumen besser ausrechnen? */ double mass = size*size*size;
  vec3_type a_ext = f_ext / mass;
  
  //gesucht: a_neu, v_neu, p_neu
  
  // lt. Aufgabenstellung
  vec3_type a_neu = a_alt * d + a_ext + a_grav;
  
  
  
  return false;
}