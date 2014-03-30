#include "Character.h"
#include <iostream>
#include <time.h>  

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
		this->last_fall_time = 0;
		this->last_height = 0;

		this->heal_tick = 0;
		this->fire_tick;
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

		this->slow = 1;
		this->slow_start = -1;
		this->paralysis = false;
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
		this->last_height = this->character_node->getPosition().Y;
		

		if (this->character_node)
		{
			do_transformations_and_joints(position, rotation, scale);
			this->last_height = character_node->getPosition().Y;

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
		this->last_height = this->character_node->getPosition().Y;
		
		if(!weapon || (weapon && weapon->with_shield()))
			sh->add_to_camera(vector3df(-5,-5,5), vector3df(0,0,0), vector3df(3,3,3), camera);
		//sh->get_weapon_node()->setDebugDataVisible(EDS_BBOX_ALL);
		//camera->setParent(character_node);
		
		if (this->character_node)
		{
			do_transformations_and_joints(position, rotation, scale);
			this->character_node->addShadowVolumeSceneNode();
			this->last_height = character_node->getPosition().Y;
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
		cout << "Bucle4" << endl;
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
					this->weapon->get_weapon_node()->remove();
					//this->weapon->get_weapon_node()->getParent()->removeChild(this->weapon->get_weapon_node());
					//this->weapon->set_weapon_node(NULL);
				}
			}
		}
	}
	catch(...)
	{}
}

void Character::fall_down(IrrlichtDevice* device)
{

	int character_position = this->character_node->getPosition().Y;
	
	if(this->character_node->getPosition().Y > last_height)
	{
		last_height = this->character_node->getPosition().Y;
	}

	if(last_height - this->character_node->getPosition().Y > 150 /*device->getTimer()->getTime() - last_fall_time > 1000*/)
	{
		if(device->getTimer()->getTime() - last_fall_time > 1000)
		{
			this->health = this->health - 10;
			last_fall_time = device->getTimer()->getTime();
			last_height = this->character_node->getPosition().Y;
		}

	}
	else if(this->character_node->getPosition().Y == last_height && device->getTimer()->getTime() - last_fall_time > 1000)
	{
		last_fall_time = device->getTimer()->getTime();
	}

	if((int)health <= 0 && !is_dead)
	{
		this->health = 0;
		
		//character_node->setFrameLoop(62,211);
		//character_node->setAnimationSpeed(15);
		//character_node->setLoopMode(true);
		is_dead = true;
			
	}
}

