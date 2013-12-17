#include "Character.h"

Character::Character(const char* path, ISceneManager *sm)
{
	this->scene_manager = sm;

	this->character_mesh = sm->getMesh(path); 
	this->weapon = NULL;
	this->heal_flag = false;
	this->heal_count = 0;

	sh = new Shield(scene_manager);

}

Character::Character(const char* path, ISceneManager *sm, Weapon* w)
{
	this->scene_manager = sm;

	this->character_mesh = sm->getMesh(path);
	this->weapon = w;
	this->heal_flag = false;
	this->heal_count = 0;


	sh = new Shield(scene_manager);

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
	this->character_node = scene_manager->addAnimatedMeshSceneNode(this->character_mesh);  //this is the important line where you make "gun" child of the camera so it moves when the camera moves
	character_node->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
	character_node->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL_REF);
	
	character_node->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
	character_node->setMaterialFlag(EMF_COLOR_MASK, 0);
	//character_node->setDebugDataVisible(EDS_BBOX_ALL);

	sh->add_to_camera(vector3df(-5,-5,5), vector3df(0,0,0), vector3df(3,3,3), camera);
	//sh->get_weapon_node()->setDebugDataVisible(EDS_BBOX_ALL);
	//camera->setParent(character_node);
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
		try
		{
			if(a->getTransformedBoundingBox().intersectsWithBox(b[i]->getTransformedBoundingBox()))
			{
				return true;
			}
		}
		catch(exception ex)
		{
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
	if (this->weapon && !sh->get_cover())
	{
			this->weapon->attack(first_x, first_y,  last_x,  last_y);
			if(dynamic_cast<ThrowableItem*>(this->weapon))
			{
				this->weapon->get_weapon_node()->getParent()->removeChild(this->weapon->get_weapon_node());
				this->weapon->set_weapon_node(NULL);
			}
	}
}

//0 Nada, 1 Fuego, 2 Cura
int Character::heal_or_fire(ISceneNode* camp_fire, ISceneNode* heal, IrrlichtDevice* d)
{
	if(character_node)
	{
		if( camp_fire && camp_fire->getTransformedBoundingBox().
			intersectsWithBox(character_node->getTransformedBoundingBox()))
		{
			heal_flag = false;
			return 1;
		}
		else if(heal && heal->getTransformedBoundingBox().
			intersectsWithBox(character_node->getTransformedBoundingBox()))
		{
			if(heal_flag)
			{
				if(d->getTimer()->getTime() - heal_time < 5000)
				{
					return 2;
				}
				else
				{
					heal_flag = false;
				}
			}
			else if(heal_count < 5)
			{
				heal_time = d->getTimer()->getTime();
				heal_count++;
				heal_flag = true;
				return 2;
			}
			
		}
		else
		{
			heal_flag = false;
		}
	}
		return 0;
}

void Character::movement(ICameraSceneNode* camera)
{
	/*vector3df rotation = camera->getRotation();
	vector3df position = camera->getPosition();
	
	rotation.X = 0;
	rotation.Z = 0;
	rotation.Y += 180;
	//rotation.Y = 0;
	//rotation.Y +=  character_node->getRotation().Y;
	//rotation.X +=  character_node->getRotation().X;

	position.Z += -15;
	position.Y += -70;

	character_node->setPosition(position);
	character_node->setRotation(rotation);*/
}

void Character::defend()
{
}

void Character::no_defend()
{
}