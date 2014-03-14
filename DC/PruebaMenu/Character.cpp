#include "Character.h"

Character::Character(const char* path, ISceneManager *sm)
{
	try
	{
		this->scene_manager = sm;

		this->character_mesh = sm->getMesh(path); 
		this->weapon = NULL;
		this->heal_flag = false;
		this->heal_count = 0;
		this->health=100;
		sh = new Shield(scene_manager);

		this->slow = 1;
		this->slow_start = -1;
		this->paralysis = false;
		this->paralysis_start = -1;

	}
	catch(...)
	{}
}

Character::Character(const char* path, ISceneManager *sm, Weapon* w)
{
	try
	{
		this->scene_manager = sm;

		this->character_mesh = sm->getMesh(path);
		this->weapon = w;
		this->heal_flag = false;
		this->heal_count = 0;

		this->health=100;
		sh = new Shield(scene_manager);
	}
	catch(...)
	{
	}

}

Character::~Character(void)
{
}

void Character::do_transformations_and_joints(vector3df position, vector3df rotation, vector3df scale)
{
	try
	{
		this->character_node->setScale(scale);
		this->character_node->setRotation(rotation);
		this->character_node->setPosition(position);
		this->character_node->setMaterialFlag(video::EMF_LIGHTING, false);
	}
	catch(...)
	{}

	calculate_joint();
}

void Character::add_to_scene(vector3df position, vector3df rotation, vector3df scale)
{
	try
	{
		this->character_node = scene_manager->addAnimatedMeshSceneNode(this->character_mesh);

		

		if (this->character_node)
		{
			do_transformations_and_joints(position, rotation, scale);

			if((!weapon || (weapon && weapon->with_shield())) && sh)
				sh->add_to_camera(vector3df(-30,110,-30), vector3df(0,180,0), vector3df(30,30,30), this->character_node);
		}
	}
	catch(...)
	{}
}

void Character::add_to_camera(vector3df position, vector3df rotation, vector3df scale, ICameraSceneNode* camera)
{
	try
	{
		this->character_node = scene_manager->addAnimatedMeshSceneNode(this->character_mesh);  //this is the important line where you make "gun" child of the camera so it moves when the camera moves
		character_node->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
		character_node->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL_REF);
	
		character_node->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
		character_node->setMaterialFlag(EMF_COLOR_MASK, 0);
		//character_node->setDebugDataVisible(EDS_BBOX_ALL);

		if(!weapon || (weapon && weapon->with_shield()))
			sh->add_to_camera(vector3df(-5,-5,5), vector3df(0,0,0), vector3df(3,3,3), camera);
		//sh->get_weapon_node()->setDebugDataVisible(EDS_BBOX_ALL);
		//camera->setParent(character_node);
		if (this->character_node)
		{
			do_transformations_and_joints(position, rotation, scale);
			this->character_node->addShadowVolumeSceneNode();
		}
	}
	catch(...)
	{}
}

void Character::calculate_joint()
{
	try
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
	catch(...)
	{}
}

bool Character::detect_collision(ISceneNode* a, vector<IBoneSceneNode*> b)
{
	int tam = 0;



	for(unsigned int i = 0; i < b.size(); i++)
	{
		try
		{
			if(a && b[i] && a->getTransformedBoundingBox().intersectsWithBox(b[i]->getTransformedBoundingBox()))
			{
				return true;
			}
		}
		catch(...)
		{
			return false;
		}
	}

	return false;
	/*return a->getTransformedBoundingBox().
		intersectsWithBox(b->getTransformedBoundingBox());*/
}

ISceneNode* Character::get_character_node()
{
	return this->character_node;
}

Weapon* Character::get_weapon()
{
	return this->weapon;
}

void Character::set_weapon(Weapon* w)
{
	try
	{
		this->weapon = w;
	}
	catch(...)
	{}
}

bool Character::no_weapon()
{
	if(this->weapon == NULL)
	{
		return true;
	}
	else return this->weapon->get_weapon_node() == NULL;
}

void Character::manage_collision(Weapon *, IrrlichtDevice* d)
{
}

void Character::attack(float first_x, float first_y, float last_x, float last_y)
{
	try
	{
		if (this->weapon && !sh->get_cover() && !this->paralysis)
		{
				this->weapon->attack(first_x, first_y,  last_x,  last_y);
				if(dynamic_cast<ThrowableItem*>(this->weapon))
				{
					if(weapon->get_weapon_node())
					{
						this->weapon->get_weapon_node()->getParent()->removeChild(this->weapon->get_weapon_node());
						this->weapon->set_weapon_node(NULL);
					}
				}
		}
	}
	catch(...)
	{}
}

//0 Nada, 1 Fuego, 2 Cura
int Character::heal_or_fire(ISceneNode* camp_fire, ISceneNode* heal, IrrlichtDevice* d)
{
	try
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
	}
	catch(...)
	{}
	return 0;
}

void Character::movement(ICameraSceneNode* camera)
{
	try
	{
		if(camera)
		{
			vector3df rotation = camera->getRotation();
			vector3df position = camera->getPosition();
	
			rotation.X = 0;
			rotation.Z = 0;
			rotation.Y += 180;
			//rotation.Y = 0;
			//rotation.Y +=  character_node->getRotation().Y;
			//rotation.X +=  character_node->getRotation().X;

			position.Z += -15;
			position.Y += -70;

			if(character_node)
			{
				character_node->setPosition(position);
				character_node->setRotation(rotation);
			}
		}
	}
	catch(...)
	{}
}

void Character::defend()
{
}

void Character::no_defend()
{
}

double Character::get_health()
{
	return health;
}
void Character::set_health(double h)
{
	health=h;
}

vector3df Character::get_position()
{
	return character_node->getPosition();
}
void Character::set_position(double x,double y ,double z)
{
	vector3df v3=vector3df(x,y,z);
	
	character_node->addAnimator(scene_manager->createFlyStraightAnimator(character_node->getPosition(),v3,150,false,false));
}

void Character::restore_condition(IrrlichtDevice* d)
{
	if(slow_start != -1 && d->getTimer()->getTime() - slow_start > 3000)
	{
		slow = 1;
		slow_start = -1;
	}

	if(paralysis_start != -1 && d->getTimer()->getTime() - paralysis_start > 3000)
	{
		paralysis = false;
		paralysis_start = -1;
	}
}