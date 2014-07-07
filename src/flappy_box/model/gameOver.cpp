# include "flappy_box/model/gameOver.hpp"

using namespace flappy_box::model;

GameOver::GameOver( const std::string& n, const int p )
: GameObject( true, n )
, _playerPoints( p )
{

}