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

void Player::drop_weapon(ISceneNode* cam)
{
	if (cam = dynamic_cast<ICameraSceneNode*>(cam))
	{
		cam->removeChild(weapon->get_weapon_node());
		weapon->set_weapon_node(NULL);
	}
}

void Player::pick_weapon(ISceneNode* cam, IAnimatedMeshSceneNode* w)
{
	if (no_weapon() && (cam = dynamic_cast<ICameraSceneNode*>(cam)))
	{
		weapon->set_weapon_node(w);
		weapon->set_weapon_node( scene_manager->addAnimatedMeshSceneNode(weapon->get_weapon_node()->getMesh(), cam, -1));  //this is the important line where you make "gun" child of the camera so it moves when the camera moves
	
		weapon->get_weapon_node()->setScale(core::vector3df(0.008,0.008,0.008));
		weapon->get_weapon_node()->setPosition(core::vector3df(15,-10,20)); 
		weapon->get_weapon_node()->setRotation(core::vector3df(0,50,90));		
	}
}