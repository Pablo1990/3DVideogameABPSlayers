﻿#include "Npc.h"


Npc::Npc(ISceneManager *sm,vector3df pos, IrrlichtDevice* d, ITriangleSelector* mp): Character(knight_path, sm)
{
	posHealth=pos;
	is_dead = false;
	is_moving = false;
	steps_count = 0;
	device = d;
	mapSelector = mp;
	m_dFitness = 0;
}


Npc::Npc(ISceneManager *sm, Weapon* w,vector3df pos, IrrlichtDevice* d, ITriangleSelector* mp): Character(knight_path, sm, w)
{
	posHealth=pos;
	is_dead = false;
	is_moving = false;
	steps_count = 0;
	device = d;
	mapSelector = mp;
	m_dFitness = 0;
}


Npc::Npc(ISceneManager *sm, Weapon* w,vector3df pos, IrrlichtDevice* d, ITriangleSelector* mp,ISceneNode* camp_fire_,ISceneNode* heal_): Character(knight_path, sm, w)
{
	posHealth=pos;
	is_dead = false;
	is_moving = false;
	steps_count = 0;
	device = d;
	mapSelector = mp;
	m_dFitness = 0;
	camp_fire=camp_fire_;
	heal=heal_;
}


Npc::~Npc(void)
{
	if(this->weapon)
	{
		delete weapon;
		weapon = 0;
	}
}

void Npc::set_pathfinding(Pathfinding *pf)
{
	path = pf;
}

