#include "Weapon.h"
#include <iostream>

Weapon::Weapon(const char* path, int dmg = 0, int sp = 0, ISceneManager *sm = 0, int t = -1)
{
	try
	{
		this->damage = dmg;
		this->speed = sp;

		this->scene_manager = sm;

		this->weapon_mesh = sm->getMesh(path); 
		this->weapon_mesh->setMaterialFlag(video::EMF_LIGHTING, false);
		this->ty = t;
		this->no_weapon_flag = false;
		this->resist=15;
		this->distance = 60;
	}
	catch(...)
	{}
}
void Weapon::set_no_weapon(bool wp)
{
	this->no_weapon_flag = wp;
}

int Weapon::get_distance()
{
	return this->distance;
}

bool Weapon::no_weapon()
{
	return this->no_weapon_flag;
}

void Weapon::add_to_scene(vector3df position, vector3df rotation, vector3df scale, bool pickable, int index)
{
	try
	{
		if(scene_manager)
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
			main_position = position;
			main_rotation = rotation;
			weapon_node->setName((std::to_string(ty) + '_' + std::to_string(index)).c_str());
			//cout << weapon_node->getName() << endl;
						weapon_node->setDebugDataVisible(EDS_BBOX_ALL);

		}
	}
	catch(...)
	{}
}

void Weapon::add_to_camera(vector3df position, vector3df rotation, vector3df scale, ISceneNode* camera)
{
	try
	{
		if(scene_manager)
			this->weapon_node = scene_manager->addAnimatedMeshSceneNode(this->weapon_mesh, camera, ID_IsNotPickable);  //this is the important line where you make "gun" child of the camera so it moves when the camera moves
	
		if(this->weapon_node)
		{
			this->weapon_node->setScale(scale);
			this->weapon_node->setPosition(position); 
			this->weapon_node->setRotation(rotation);
			main_position = position;
			main_rotation = rotation;
			weapon_node->setDebugDataVisible(EDS_BBOX_ALL);

		}
	}
	catch(...)
	{}
}

bool Weapon::get_collision_flag()
{
	return this->collision_flag;
}

void Weapon::set_collision_flag(bool cf)
{
	this->collision_flag = cf;
}

int Weapon::get_damage()
{
	return this->damage;
}

void Weapon::set_damage(int d)
{
	this->damage = d;
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
	try
	{
		if(weapon_node)
			return ! this->weapon_node->getAnimators().empty();
	}
	catch(...)
	{
		return false;
	}
	return false;
}

vector3df Weapon::get_absolute_position()
{
	try
	{
		if(this->weapon_node && !no_weapon_flag)
		{
			return this->weapon_node->getAbsolutePosition();
		}
	}
	catch(...)
	{}
	return vector3df(0,0,0);
}

Weapon::~Weapon(void)
{
	if(weapon_node && weapon_node->getParent())
	{
		weapon_node->getParent()->removeChild(weapon_node);
		weapon_node = 0;
	}
	else if(weapon_node)
	{
		this->weapon_node->remove();
		this->weapon_node = 0;
	}
}

void Weapon::attack(float first_x, float first_y, float last_x, float last_y)
{
}

void Weapon::finish_animation()
{
}

void Weapon::set_resist(double r)
{
	resist=r;
}

double Weapon::get_resist()
{
	return resist;
}

void Weapon::set_main_position(vector3df pos)
{
	main_position = pos;
}

void Weapon::add_to_node(vector3df position, vector3df rotation, vector3df scale, ISceneNode* node)
{
	try
	{
		if(scene_manager)
			this->weapon_node = scene_manager->addAnimatedMeshSceneNode(this->weapon_mesh, node, ID_IsNotPickable);  //this is the important line where you make "gun" child of the camera so it moves when the camera moves
	
		if(this->weapon_node)
		{
			this->weapon_node->setScale(scale);
			this->weapon_node->setPosition(position); 
			this->weapon_node->setRotation(rotation);
			weapon_node->setDebugDataVisible(EDS_BBOX_ALL);
			main_position = position;
			main_rotation = rotation;
		}
	}
	catch(...)
	{}
}

void Weapon::attack(int type,IAnimatedMeshSceneNode* node, vector3df player_position)
{
}

bool Weapon::with_shield()
{
	return shield;
}

int Weapon::get_type()
{
	return this->ty;
}

vector3df Weapon::get_main_position()
{
	return main_position;
}