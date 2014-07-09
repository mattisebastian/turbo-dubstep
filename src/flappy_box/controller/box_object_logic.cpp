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
    
    vec3_type p_alt = _model->position();
    vec3_type v_alt = _model->velocity();
    vec3_type a_alt = _model->acceleration();
    vec3_type a_grav(0,0,-1.5);
    vec3_type f_ext = _model->externalForce();
    double d = 0.8;
    double size = _model->size();
    /*TODO: Volumen besser ausrechnen? */ double mass = size*size*size;
    vec3_type a_ext = f_ext / mass;

    //wanted: a_neu, v_neu, p_neu
    vec3_type a_neu = a_alt * d + a_ext + a_grav;

    // explicit. Euler-Method
    vec3_type dv = a_neu * timestep_sec;
    vec3_type v_neu = v_alt + dv;

    vec3_type dp = v_neu * timestep_sec;
    vec3_type p_neu = p_alt + dp;

    // check for correct position value
    
    //std::cout << p_neu <<  std::endl;
    
    //std::cout << ev.key << " " << ev.key_state << std::endl;
    
    // links + rechts
    if (std::abs(p_neu[0]) > _model->maxPosition()[0]) {
        p_neu[0] =_model->maxPosition()[0];
        // box at side end of the world
	v_neu[0] *= -1;
	v_neu *= 0.8;
    }
    // oben + unten
    if (std::abs(p_neu[2]) > _model->maxPosition()[2]) {
	if(p_neu[2] < 0) {
	    p_neu[2] = -1 *_model->maxPosition()[2];
	}else{
	    p_neu[2] = _model->maxPosition()[2];
	}
	// box at top or bottom end of the world
	v_neu[2] *= -1;
	v_neu *= 0.8;
    }
    
    _model->setAcceleration(a_neu);
    _model->setVelocity(v_neu);
    _model->setPosition(p_neu);
        
    return false;
    }