void Npc::manage_collision(Weapon *w, IrrlichtDevice* d, SoundEffect* sound)
{
	try
	{

		//RangeWeapon* rw2 = dynamic_cast<RangeWeapon*>(w);
		if (w != NULL && !is_dead) 
		{

			if(!dynamic_cast<RangeWeapon*>(w) && !w->get_collision_flag() && w->is_animated() && !w->no_weapon())

			{
				if(detect_collision(w->get_weapon_node(), this->sh->get_weapon_node()))
				{
					this->lose_resistance();
					if(this->resistance < 6)
					{
						this->no_defend();
					}
					w->set_collision_flag(true);
					if(sound)
						sound->shield_sound();
				}
				else if (detect_collision(w->get_weapon_node(), this->head))
				{
					w->set_collision_flag(true);
					this->health = this->health - (w->get_damage() + 0.50 * w->get_damage());
					if(sound)
						sound->hit_sound();

				}
				else if (detect_collision(w->get_weapon_node(), this->body))
				{
					w->set_collision_flag(true);
					this->health = this->health - (w->get_damage() - 0.20 * w->get_damage());
					if(sound)
						sound->hit_sound();

				}
				else if (detect_collision(w->get_weapon_node(), this->extremity))
				{
					w->set_collision_flag(true);
					int restar = w->get_damage() - 0.40 * w->get_damage();
					this->health = this->health - (w->get_damage() - 0.40 * w->get_damage());
					if(sound)
						sound->hit_sound();

				}
				
			}
			else
			{
				if(dynamic_cast<ThrowableItem*>(w))
				{
					ThrowableItem* rw = dynamic_cast<ThrowableItem*>(w);
					//array<SParticleImpact> imp = rw->get_impacts();


					if(!rw->get_impacts().empty())
					{

						if(!rw->get_impact_at(0) && detect_collision(rw->get_impact_node_at(0), this->sh->get_weapon_node()))
						{
							this->lose_resistance();
							if(this->resistance < 6)
							{
								this->no_defend();
							}
							if(sound)
								sound->shield_sound();
						}
						else
						if((!rw->get_impact_at(0) && detect_collision(rw->get_impact_node_at(0), this->head)) || (!rw->get_impact_at(0) && detect_collision(rw->get_impact_node_at(0), this->body)) || (!rw->get_impact_at(0) && detect_collision(rw->get_impact_node_at(0), this->extremity)))
						{
							if(sound)
								sound->hit_sound();
							rw->set_impact_at(0, true);
							switch(rw->get_type())
							{
							case RED_SHROOM_TYPE:
								this->slow_start = d->getTimer()->getTime();
								this->slow = 2;
								break;
							case YELLOW_SHROOM_TYPE:
								this->paralysis_start = d->getTimer()->getTime();
								this->paralysis = true;
								break;
							case BLUE_SHROOM_TYPE:
								//restan cansancio, aun no hecho
								this->resistance = this->resistance - 5;
								break;
							case TORCH_TYPE:
								this->health = this->health - 1;
								break;
							case STONE_TYPE:
								this->health = this->health - 1;
								break;
							}
						}
					}
				}
				else if(dynamic_cast<RangeWeapon*>(w) && !w->no_weapon())
				{
					RangeWeapon* rw = dynamic_cast<RangeWeapon*>(w);
					//array<SParticleImpact> imp = rw->get_impacts();
					for(int i = 0; i < rw->get_impacts().size(); i++)
					{
						if(!rw->get_impact_at(i) && detect_collision(rw->get_impact_node_at(i), this->sh->get_weapon_node()))
						{
							this->lose_resistance();
							if(this->resistance < 6)
							{
								this->no_defend();
							}
							if(sound)
								sound->shield_sound();
						}
						else
						if(!rw->get_impact_at(i) && detect_collision(rw->get_impact_node_at(i), this->head))
						{
							rw->set_impact_at(i, true);

							if(sound)
								sound->hit_sound();
							this->health = this->health - (2 * ((w->get_damage() + 0.50 * w->get_damage()) 
								/ rw->get_distance_multiplier(i, this->character_node->getPosition().X,
								this->character_node->getPosition().Z)));
						}
						else if(!rw->get_impact_at(i) && detect_collision(rw->get_impact_node_at(i), this->body))
						{
							rw->set_impact_at(i, true);
						
							if(sound)
								sound->hit_sound();
							double resto = (2 * ((w->get_damage() - 0.40 * w->get_damage()) 
								/ rw->get_distance_multiplier(i, this->character_node->getPosition().X,
								this->character_node->getPosition().Z)));
							this->health = this->health - (2 * ((w->get_damage() - 0.40 * w->get_damage()) 
								/ rw->get_distance_multiplier(i, this->character_node->getPosition().X,
								this->character_node->getPosition().Z)));

						}
						else if(!rw->get_impact_at(i) && detect_collision(rw->get_impact_node_at(i), this->extremity))
						{
							rw->set_impact_at(i, true);

							if(sound)
								sound->hit_sound();
							this->health = this->health - (((w->get_damage() - 0.20 * w->get_damage())  
								/ rw->get_distance_multiplier(i, this->character_node->getPosition().X,
								this->character_node->getPosition().Z)) * 2);
						}
					}
				}
			}


			if((int)health <= 0 && !is_dead)
			{
				this->health = 0;



				//character_node->setAnimationSpeed(15);
				//character_node->setLoopMode(true);

				is_dead = true;


			}
		}
	}
	catch(...)
	{

	}
}


void Npc::Reset(){
	is_dead = false;
	is_moving = false;
	steps_count = 0;
	heal_flag = false;
	heal_count = 0;
	health=100;
	m_dFitness= 0;
}


void Npc::setItems(std::list<Weapon*>* armas,double * tipos)
{
	items=armas;
	this->types = tipos;

	int cont=0;
	for (std::list<Weapon*>::iterator it = items->begin();
		it != items->end();
		it++)
	{

		itemsPx[cont] =Clamp((*it)->get_absolute_position().X/1894.93,0,1);
		itemsPy[cont]=Clamp((*it)->get_absolute_position().Z/1294.88,0,1);

		cont++;

	}
	itemsPx[5] = Clamp(this->DarPosSalud().X/1894.93,0,1);
	itemsPy[5] = (this->DarPosSalud().Z/1294.88,0,1);
	itemsType=tipos;

}


