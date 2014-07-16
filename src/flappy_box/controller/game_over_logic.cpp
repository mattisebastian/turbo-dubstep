#include "flappy_box/controller/game_over_logic.hpp"
#include "flappy_box/model/world.hpp"


using namespace ::flappy_box::controller;
GameOverLogic::GameOverLogic(const std::shared_ptr< model::GameOver >& model)
: _model(model) {
}

bool GameOverLogic::advance( ::controller::Logic& l, ::controller::InputEventHandler::keyboard_event const& ev ) {
	if (ev.key == 114) {
	    std::cout << "R pressed! " << std::endl;
		_model->setAlive(false);

		auto world = std::dynamic_pointer_cast< flappy_box::model::World >(*std::find_if(
				l.game_model()->objects().begin(), l.game_model()->objects().end(),
				[](std::shared_ptr< ::model::GameObject> const& p) { return p->name() == "World"; }));
		world->setShallRestartTheGame(true);

	}

	return false;
}