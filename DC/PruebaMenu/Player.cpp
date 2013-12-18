#include "Player.h"
#include <string.h>
using namespace std;

Player::Player(ISceneManager *sm, ITriangleSelector* ms, ICameraSceneNode *camera): Character(knight_path, sm)
{
	mapSelector = ms;
	cam = camera;
}

Player::Player(ISceneManager *sm, Weapon* w, ITriangleSelector* ms,ICameraSceneNode *camera): Character(knight_path, sm, w)
{
		mapSelector = ms;
		cam = camera;
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
		if(weapon->get_weapon_node() && weapon->get_weapon_node()->getAnimators().empty())
		{
			cam->removeChild(weapon->get_weapon_node());
			weapon->set_weapon_node(NULL);
			pick_shield();
		}
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
			pick_shield();
		}
		else if(atoi(w->getName()) ==  BOW_TYPE)
		{
			weapon = new Bow(0,0,scene_manager, mapSelector, device);
			weapon->add_to_camera(core::vector3df(15,-10,20), core::vector3df(0,-90,0), core::vector3df(0.02,0.02,0.02), cam);
			drop_shield();
		}	
		else if(atoi(w->getName()) ==  RED_SHROOM_TYPE)
		{
			weapon = new ThrowableItem(scene_manager, mapSelector, device, ThrowableItem::RED_SHROOM);
			weapon->add_to_camera(core::vector3df(15,-10,20), core::vector3df(0,-90,0), core::vector3df(0.02,0.02,0.02), cam);
			pick_shield();
		}	
		else if(atoi(w->getName()) == SPEAR_TYPE)
		{
			weapon = new Spear(0,0,scene_manager);
			weapon -> add_to_camera(core::vector3df(10,-30,20), core::vector3df(-140,0,0), core::vector3df(1.4,1.4,1.4), cam);
			drop_shield();
		}
		else if(atoi(w->getName()) ==  BLUE_SHROOM_TYPE)
		{
			weapon = new ThrowableItem(scene_manager, mapSelector, device, ThrowableItem::BLUE_SHROOM);
			weapon->add_to_camera(core::vector3df(15,-10,20), core::vector3df(0,-90,0), core::vector3df(0.02,0.02,0.02), cam);
			pick_shield();
		}	
		else if(atoi(w->getName()) ==  YELLOW_SHROOM_TYPE)
		{
			weapon = new ThrowableItem(scene_manager, mapSelector, device, ThrowableItem::YELLOW_SHROOM);
			weapon->add_to_camera(core::vector3df(15,-10,20), core::vector3df(0,-90,0), core::vector3df(0.02,0.02,0.02), cam);
			pick_shield();
		}
		else if(atoi(w->getName()) ==  STONE_TYPE)
		{
			weapon = new ThrowableItem(scene_manager, mapSelector, device, ThrowableItem::STONE);
			weapon->add_to_camera(core::vector3df(15,-10,20), core::vector3df(0,-90,0), core::vector3df(0.05,0.05,0.05), cam);
			pick_shield();
		}
		else if(atoi(w->getName()) ==  TORCH_TYPE)
		{
			weapon = new ThrowableItem(scene_manager, mapSelector, device, ThrowableItem::TORCH);
			weapon->add_to_camera(core::vector3df(15,-10,20), core::vector3df(0,-90,0), core::vector3df(1,1,1), cam);
			pick_shield();
		}
	}
}

void Player::drop_shield()
{
	try
	{
		if(sh->get_weapon_node())
		{
			cam->removeChild(sh->get_weapon_node());
			sh->set_weapon_node(NULL);
		}
	}
	catch(exception ex)
	{
	}
}

void Player::pick_shield()
{
	try
	{
		if(!sh->get_weapon_node())
		{
			sh->add_to_camera(vector3df(-5,-5,5), vector3df(0,0,0), vector3df(3,3,3), cam);
		}
	}
	catch(exception ex)
	{
	}
}

void Player::defend()
{
	try
	{
		if((weapon->get_weapon_node() && !weapon->is_animated()) || !weapon->get_weapon_node())
		{
			sh->attack(0,0,0,0);
		}
	}
	catch(exception ex)
	{
	}
}

void Player::no_defend()
{
	try
	{
		if((weapon->get_weapon_node() && !weapon->is_animated()) || !weapon->get_weapon_node())
		{
			sh->finish_animation();
		}
	}
	catch(exception ex)
	{
	}
}