void Npc::setEnem(Player* p)
{
	player=p;
}

vector<double> Npc::getPesosDeFichero(std::string nombre){
	
	
	vector<double> vecPesos;
	std::string pesos="";
	try
	{
		
		ifstream myfile (nombre);
		if (myfile.is_open())
		{
			char my_character ;
			int number_of_lines = 0;
		
			getline(myfile, pesos);
			getline(myfile, pesos);
			pesos = "";
			while (!myfile.eof() ) {
				myfile.get(my_character);
				pesos+=my_character;
			}
			myfile.close();
		}
		else cout << "Unable to open file";

		//string al vector
		std::string line = "";
		for(int i = 0; i<pesos.length(); i++)
		{
			if(pesos[i]!=' ')
				line+=pesos[i];
			else{
				vecPesos.push_back(atof(line.c_str()));
				line = "";
			}
		}
		vecPesos.push_back(atof(line.c_str()));
	}
	catch(...)
	{
		//string al vector
		std::string line = "";
		for(int i = 0; i<pesos.length(); i++)
		{
			if(pesos[i]!=' ')
				line+=pesos[i];
			else{
				vecPesos.push_back(atof(line.c_str()));
				line = "";
			}
		}
		vecPesos.push_back(atof(line.c_str()));
	}

	return vecPesos;
}

std::list<Weapon*>* Npc::getItems()
{
	return items;

}
vector3df  Npc::DarPosSalud()
{
	vector3df v3=vector3df();
	v3=posHealth;
	return v3;
}
vector3df  Npc::DarPosArmaCercana()
{
	int distancia=9999.9;

	vector3df v3=vector3df();
	for (std::list<Weapon*>::iterator it = items->begin();
		it != items->end();
		++it)
	{

		if(!(*it)->no_weapon())
		{
			double distaux=sqrt((pow((get_position().X-(*it)->get_absolute_position().X),2))+(pow((get_position().Z-(*it)->get_absolute_position().Z),2)));
			//Estandarizamos
			if(distaux <=distancia)
			{
				distancia=distaux;
				v3=(*it)->get_absolute_position();
				this->near_weapon = (*it);
			}
		}
	}
	return v3;
}

void Npc::setBrain(Goal_Think* mente_)
{

	mente=mente_;
}

Goal_Think* Npc::getBrain()
{
	return mente;

}
Player* Npc::getEnem()
{
	return player;
}

bool Npc::isEnemigoPresent()
{
	if(player !=NULL)
	{
		int x_E=player->get_position().X;
		int y_E=player->get_position().Z;
		int x=get_position().X;
		int y=get_position().Z;
		int distaux=sqrt(pow((x-x_E),2)+pow((y-y_E),2));

		//POner distancia máxima de visión
		if(distaux <=500)
		{
			return true;
		}
		//cout<<"entro Distancia:"<<distaux<<endl;
	}
	return false;
}


