# include "flappy_box/controller/world_logic.hpp"
#include "flappy_box/model/box.hpp""
#include <flappy_box/model/paddle.hpp>
#include <flappy_box/model/game_over.hpp>
# include <AL/alut.h>
# include <thread>
# include <chrono>
#include <cmath>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

# include <GL/freeglut.h>

using namespace ::flappy_box::controller;
using namespace  std::chrono;

WorldLogic::WorldLogic(const std::shared_ptr< flappy_box::model::World >& w )
    : ::controller::Logic::ObjectLogic()
    , _model( w )
    , _shallRestartTheGame (true)
{}

void WorldLogic::addBoxToGame( ::controller::Logic& l )
{
	double x = rand() % static_cast<int>(2 * _model->getWorldHalfWidth()) - _model->getWorldHalfWidth();
	float size = rand() % 4 + 2;

	std::shared_ptr< ::flappy_box::model::Box > new_box = std::make_shared< ::flappy_box::model::Box >();
	new_box->setPosition(vec3_type(x, 0, _model->getWorldHalfHeight()));
	new_box->setMaxPosition(vec3_type(_model->getWorldHalfWidth()-(size/2), _model->getWorldHalfHeight()-(size/2), 1.));

	new_box->setSize(size);

    l.game_model()->addGameObject(new_box);
    
}

void WorldLogic::setForce(std::shared_ptr< flappy_box::model::Box > & box, std::shared_ptr< flappy_box::model::Paddle > & paddle)
{
	vec3_type bpos = box->position();
	vec3_type psize = paddle->size();

	// 1. Fall
	if ((bpos[0] > -0.5*psize[0]) && (bpos[0] < +0.5*psize[0]))
	{
		box->setExternalForce(vec3_type(0, 0, 1)*(10 * box->size() * box->size()));
	}
	else
	{

	}
}

void WorldLogic::restartGame(::controller::Logic& l)
{
	// invalidate all game objects
	for (auto o : l.game_model()->objects())
	{
		o->setAlive(false);
	}
	// reject invalidation for world object
	_model->setAlive(true);
	_model->setPlayerPoints(0);
	_model->setRemainingLives(5);
	// create and configure new paddle object
	std::shared_ptr< flappy_box::model::Paddle > user_paddle = std::make_shared< flappy_box::model::Paddle >("PlayerPaddle");
	user_paddle->setSize(vec3_type(10.0, 1.0, 2.5));
	user_paddle->setPosition(vec3_type(0.0, 0.0, -_model->getWorldHalfHeight() + user_paddle->size()[2] * 2.0));
	user_paddle->setMaxPosition(vec3_type(_model->getWorldHalfWidth() - user_paddle->size()[0] * 0.5, 0.0, _model->getWorldHalfHeight()));
	// add paddle object
	l.game_model()->addGameObject(user_paddle);
	// unset restart flag
	_shallRestartTheGame = false;
}

bool WorldLogic::advance( ::controller::Logic& l, ::controller::InputEventHandler::keyboard_event const& ev )
{

    if(_shallRestartTheGame) 
		restartGame(l);
    
    static steady_clock::time_point now = steady_clock::now();
    
    if((duration_cast<duration<double>>(steady_clock::now() - now)).count() > 5.0  ){
		now = steady_clock::now();
		addBoxToGame( l );
    }
    
	const auto objects = l.game_model()->objects();
	
	// groﬂes R‰tsel bei std::shared_ptr< ::model::GameObject >go  ODER & go
    auto it = std::find_if(objects.begin(), objects.end(), 
		[](std::shared_ptr< ::model::GameObject >go) { return typeid(*go) == typeid(::flappy_box::model::Paddle); })
    ;
    
    std::shared_ptr< ::flappy_box::model::Paddle > player_paddle(nullptr);
    
	if (it != objects.end())
	{
		player_paddle = std::dynamic_pointer_cast<::flappy_box::model::Paddle>(*it);
	}
    
    _model->setPlayerPoints(_model->playerPoints() + 5);
    
    if (_model->lives() <= 0)
	{
		_model->setAlive(false);
		l.game_model()->addGameObject( std::make_shared< ::flappy_box::model::GameOver >( _model->playerPoints() ) );
	}    

	for (auto go : objects)
	{
		std::shared_ptr<::flappy_box::model::Box> bo = std::dynamic_pointer_cast<::flappy_box::model::Box>(go);

		if (typeid(*go) != typeid(::flappy_box::model::Box))
			continue;

		for (auto co : objects)
		{
			// Kollision mit sich selbst verhindern
			if (go == co)
				continue;

			if (typeid(*co) != typeid(::flappy_box::model::Box))
				continue;

			if (go->isAlive() && co->isAlive())
			{
				std::shared_ptr<::flappy_box::model::Box> cobo = std::dynamic_pointer_cast<::flappy_box::model::Box>(co);

				if (sqrt((cobo->position()[0] - bo->position()[0]) * (cobo->position()[0] - bo->position()[0]) +
					(cobo->position()[1] - bo->position()[1]) * (cobo->position()[1] - bo->position()[1])) < ((bo->size() + cobo->size()) / 2))
				{
					bo->setAlive(false);
					cobo->setAlive(false);
				}

			}
		}

		if (!bo || !player_paddle)
			continue;

		setForce(bo, player_paddle);

		if (bo->position()[2] < player_paddle->position()[2])
		{
			bo->setAlive(false);
			_model->setPlayerPoints(_model->playerPoints() - 1);
		}
	}

    return false;
}
