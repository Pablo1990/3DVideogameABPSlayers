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
		this->is_dead = false;
		this->resistance = 20;
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
		this->paralysis_start = -1;
		this->last_fall_time = 0;
		this->last_height = 0;

		this->heal_tick = 0;
		this->fire_tick;

		this->is_dead = false;
		this->resistance = 20;

	}
	catch(...)
	{
	}

}

Character::~Character(void)
{

		if(this->sh)
	{
		delete this->sh;
		this->sh = 0;
	}

	if(this->weapon)
	{
		delete this->weapon;
		this->weapon = 0;
	}

	if(this->character_node)
	{
		this->character_node->getParent()->removeChild(this->character_node);
		this->character_node = 0;
	}

	//Esto son partes del nodo, imagino que si se borra el nodo estas tambien
	for(int i = 0; i < this->extremity.size(); i++)
	{
		this->extremity[i] = 0;
	}

	//Esto son partes del nodo, imagino que si se borra el nodo estas tambien
	for(int i = 0; i < this->head.size(); i++)
	{
		this->head[i] = 0;
	}

	//Esto son partes del nodo, imagino que si se borra el nodo estas tambien
	for(int i = 0; i <this->body.size(); i++)
	{
		this->body[i] = 0;
	}


}

bool Character::get_is_dead()
{
	return this->is_dead;
}

void Character::die(IrrlichtDevice* device)
{

	/*IParticleSystemSceneNode* campFire = scene_manager->addParticleSystemSceneNode(false,0,1 << 0);
	campFire->setPosition(character_node->getPosition());
	campFire->setScale(core::vector3df(10,30,10));
	campFire->setName(std::to_string(TORCH_TYPE).c_str());
	
	scene::IParticleEmitter* em = campFire->createBoxEmitter(
		core::aabbox3d<f32>(-7,0,-7,7,1,7),
		core::vector3df(0.0f,0.06f,0.0f),
		40,100, video::SColor(1,255,255,255),video::SColor(1,255,255,255), 1000,2200);

	em->setMinStartSize(core::dimension2d<f32>(20.0f, 10.0f));
	em->setMaxStartSize(core::dimension2d<f32>(80.0f, 40.0f));
	campFire->setEmitter(em);
	em->drop();

	scene::IParticleAffector* paf = campFire->createFadeOutParticleAffector();
	campFire->addAffector(paf);
	paf->drop();
	
	campFire->setMaterialFlag(video::EMF_LIGHTING, false);
	campFire->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	campFire->setMaterialTexture(0, device->getVideoDriver()->getTexture("../media/smoke.bmp"));
	campFire->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);*/
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
		try
		{
			if(a!=NULL  && a->getReferenceCount() && a->getReferenceCount() > 0 && b[i] && a->getTransformedBoundingBox().intersectsWithBox(b[i]->getTransformedBoundingBox()))
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

bool Character::detect_collision(ISceneNode* a, IAnimatedMeshSceneNode* b)
{

		try
		{
			if(a!=NULL  && a->getReferenceCount() && a->getReferenceCount() > 0 && b && a->getTransformedBoundingBox().intersectsWithBox(b->getTransformedBoundingBox()))
			{
				return true;
			}
		}
		catch(...)
		{
			return false;
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

void Character::remove_character_node()
{
	this->character_node->setVisible(false);
}

void Character::set_weapon(Weapon* w)
{
	try
	{
		if(this->weapon)
		{
			delete this->weapon;
		}
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

void Character::manage_collision(Weapon *, IrrlichtDevice* d, SoundEffect* sound)
{
}

void Character::attack(float first_x, float first_y, float last_x, float last_y)
{
	try
	{
		if (this->weapon && !sh->get_cover() && !this->paralysis && !this->weapon->no_weapon() && resistance > 0)
		{

			if(this->weapon->attack(first_x, first_y,  last_x,  last_y))
				this->lose_resistance();
			if(dynamic_cast<ThrowableItem*>(this->weapon))
			{
				if(weapon->get_weapon_node())
				{
		
					this->weapon->get_weapon_node()->getParent()->removeChild(this->weapon->get_weapon_node());
					this->weapon->set_weapon_node(NULL);
					this->weapon->set_no_weapon(true);
					
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

	if(d->getTimer()->getTime() - resistance_count > 3000)
	{
		this->gain_resistance();
		resistance_count = d->getTimer()->getTime();
	}
}

void Character::set_types(double* ty)
{
	this->types = ty;
}

void Character::delete_item(int index, std::list<Weapon*>* armas)
{

		std::list<Weapon*>::iterator it = armas->begin();

		for(int i = 0; i < index; i++)
		{
			it++;
		}
		if((*it) != NULL && (*it)->get_weapon_node() != NULL)
		{
			(*it)->set_no_weapon(true);
			(*it)->get_weapon_node()->remove();
			(*it)->set_weapon_node(0);
		}
}

bool Character::can_i_heal()
{
	if(this->heal_count < 5)
		return true;
	else return false;
}

void Character::lose_resistance()
{
	resistance = (resistance - 3);
	if(resistance <= 0)
		resistance = 0;
}

void Character::gain_resistance()
{
	resistance = resistance + 3;
	if(resistance > 20)
		resistance = 20;
}

int Character::get_resistance()
{
	return resistance;
}