bool Npc::MoverseAItemSalud()
{


	if(!is_moving || status != 0)
	{
		init_pos.setX(this->character_node->getPosition().X);
		init_pos.setY(0);
		init_pos.setZ(this->character_node->getPosition().Z);

		//vector3df p_position = player->get_character_node()->getPosition();
		end_pos.setX(this->posHealth.X);
		end_pos.setY(0);
		end_pos.setZ(this->posHealth.Z);

		path->setPosInicio(init_pos);
		path->setPosFin(end_pos);

		path->AEstrella(250);
		steps_count = 0;
		status = 0;
	}

	this->way_to(path->getCamino());

	return true;
}
bool Npc::MoverseAItemArma()
{
	if(!is_moving || status != 1)
	{
		//if(!this->near_weapon)
		this->DarPosArmaCercana();

		init_pos.setX(this->character_node->getPosition().X);
		init_pos.setY(0);
		init_pos.setZ(this->character_node->getPosition().Z);

		//vector3df p_position = player->get_character_node()->getPosition();
		end_pos.setX(this->near_weapon->get_weapon_node()->getPosition().X);
		end_pos.setY(0);
		end_pos.setZ(this->near_weapon->get_weapon_node()->getPosition().Z);

		path->setPosInicio(init_pos);
		path->setPosFin(end_pos);

		path->AEstrella(250);
		steps_count = 0;
		status = 1;
	}

	this->way_to(path->getCamino());
	return true;
}
bool Npc::MoverseAEnemigo()
{


	/*	Position p1(this->character_node->getPosition().X, 0, this->character_node->getPosition().Z);
	vector3df p_position = player->get_character_node()->getPosition();
	Position p2(p_position.X, 0, p_position.Z);
	Pathfinding pf(p1, p2);
	Position last_corner(1894.93, 1, 1294.88);
	vector<vector<Position>> obstacles;
	vector<Position> v2;
	v2.push_back(last_corner);
	obstacles.push_back(v2);
	pf.setMapa(obstacles);

	vector<Position> way_points = pf.AEstrella(250);
	this->way_to(way_points);*/

	if(!is_moving || status != 2)
	{
		init_pos.setX(this->character_node->getPosition().X);
		init_pos.setY(0);
		init_pos.setZ(this->character_node->getPosition().Z);

		//vector3df p_position = player->get_character_node()->getPosition();
		end_pos.setX(player->get_character_node()->getPosition().X);
		end_pos.setY(0);
		end_pos.setZ(player->get_character_node()->getPosition().Z);

		path->setPosInicio(init_pos);
		path->setPosFin(end_pos);

		path->AEstrella(250);
		steps_count = 0;
		status = 2;
	}

	this->way_to(path->getCamino());
	return true;
}
bool Npc::Move_Explore()
{

	if(!is_moving || status != 3)
	{
		//if(!this->near_weapon)

		init_pos.setX(this->character_node->getPosition().X);
		init_pos.setY(0);
		init_pos.setZ(this->character_node->getPosition().Z);

		//vector3df p_position = player->get_character_node()->getPosition();

		srand((unsigned)time(0)); 
		int r = rand();
		int r2 = rand();
		r = r % 200;
		r = r +60;

		r2 = r2%200;
		r2 = r +60;

		if(r % 2 == 0)
		{


			if(init_pos.getX() + r > 1894)
			{
				end_pos.setX(init_pos.getX() - r);
			}
			else
			{
				end_pos.setX(init_pos.getX() + r);
			}


		}
		else
		{
			if(init_pos.getX() - r <= 40)
			{
				end_pos.setX(init_pos.getX() + r);
			}
			else
			{
				end_pos.setX(init_pos.getX() - r);
			}


		}

		if(r2 % 2 == 0)
		{

			if(init_pos.getZ() + r2 > 1294)
			{
				end_pos.setZ(init_pos.getZ() - r2);
			}
			else
			{
				end_pos.setZ(init_pos.getZ() + r2);
			}
		}
		else
		{
			if(init_pos.getZ() - r2 < 40)
			{
				end_pos.setZ(init_pos.getZ() + r2);
			}
			else
			{
				end_pos.setZ(init_pos.getZ() - r2);
			}
		}

		end_pos.setY(0);

		path->setPosInicio(init_pos);
		path->setPosFin(end_pos);

		path->AEstrella(50);
		steps_count = 0;
		status = 3;
	}

	this->way_to(path->getCamino());
	return true;
}
bool Npc::Move_ToFreeAttack()
{
	if(!is_moving || status != 4)
	{
		//if(!this->near_weapon)
		this->DarPosArmaCercana();

		init_pos.setX(this->character_node->getPosition().X);
		init_pos.setY(0);
		init_pos.setZ(this->character_node->getPosition().Z);

		//vector3df p_position = player->get_character_node()->getPosition();

		end_pos.setX(init_pos.getX());
		if(init_pos.getX() > player->get_position().X && init_pos.getX() + 100 < 1894)
		{
			end_pos.setX(init_pos.getX() + 100);
		}
		else if(init_pos.getX() < player->get_position().X && init_pos.getX() - 100 > 0)
		{
			end_pos.setX(init_pos.getX() - 100);
		}

		end_pos.setZ(init_pos.getZ());
		if(init_pos.getZ() > player->get_position().Z && init_pos.getZ() + 100 < 1294)
		{
			end_pos.setZ(init_pos.getZ() + 100);
		}
		else if(init_pos.getZ() < player->get_position().Z && init_pos.getZ() - 100 > 0)
		{
			end_pos.setZ(init_pos.getZ() - 100);
		}

		end_pos.setY(0);

		path->setPosInicio(init_pos);
		path->setPosFin(end_pos);

		path->AEstrella(250);
		steps_count = 0;
		status = 4;
	}

	this->way_to(path->getCamino());

	return true;

}

