# include "flappy_box/model/paddle.hpp"

using namespace flappy_box::model;

Paddle::Paddle( const std::string& n )
: GameObject( true, n )
,      _position( 0.0, 0.0, 0.0 )
,   _maxPosition( 50.0, 0.0, 0.0 )
,      _velocity( 0.0, 0.0, 0.0 )
,  _acceleration( 1.0, 0.0, 0.0 )
,          _size( 4.0, 4.0, 2.0 )
, _playerControl( 0.0, 0.0, 0.0 )
{
	std::cout << "Paddle" << std::endl;
}