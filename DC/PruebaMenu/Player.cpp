#include "Player.h"
#include <string.h>
using namespace std;

Player::Player(ISceneManager *sm, ITriangleSelector* ms): Character(knight_path, sm)
{
	mapSelector = ms;
}

Player::Player(ISceneManager *sm, Weapon* w, ITriangleSelector* ms): Character(knight_path, sm, w)
{
		mapSelector = ms;
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

	try
	{
		cam->removeChild(weapon->get_weapon_node());
		weapon->set_weapon_node(NULL);
	}
	catch(exception ex)
	{
	}
	
}

void Player::pick_weapon(ISceneNode* cam, IAnimatedMeshSceneNode* w, 	IrrlichtDevice *device)
{
	if (no_weapon() && (cam = dynamic_cast<ICameraSceneNode*>(cam)))
	{
		if(atoi(w->getName()) == SWORD_TYPE)
		{
			weapon = new Sword(0,0,scene_manager);
			weapon -> add_to_camera(core::vector3df(15,-10,20), core::vector3df(0,50,90), core::vector3df(0.008,0.008,0.008), cam);
		}
		else if(atoi(w->getName()) ==  BOW_TYPE)
		{
			weapon = new Bow(0,0,scene_manager, mapSelector, device);
			weapon->add_to_camera(core::vector3df(0,-10,20), core::vector3df(0,-90,0), core::vector3df(0.02,0.02,0.02), cam);
		}	
		else if(atoi(w->getName()) ==  RED_SHROOM_TYPE)
		{
			weapon = new ThrowableItem(scene_manager, mapSelector, device, ThrowableItem::RED_SHROOM);
			weapon->add_to_camera(core::vector3df(0,-10,20), core::vector3df(0,-90,0), core::vector3df(0.02,0.02,0.02), cam);
		}	
	}
}