void Npc::add_weapon_to_node(vector3df position, vector3df rotation, vector3df scale)
{
	try
	{
		if(weapon)
		{
			weapon->add_to_node(position, rotation, scale, character_node);
		}
	}
	catch(...)
	{}

}

void Npc::attack(int type)
{
	try
	{
		if(weapon && !this->paralysis && resistance > 0)
		{
			//this->face_target(player->get_character_node());
			if(weapon->attack(type, this->character_node, scene_manager->getActiveCamera()->getPosition()))
				this->lose_resistance();

			if(dynamic_cast<ThrowableItem*>(this->weapon))
			{
				if(weapon->get_weapon_node())
				{
					this->weapon->set_resist(0);
					this->weapon->get_weapon_node()->remove();
					this->weapon->set_weapon_node(0);
					this->weapon->set_no_weapon(true);

				}
			}
		}
	}
	catch(...)
	{
	}

}

void Npc::attackBot(int type)
{
	try
	{
		if(weapon && !this->paralysis &&  !this->weapon->no_weapon() && resistance > 0)
		{
			double salud = this->enemigo->get_health();
			if(weapon->attack(type, this->character_node, scene_manager->getActiveCamera()->getPosition()))
				this->lose_resistance();

			if(dynamic_cast<ThrowableItem*>(this->weapon))
			{
				if(weapon->get_weapon_node())
				{
					this->weapon->set_resist(0);
					this->weapon->get_weapon_node()->remove();
					this->weapon->set_weapon_node(0);
					this->weapon->set_no_weapon(true);
				}
			}
			if(salud>=this->enemigo->get_health())
			{
				if(Fitness()>0)
					setFitness(Fitness()-1);
			}
		}
	}
	catch(...)
	{
	}

}

void Npc::pick_shield()
{
	try
	{
		if(sh && !sh->get_weapon_node())
		{
			sh->add_to_node(vector3df(-5,-5,5), vector3df(0,0,0), vector3df(3,3,3), this->character_node);
		}
	}
	catch(...)
	{
	}
}

void Npc::drop_shield()
{
	try
	{
		if(sh != NULL && sh->get_weapon_node())
		{
			this->character_node->removeChild(sh->get_weapon_node());
			sh->set_weapon_node(NULL);
		}
	}
	catch(...)
	{
	}
}


