#include "Character.h"


Character::Character(char* path,ISceneManager *sm)
{
	this->scene_manager = sm;

	character_mesh = sm->getMesh(path); 
	weapon = NULL;
}

Character::Character(char* path,ISceneManager *sm, Weapon* w)
{
	this->scene_manager = sm;

	character_mesh = sm->getMesh(path); 
	weapon = w;
}

Character::~Character(void)
{
}

void Character::add_to_scene(vector3df position, vector3df rotation, vector3df scale)
{
	character_node = scene_manager->addAnimatedMeshSceneNode(character_mesh);
	if(character_node)
	{
		character_node->setScale(scale);
		character_node->setRotation(rotation);
		character_node->setPosition(position);
		
		calculate_joint();

	}
}

void Character::add_to_camera(vector3df position, vector3df rotation, vector3df scale, ICameraSceneNode* camera)
{
	character_node = scene_manager->addAnimatedMeshSceneNode(character_mesh, camera);  //this is the important line where you make "gun" child of the camera so it moves when the camera moves
	if(character_node)
	{
		character_node->setScale(scale);
		character_node->setPosition(position); 
		character_node->setRotation(rotation);

		character_node->setMaterialFlag(video::EMF_LIGHTING, false);
		character_node->addShadowVolumeSceneNode();

		calculate_joint();
	}
}

void Character::calculate_joint()
{
	if(character_node)
	{
		//Caja cabeza
		head.push_back(character_node->getJointNode("Bip01_Head"));

		//Cajas cuerpo
		body.push_back(character_node->getJointNode("Bip01_Spine1"));
		body.push_back(character_node->getJointNode("Bip01_Spine"));	
		body.push_back(character_node->getJointNode("Bip01_Neck"));
		
		//Cajas pierna derecha
		extremity.push_back(character_node->getJointNode("Bip01_Pelvis"));
		extremity.push_back(character_node->getJointNode("Bip01_R_Thigh"));
		extremity.push_back(character_node->getJointNode("Bip01_R_Calf"));
		extremity.push_back(character_node->getJointNode("Bip01_R_Foot"));

		//Cajas pierna izquierda
		extremity.push_back(character_node->getJointNode("Bip01_L_Thigh"));
		extremity.push_back(character_node->getJointNode("Bip01_L_Calf"));
		extremity.push_back(character_node->getJointNode("Bip01_L_Foot"));

		//Cajas brazo derecho
		//this->model1_RClavicle = model1->getJointNode("Bip01_R_Clavicle");
		extremity.push_back(character_node->getJointNode("Bip01_R_UpperArm"));
		extremity.push_back(character_node->getJointNode("Bip01_R_Forearm"));
		extremity.push_back(character_node->getJointNode("Bip01_R_Hand"));


		//Cajas brazo izquierdo
		//this->model1_LClavicle = model1->getJointNode("Bip01_L_Clavicle");
		extremity.push_back(character_node->getJointNode("Bip01_L_UpperArm"));
		extremity.push_back(character_node->getJointNode("Bip01_L_Forearm"));
		extremity.push_back(character_node->getJointNode("Bip01_L_Hand"));
	}

	
}

bool Character::detect_collision(ISceneNode* a, vector<ISceneNode*> b)
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

void Character::manage_collision()
{
}

void Character::attack(float first_x, float first_y, float last_x, float last_y)
{
	if(weapon)
	{
		weapon->attack(first_x, first_y,  last_x,  last_y);
	}
}