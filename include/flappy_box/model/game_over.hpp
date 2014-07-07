# pragma once

# include "model/game_object.hpp"
# include "math.hpp"

// ATTENTION: NEW CODE!

namespace flappy_box
{

  namespace model
  {
    class GameOver : public ::model::GameObject
    {
      public:

        GameOver( const std::string& = "GameOver", const int p );

        const int playerPoints() const { return _playerPoints; }

    private:

        int  _playerPoints;

    }; // GameOver

  } // model::
} // flappy_box::