void Npc::pick_weapon()
{

	try
	{
		if(!near_weapon->no_weapon())
		{
			if(this->weapon && this->weapon->get_weapon_node())
			{
				character_node->removeChild(weapon->get_weapon_node());
				weapon->set_weapon_node(0);
				
			}
			//SWORD: position 40, 100, 0; rotation 180, -50, 90; scale 0.02, 0.02, 0.02
			//SPEAR: position 10, 100, -20; rotation 90,-50,90, scale 2.5, 2.5, 2.5
			//BOW: position 40, 100, 0; rotation 180, -50, 90, scale 0.02, 0.02, 0.02
			int pick_type = atoi(((std::string)this->near_weapon->get_weapon_node()->getName()).substr(0, strcspn(this->near_weapon->get_weapon_node()->getName(), "_")).c_str()); // strcspn (str,keys);//strtok(((std::string)w->getName()), "_");

			if(pick_type == SWORD_TYPE)
			{
				weapon = new Sword(4,7,scene_manager);
				this->add_weapon_to_node(core::vector3df(40, 100, 0), core::vector3df(180, -50, 90), core::vector3df(0.02, 0.02, 0.02));
				pick_shield();
			}
			else if(pick_type ==  BOW_TYPE)
			{
				weapon = new Spear(7,5,scene_manager);
				this->add_weapon_to_node(core::vector3df( 10, 100, -20), core::vector3df(90,-50,90), core::vector3df(2.5,2.5,2.5));
				drop_shield();
			}	
			else if(pick_type ==  RED_SHROOM_TYPE)
			{
				weapon = new ThrowableItem(scene_manager, mapSelector, device, ThrowableItem::RED_SHROOM);
				this->add_weapon_to_node(core::vector3df(40,110,20), core::vector3df(0,180,0), core::vector3df(0.05,0.05,0.05));
				pick_shield();
			}	
			else if(pick_type == SPEAR_TYPE)
			{
				weapon = new Spear(7,5,scene_manager);
				this->add_weapon_to_node(core::vector3df( 10, 100, -20), core::vector3df(90,-50,90), core::vector3df(2.5,2.5,2.5));
				drop_shield();
			}
			else if(pick_type ==  BLUE_SHROOM_TYPE)
			{
				weapon = new ThrowableItem(scene_manager, mapSelector, device, ThrowableItem::BLUE_SHROOM);
				this->add_weapon_to_node(core::vector3df(40,110,20), core::vector3df(0,180,0), core::vector3df(0.05,0.05,0.05));
				pick_shield();
			}	
			else if(pick_type ==  YELLOW_SHROOM_TYPE)
			{
				weapon = new ThrowableItem(scene_manager, mapSelector, device, ThrowableItem::YELLOW_SHROOM);
				this->add_weapon_to_node(core::vector3df(40,110,20), core::vector3df(0,180,0), core::vector3df(0.05,0.05,0.05));
				pick_shield();
			}
			else if(pick_type ==  STONE_TYPE)
			{
				weapon = new ThrowableItem(scene_manager, mapSelector, device, ThrowableItem::STONE);
				this->add_weapon_to_node(core::vector3df(40,110,20), core::vector3df(0,180,0), core::vector3df(0.05,0.05,0.05));
				pick_shield();
			}
			else if(pick_type ==  TORCH_TYPE)
			{
				weapon = new ThrowableItem(scene_manager, mapSelector, device, ThrowableItem::TORCH);
				this->add_weapon_to_node(core::vector3df(40,110,20), core::vector3df(0,180,0), core::vector3df(0.05,0.05,0.05));
				pick_shield();
			}
			this->delete_item(atoi(((std::string)this->near_weapon->get_weapon_node()->getName()).substr(strcspn(this->near_weapon->get_weapon_node()->getName(), "_") + 1).c_str()), items);
			//this->replace_random_item(atoi(((std::string)this->near_weapon->get_weapon_node()->getName()).substr(strcspn(this->near_weapon->get_weapon_node()->getName(), "_") + 1).c_str()), items, device, mapSelector);
		}

	}
	catch(...)
	{}
}
void Npc::move_to(Position p)
{

	if(!paralysis)
	{
		ISceneNodeAnimator *anim = scene_manager->createFlyStraightAnimator(
			this->get_position(), vector3df(p.getX(), p.getY(), p.getZ()), 70 * slow, false);
		this->get_character_node()->addAnimator(anim);
		anim->drop();
	}

}



