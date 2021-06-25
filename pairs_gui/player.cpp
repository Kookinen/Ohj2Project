#include "player.hh"

Player::Player()
{

}

Player::Player(const QString& name): name_(name)
{

}

Player::~Player()
{

}

int Player::get_points()
{
    return points_;
}

QString Player::get_name()
{
    return name_;
}

void Player::add_points()
{
    points_ ++;
}
