#include "flappy_box/model/game_over.hpp"

using namespace flappy_box::model;

GameOver::GameOver(const std::string& n, const int p)
: GameObject( true, n )
, _playerPoints( p )
{

}