void Npc::way_to(vector<Position> vp)
{

	try
	{
		if (character_node && character_node->getAnimators().size() > 0)
		{
			core::list<ISceneNodeAnimator*>::ConstIterator it = character_node->getAnimators().begin();
			if ((*it)->hasFinished())
			{
				character_node->removeAnimators();
				if(steps_count < vp.size())
				{
					this->move_to(vp[steps_count]);
					steps_count++;
					is_moving = true;
				}
			}
		}
		else
		{
			if(steps_count < vp.size())
			{
				this->move_to(vp[steps_count]);
				steps_count++;
				is_moving = true;
			}
			else
			{
				is_moving = false;
			}
		}
	}
	catch(...)
	{}

}

void Npc::face_target(ISceneNode* target)
{
	vector3df target_pos = target->getPosition();
	vector3df my_rotation = this->character_node->getRotation();

	vector3df node_pos = target_pos - this->character_node->getPosition();
	my_rotation.Y = atan(node_pos.Z/node_pos.X) * (180.0/irr::core::PI);

	if((target_pos.X - this->character_node->getPosition().X) > 0)
	{
		my_rotation.Y = 90 - my_rotation.Y;
	}
	else if((target_pos.X - this->character_node->getPosition().X) < 0)
	{
		my_rotation.Y = -90 - my_rotation.Y;
	}

	my_rotation.Y -= 180;

	this->character_node->setRotation(my_rotation);

}

void Npc::face_target(vector3df target_pos)
{
	vector3df my_rotation = this->character_node->getRotation();

	vector3df node_pos = target_pos - this->character_node->getPosition();
	my_rotation.Y = atan(node_pos.Z/node_pos.X) * (180.0/irr::core::PI);

	if((target_pos.X - this->character_node->getPosition().X) > 0)
	{
		my_rotation.Y = 90 - my_rotation.Y;
	}
	else if((target_pos.X - this->character_node->getPosition().X) < 0)
	{
		my_rotation.Y = -90 - my_rotation.Y;
	}

	my_rotation.Y -= 180;

	this->character_node->setRotation(my_rotation);

}

//Aprendizaje

