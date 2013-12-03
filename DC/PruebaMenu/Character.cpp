#include "Character.h"

Character::Character(const char* path, ISceneManager *sm)
{
	this->scene_manager = sm;

	this->character_mesh = sm->getMesh(path); 
	this->weapon = NULL;
}

Character::Character(const char* path, ISceneManager *sm, Weapon* w)
{
	Character(path, sm);
	this->weapon = w;
}

Character::~Character(void)
{
}

void Character::do_transformations_and_joints(vector3df position, vector3df rotation, vector3df scale)
{
	this->character_node->setScale(scale);
	this->character_node->setRotation(rotation);
	this->character_node->setPosition(position);
	this->character_node->setMaterialFlag(video::EMF_LIGHTING, false);
	
	calculate_joint();
}

void Character::add_to_scene(vector3df position, vector3df rotation, vector3df scale)
{
	this->character_node = scene_manager->addAnimatedMeshSceneNode(this->character_mesh);
	if (this->character_node)
	{
		do_transformations_and_joints(position, rotation, scale);
	}
}

void Character::add_to_camera(vector3df position, vector3df rotation, vector3df scale, ICameraSceneNode* camera)
{
	this->character_node = scene_manager->addAnimatedMeshSceneNode(this->character_mesh, camera);  //this is the important line where you make "gun" child of the camera so it moves when the camera moves
	if (this->character_node)
	{
		do_transformations_and_joints(position, rotation, scale);
		this->character_node->addShadowVolumeSceneNode();
	}
}

void Character::calculate_joint()
{
	if (this->character_node)
	{
		//Caja cabeza
		head.push_back(this->character_node->getJointNode("Bip01_Head"));

		//Cajas cuerpo
		body.push_back(this->character_node->getJointNode("Bip01_Spine1"));
		body.push_back(this->character_node->getJointNode("Bip01_Spine"));	
		body.push_back(this->character_node->getJointNode("Bip01_Neck"));
		
		//Cajas pierna derecha
		extremity.push_back(this->character_node->getJointNode("Bip01_Pelvis"));
		extremity.push_back(this->character_node->getJointNode("Bip01_R_Thigh"));
		extremity.push_back(this->character_node->getJointNode("Bip01_R_Calf"));
		extremity.push_back(this->character_node->getJointNode("Bip01_R_Foot"));

		//Cajas pierna izquierda
		extremity.push_back(this->character_node->getJointNode("Bip01_L_Thigh"));
		extremity.push_back(this->character_node->getJointNode("Bip01_L_Calf"));
		extremity.push_back(this->character_node->getJointNode("Bip01_L_Foot"));

		//Cajas brazo derecho
		//this->model1_RClavicle = model1->getJointNode("Bip01_R_Clavicle");
		extremity.push_back(this->character_node->getJointNode("Bip01_R_UpperArm"));
		extremity.push_back(this->character_node->getJointNode("Bip01_R_Forearm"));
		extremity.push_back(this->character_node->getJointNode("Bip01_R_Hand"));

		//Cajas brazo izquierdo
		//this->model1_LClavicle = model1->getJointNode("Bip01_L_Clavicle");
		extremity.push_back(this->character_node->getJointNode("Bip01_L_UpperArm"));
		extremity.push_back(this->character_node->getJointNode("Bip01_L_Forearm"));
		extremity.push_back(this->character_node->getJointNode("Bip01_L_Hand"));
	}
}

bool Character::detect_collision(ISceneNode* a, vector<IBoneSceneNode*> b)
{
	for(unsigned int i = 0; i < b.size(); i++)
	{
		if(a->getTransformedBoundingBox().intersectsWithBox(b[i]->getTransformedBoundingBox()))
		{
			return true;
		}
	}

	return false;
	/*return a->getTransformedBoundingBox().
		intersectsWithBox(b->getTransformedBoundingBox());*/
}

Weapon* Character::get_weapon()
{
	return this->weapon;
}

void Character::set_weapon(Weapon* w)
{
	this->weapon = w;
}

bool Character::no_weapon()
{
	return this->weapon->get_weapon_node() == NULL;
}

void Character::manage_collision(Weapon *)
{
}

void Character::attack(float first_x, float first_y, float last_x, float last_y)
{
	if (this->weapon)
	{
		this->weapon->attack(first_x, first_y,  last_x,  last_y);
	}
}