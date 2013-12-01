#include "Weapon.h"


Weapon::Weapon(const char* path, int dmg = 0, int sp = 0, ISceneManager *sm = 0)
{
	this->damage = dmg;
	this->speed = sp;
	this->scene_manager = sm;

	weapon_mesh = sm->getMesh(path); 
	weapon_mesh->setMaterialFlag(video::EMF_LIGHTING, false);
}

void Weapon::add_to_scene(vector3df position, vector3df rotation, vector3df scale, bool pickable)
{
	weapon_node = scene_manager->addAnimatedMeshSceneNode(weapon_mesh,0, IDFlag_IsPickable);
	if(weapon_node)
	{
		weapon_node->setScale(scale);
		weapon_node->setRotation(rotation);
		weapon_node->setPosition(position);
		ITriangleSelector* selector;
		selector = scene_manager->createTriangleSelector(weapon_node);
		weapon_node->setTriangleSelector(selector);
		selector->drop();
	}
}

void Weapon::add_to_camera(vector3df position, vector3df rotation, vector3df scale, ICameraSceneNode* camera)
{
	weapon_node = scene_manager->addAnimatedMeshSceneNode(weapon_mesh, camera, ID_IsNotPickable);  //this is the important line where you make "gun" child of the camera so it moves when the camera moves
	if(weapon_node)
	{
		weapon_node->setScale(scale);
		weapon_node->setPosition(core::vector3df(15,-10,20)); 
		weapon_node->setRotation(rotation);
	}
}

bool Weapon::get_collision_flag()
{
	return collision_flag;
}

void Weapon::set_collision_flag(bool cf)
{
	collision_flag = cf;
}

IAnimatedMeshSceneNode* Weapon::get_weapon_node()
{
	return weapon_node;
}

void Weapon::set_weapon_node(IAnimatedMeshSceneNode* wn)
{
	weapon_node = wn;
}

Weapon::~Weapon(void)
{
}

void Weapon::attack(float first_x, float first_y, float last_x, float last_y)
{
}

void Weapon::finish_animation()
{
}

