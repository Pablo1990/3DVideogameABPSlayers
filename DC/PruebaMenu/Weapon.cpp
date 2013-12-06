#include "Weapon.h"

Weapon::Weapon(const char* path, int dmg = 0, int sp = 0, ISceneManager *sm = 0)
{
	this->damage = dmg;
	this->speed = sp;
	this->scene_manager = sm;

	this->weapon_mesh = sm->getMesh(path); 
	this->weapon_mesh->setMaterialFlag(video::EMF_LIGHTING, false);
}

void Weapon::add_to_scene(vector3df position, vector3df rotation, vector3df scale, bool pickable)
{
	this->weapon_node = scene_manager->addAnimatedMeshSceneNode(this->weapon_mesh, 0, IDFlag_IsPickable);
	if(this->weapon_node)
	{
		this->weapon_node->setScale(scale);
		this->weapon_node->setRotation(rotation);
		this->weapon_node->setPosition(position);

		ITriangleSelector* selector;
		selector = scene_manager->createTriangleSelector(this->weapon_node);
		this->weapon_node->setTriangleSelector(selector);
		selector->drop();
	}
}

void Weapon::add_to_camera(vector3df position, vector3df rotation, vector3df scale, ICameraSceneNode* camera)
{
	this->weapon_node = scene_manager->addAnimatedMeshSceneNode(this->weapon_mesh, camera, ID_IsNotPickable);  //this is the important line where you make "gun" child of the camera so it moves when the camera moves
	if(this->weapon_node)
	{
		this->weapon_node->setScale(scale);
		this->weapon_node->setPosition(core::vector3df(15,-10,20)); 
		this->weapon_node->setRotation(rotation);
	}
}

bool Weapon::get_collision_flag()
{
	return this->collision_flag;
}

void Weapon::set_collision_flag(bool cf)
{
	this->collision_flag = cf;
}

IAnimatedMeshSceneNode* Weapon::get_weapon_node()
{
	return this->weapon_node;
}

void Weapon::set_weapon_node(IAnimatedMeshSceneNode* wn)
{
	this->weapon_node = wn;
}

bool Weapon::is_animated()
{
	return ! this->weapon_node->getAnimators().empty();
}

vector3df Weapon::get_absolute_position()
{
	if(this->weapon_node)
	{
		return this->weapon_node->getAbsolutePosition();
	}

	return vector3df(0,0,0);
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