void Character::reset_fall_time(IrrlichtDevice* device)
{
			last_fall_time = device->getTimer()->getTime();
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

				if(this->health > 0 && (d->getTimer()->getTime() - fire_tick > 1000))
				{
					this->health = this->health - 1;
					this->fire_tick = d->getTimer()->getTime();
				}

				return 1;
			}
			else if(heal && heal->getTransformedBoundingBox().
				intersectsWithBox(character_node->getTransformedBoundingBox()))
			{
				if(heal_flag)
				{
					if(this->health < 100 && (d->getTimer()->getTime() - heal_tick > 1000))
					{
						this->health = this->health + 1;
						this->heal_tick = d->getTimer()->getTime();
					}

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
					
					if(this->health < 100 && (d->getTimer()->getTime() - heal_tick > 1000))
					{
						this->health = this->health + 1;
						this->heal_tick = d->getTimer()->getTime();
					}

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
	ISceneNode* cosa = character_node;

	if(character_node)
		return character_node->getPosition();
	else return vector3df(0,0,0);
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

void Character::set_types(double* ty)
{
	this->types = ty;
}

void Character::replace_random_item( int index, std::list<Weapon*>* armas, IrrlichtDevice *device, 	scene::ITriangleSelector* mapSelector)
{
	try
	{
		
		scene::ISceneManager* sm = device->getSceneManager();
		std::list<Weapon*>::iterator it = armas->begin();


		for(int i = 0; i < index; i++)
		{
			cout << "Bucle5" << endl;
			it++;
		}

		vector3df position = (*it)->get_main_position();
		if((*it) && (*it)->get_weapon_node())
		{
			//(*it)->get_weapon_node()->setVisible(false);
			//sm->addToDeletionQueue((*it)->get_weapon_node());
			(*it)->get_weapon_node()->remove();
			//delete (*it);
		}
		Weapon *w;
		armas->remove(*it);
		srand((unsigned)time(0)); 
		int r = rand();
		r = r % 7;

		it = armas->begin();
		for(int i = 0; i < index; i++)
		{
			cout << "Bucle2" << endl;
			it++;
		}
		
		switch(r)
		{
			case 0:
				it = armas->insert(it, new Spear(6,5,sm));
				position.Y = 25;
				(*it)->add_to_scene(position, core::vector3df(90,0,0), core::vector3df(1.5,1.5,1.5), true, armas->size() - 1);
			/*armas.push_back( new Spear(0,0,sm));
			position.Y = 25;
			(*(--armas.end()))->add_to_scene(position, core::vector3df(90,0,0), core::vector3df(1.5,1.5,1.5), true, armas.size() - 1);*/
			break;
		case 1:
			it = armas->insert(it, new Sword(4,7,sm));
			(*it)->add_to_scene(position, core::vector3df(0,0,0), core::vector3df(0.008,0.008,0.008), true, armas->size() - 1);
			break;
		case 2:
			it = armas->insert(it, new Bow(4,4,sm, mapSelector, device));
			(*it)->add_to_scene(position, core::vector3df(90,0,0), core::vector3df(0.05,0.05,0.05), true, armas->size() - 1);
			break;
		case 3:
			it = armas->insert(it, new ThrowableItem(sm, mapSelector, device, ThrowableItem::RED_SHROOM));
			(*it)->add_to_scene(position, core::vector3df(0,0,0), core::vector3df(0.05,0.05,0.05), true, armas->size() - 1);
			break;
		case 4:
			it = armas->insert(it, new ThrowableItem(sm, mapSelector, device, ThrowableItem::BLUE_SHROOM));
			(*it)->add_to_scene(position, core::vector3df(0,0,0), core::vector3df(0.05,0.05,0.05), true, armas->size() - 1);
			break;
		case 5:
			it = armas->insert(it, new ThrowableItem(sm, mapSelector, device, ThrowableItem::YELLOW_SHROOM));
			(*it)->add_to_scene(position, core::vector3df(0,0,0), core::vector3df(0.05,0.05,0.05), true, armas->size() - 1);
			break;
		case 6:
			it = armas->insert(it, new ThrowableItem(sm, mapSelector, device, ThrowableItem::STONE));
			(*it)->add_to_scene(position, core::vector3df(0,0,0), core::vector3df(0.05,0.05,0.05), true, armas->size() - 1);
			break;
		}
		
		(*it)->get_weapon_node()->setName((std::to_string((*it)->get_type()) + '_' + std::to_string(index)).c_str());
		this->types[index] = (*it)->get_type();


		//int number;
		//int type;

		//number = atoi(((std::string)(*it)->get_weapon_node()->getName()).substr(strcspn((*it)->get_weapon_node()->getName(), "_") + 1).c_str());
		//type = atoi(((std::string)(*it)->get_weapon_node()->getName()).substr(0, strcspn((*it)->get_weapon_node()->getName(), "_")).c_str());
		
		
		int i = 0;
		for(it = armas->begin(); it != armas->end(); ++it)
		{
			cout << "Bucle3" << endl;
			/*if(i >= index && it != --armas.end())
			{
				number = atoi(((std::string)(*it)->get_weapon_node()->getName()).substr(strcspn((*it)->get_weapon_node()->getName(), "_") + 1).c_str());
				number--;
				type = atoi(((std::string)(*it)->get_weapon_node()->getName()).substr(0, strcspn((*it)->get_weapon_node()->getName(), "_")).c_str());
				//cout << "DA NAME " << type << "_" << number << endl;
				(*it)->get_weapon_node()->setName((std::to_string(type) + '_' + std::to_string(number)).c_str());
			}*/
			cout << "DA NAME " << (*it)->get_weapon_node()->getName() << endl;
	

			i++;
		}
		
//		cout << "DA NAME " << (*it)->get_weapon_node()->getName() << endl;

	}
	catch(...)
	{}
	
}
