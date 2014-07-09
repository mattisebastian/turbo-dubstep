# pragma once

# include "controller/logic.hpp"
# include "flappy_box/model/world.hpp"

namespace flappy_box
{
  namespace controller
  {

    class WorldLogic : public ::controller::Logic::ObjectLogic
    {
      public:
        WorldLogic() = delete;
        WorldLogic( const std::shared_ptr< model::World >& );
	

        virtual bool advance( ::controller::Logic&, ::controller::InputEventHandler::keyboard_event const & ) override;

      private:
        std::shared_ptr< model::World > _model;
	bool _shallRestartTheGame;
	void addBoxToGame( ::controller::Logic& l );
	void setForce( std::shared_ptr< flappy_box::model::Box > & box, std::shared_ptr<
	    flappy_box::model::Paddle > & paddle );
	void restartGame( ::controller::Logic& l );
	
    };

  }

} // audiophile