bool Npc::Update()
{


	vector<double> inputs;
	//metemos los inputs:
	//poosiciones relativas todo
	inputs.push_back(getPosEnemX());
	inputs.push_back(getPosEnemY());
	inputs.push_back(getOrienEnem());
	inputs.push_back(getSaludEnem());
	inputs.push_back(getSaludPr());
	for(int i=0; i<4; i++)
		inputs.push_back(getDesgastePr()[i]);

	//darle solo la mas cercana
		//dar por tipos la más cercana: Ej: de la espada pos X e y , 
	double x1, y1 = 0;
	getPosItemMasCercano(x1,y1);
	inputs.push_back(x1);
	inputs.push_back(y1);

	//cambiar el sigmoid y el activationResponse 
	vector<double> output = m_ItsBrain.Update(inputs);
	//make sure there were no errors in calculating the 
	//output
	if (output.size() < CParams::iNumOutputs) 
	{
		return false;
	}

	//manda al juego lo que tiene que hacer en función del output

	//outputs:  //en cada linea no simultaneos entre ellos
	//Cubrirse: 0; ataque1: 1; ataque2: 2; ataque3: 3;
	//VRotacion derecha: 4; VRotacion izquierda: 5;
	//VMovimientoDelante: 6; VMovimientoAtrás: 7;


	if(output[4]>output[5])
	{
		if(output[4]>0.3)
		{
			ISceneNodeAnimator *anim = scene_manager->createRotationAnimator(vector3df(0, -output[4]*10, 0));
			get_character_node()->addAnimator(anim);
			anim->drop();
			
		}
	}
	else{
		if(output[5]>0.3)
		{
			ISceneNodeAnimator *anim = scene_manager->createRotationAnimator(vector3df(0, output[5]*10, 0));
			get_character_node()->addAnimator(anim);
			anim->drop();
			
		}
	}

	if(output[6]>output[7])
	{
		if(output[6]>0.3)
		{
			vector3df p = this->get_position();
			double Theta = character_node->getAbsoluteTransformation().getRotationDegrees().Y;
			double xp = (output[6]*10) * cos(Theta) - (output[6]*10) * sin(Theta);
			double zp = (output[6]*10) * sin(Theta) + (output[6]*10) * cos(Theta);
			xp = p.X + xp;
			zp = p.Z + zp;

			if(zp>=1290)
				zp = 1250;
			if(xp>=1890)
				xp = 1850;
			if(xp<0)
				xp = 10;
			if(zp<0)
				zp = 10;

			p.set(xp, p.Y, zp);
			this->get_character_node()->setPosition(p);
			
		}
	}
	else{
		if(output[7]>0.3)
		{
			vector3df p = get_position();
			double Theta = -character_node->getAbsoluteTransformation().getRotationDegrees().Y;
			double xp = (output[7]*10) * cos(Theta) - (output[7]*10) * sin(Theta);
			double zp = (output[7]*10) * sin(Theta) + (output[7]*10) * cos(Theta);
			xp = p.X - xp;
			zp = p.Z - zp;

			if(zp>=1290)
				zp = 1250;
			if(xp>=1890)
				xp = 1850;
			if(xp<0)
				xp = 10;
			if(zp<0)
				zp = 10;

			p.set(xp, p.Y, zp);
			this->get_character_node()->setPosition(p);


		}
	}

	double pAtaque = max(max(output[0], output[1]), max(output[2], output[3]));
	if(pAtaque>=0.5){
		if(pAtaque==output[0])
		{
			this->defend();
			
		}	
		else if(pAtaque == output[1])
		{
			this->no_defend();
			if(this->enemigo!=NULL)
				this->attackBot(0);
			else
				this->attack(0);

			//cout<<"Ataque 1"<<endl;
		}

		else if(pAtaque == output[2])
		{
			this->no_defend();
			//cout<<"Ataque 2"<<endl;
			if(this->enemigo!=NULL)
				this->attackBot(1);
			else
				this->attack(1);
		}

		else if(pAtaque == output[3])
		{
			this->no_defend();
			//cout<<"Ataque 3"<<endl;
			if(this->enemigo!=NULL)
				this->attackBot(2);
			else
				this->attack(2);
		}

	}
	this->heal_or_fire(camp_fire,heal,device);

	if(this->getEnemigo()!=NULL)
		this->manage_collision(this->getEnemigo()->get_weapon(),device);
	else
		this->manage_collision(this->getEnem()->get_weapon(),device);

	if(this->get_weapon())
	{
		this->get_weapon()->finish_animation();

	}	
	vector3df pos=this->DarPosArmaCercana();
	if((std::abs(pos.X-this->get_position().X)<=30 ) && std::abs(pos.Z-this->get_position().Z)<=30)

	{
		this->pick_weapon();
	}
	return true;
}

void Npc::setEnem(Npc* enem)
{
	enemigo=enem;	
}

Npc* Npc::getEnemigo()
{
	return enemigo;
}

void Npc::defend()
{
	try
	{
		if(weapon && !this->paralysis)
		{
			if((weapon->get_weapon_node() && !weapon->is_animated()) || !weapon->get_weapon_node())
			{
				sh->attack(1,0,0,0);
			}
		}
		else if(!this->paralysis)
		{
			sh->attack(1,0,0,0);
		}
	}
	catch(...)
	{
	}
}

void Npc::no_defend()
{
	try
	{
		if(weapon)
		{
			if((weapon->get_weapon_node() && !weapon->is_animated()) || !weapon->get_weapon_node())
			{
				sh->finish_animation();
			}
		}
		else
		{
			sh->finish_animation();
		}
	}
	catch(...)
	{
	}
}