#include "Player.h"


Player::Player(ISceneManager *sm): Character(knight_path, sm)
{
}

Player::Player(ISceneManager *sm, Weapon* w): Character(knight_path, sm, w)
{
}


Player::~Player(void)
{
}

void Player::manage_collision(Weapon *w)
{
	if(!w->get_collision_flag())
	{
		if(this->detect_collision(w->get_weapon_node(), this->head))
		{
			w->set_collision_flag(true);
		}
		else if(this->detect_collision(w->get_weapon_node(), this->body))
		{
			w->set_collision_flag(true);
		}
		else if(this->detect_collision(w->get_weapon_node(), this->body))
		{
			w->set_collision_flag(true);
		}
	}
}