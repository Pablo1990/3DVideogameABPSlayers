
#include "Npc.h"


Npc::Npc(ISceneManager *sm,vector3df pos): Character(knight_path, sm)
{
	posHealth=pos;
	is_dead = false;
	is_moving = false;
	steps_count = 0;
}

Npc::Npc(ISceneManager *sm, Weapon* w,vector3df pos): Character(knight_path, sm, w)
{
	posHealth=pos;
	is_dead = false;
	is_moving = false;
	steps_count = 0;

}

Npc::~Npc(void)
{
}

void Npc::manage_collision(Weapon *w, IrrlichtDevice* d)
{
	try
	{

		//RangeWeapon* rw2 = dynamic_cast<RangeWeapon*>(w);
		if (w != NULL && !is_dead) 
		{
			if(!dynamic_cast<RangeWeapon*>(w) && !w->get_collision_flag() && w->is_animated())
			{
				if (detect_collision(w->get_weapon_node(), this->head))
				{
					w->set_collision_flag(true);
					//this->health = this->health - (w->get_damage() + 0.50 * w->get_damage());
					this->health = 0;



					if(scene_manager)
					{
						IMeshManipulator* mesh_manipulator = scene_manager->getMeshManipulator();
						if(mesh_manipulator)
						{
							mesh_manipulator->setVertexColors(character_node->getMesh(), SColor(255, 255, 0,   0));//RED
						}
					}
				}
				else if (detect_collision(w->get_weapon_node(), this->body))
				{
					w->set_collision_flag(true);
					this->health = this->health - (w->get_damage() - 0.20 * w->get_damage());

					if(scene_manager)
					{
						IMeshManipulator* mesh_manipulator = scene_manager->getMeshManipulator();
						if(mesh_manipulator)
						{
							mesh_manipulator->setVertexColors(character_node->getMesh(), SColor(255, 0,   0,  255));//BLUE
						}
					}
				}
				else if (detect_collision(w->get_weapon_node(), this->extremity))
				{
					w->set_collision_flag(true);
					int restar = w->get_damage() - 0.40 * w->get_damage();
					this->health = this->health - (w->get_damage() - 0.40 * w->get_damage());

					if(scene_manager)
					{
						IMeshManipulator* mesh_manipulator = scene_manager->getMeshManipulator();
						if(mesh_manipulator)
						{
							mesh_manipulator->setVertexColors(character_node->getMesh(), SColor(255, 255, 255, 0));//YELLOW
						}
					}
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
						if((!rw->get_impact_at(0) && detect_collision(rw->get_impact_node_at(0), this->head))
							|| (!rw->get_impact_at(0) && detect_collision(rw->get_impact_node_at(0), this->body))
							|| (!rw->get_impact_at(0) && detect_collision(rw->get_impact_node_at(0), this->extremity)))
						{
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
				else if(dynamic_cast<RangeWeapon*>(w))
				{
					RangeWeapon* rw = dynamic_cast<RangeWeapon*>(w);
					//array<SParticleImpact> imp = rw->get_impacts();
					for(int i = 0; i < rw->get_impacts().size(); i++)
					{
						if(!rw->get_impact_at(i) && detect_collision(rw->get_impact_node_at(i), this->head))
						{
							rw->set_impact_at(i, true);
							if(scene_manager)
							{
								IMeshManipulator* mesh_manipulator = scene_manager->getMeshManipulator();
								if(mesh_manipulator)
								{
									mesh_manipulator->setVertexColors(character_node->getMesh(), SColor(255, 255, 0,   0));//RED
								}

							}

							this->health = this->health - ((w->get_damage() + 0.50 * w->get_damage()) 
								/ rw->get_distance_multiplier(i, this->character_node->getPosition().X,
								this->character_node->getPosition().Z));
						}
						else if(!rw->get_impact_at(i) && detect_collision(rw->get_impact_node_at(i), this->body))
						{
							rw->set_impact_at(i, true);
							if(scene_manager)
							{
								IMeshManipulator* mesh_manipulator = scene_manager->getMeshManipulator();
								if(mesh_manipulator)
								{
									mesh_manipulator->setVertexColors(character_node->getMesh(), SColor(255, 0,   0,  255));//BLUE
								}


							}

							this->health = this->health - ((w->get_damage() - 0.40 * w->get_damage()) 
								/ rw->get_distance_multiplier(i, this->character_node->getPosition().X,
								this->character_node->getPosition().Z));

						}
						else if(!rw->get_impact_at(i) && detect_collision(rw->get_impact_node_at(i), this->extremity))
						{
							rw->set_impact_at(i, true);
							if(scene_manager)
							{
								IMeshManipulator* mesh_manipulator = scene_manager->getMeshManipulator();
								if(mesh_manipulator)
								{
									mesh_manipulator->setVertexColors(character_node->getMesh(), SColor(255, 255, 255, 0));//YELLOW
								}
							}

							this->health = this->health - ((w->get_damage() - 0.20 * w->get_damage())  
								/ rw->get_distance_multiplier(i, this->character_node->getPosition().X,
								this->character_node->getPosition().Z));
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

				std::cout << "FIRST FRAME" << character_node->getStartFrame() << endl;
				std::cout << "END FRAME: " << character_node->getEndFrame() << endl;

			}
		}
	}
	catch(...)
	{

	}
}

void Npc::Reset(vector3df p){
	is_dead = false;
	is_moving = false;
	steps_count = 0;
	heal_flag = false;
	heal_count = 0;
	health=100;
	this->character_node->setPosition(p);
}

std::list<Weapon*> Npc::getItems()
{
	return items;

}
void Npc::setItems(std::list<Weapon*> armas,double * tipos)
{
	items=armas;

	int cont=0;
		for (std::list<Weapon*>::iterator it = items.begin();
			  it != items.end();
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
	for (std::list<Weapon*>::iterator it = items.begin();
		it != items.end();
		++it)
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

void Npc::setEnem(Player* p)
{
	player=p;
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
	return true;
}
bool Npc::MoverseAItemArma()
{
	return true;
}
bool Npc::MoverseAEnemigo()
{

	return true;
}
bool Npc::Move_Explore()
{
	return true;
}
bool Npc::Move_ToFreeAttack()
{

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
		if(weapon && !this->paralysis)
		{
			weapon->attack(type, this->character_node, this->player->get_position());
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
	{
	}

}

void Npc::pick_weapon()
{
	try
	{
		this->weapon->get_weapon_node()->drop();
		this->weapon = this->near_weapon;

		this->add_weapon_to_node(core::vector3df(40, 100, 0), core::vector3df(180, -50, 90), core::vector3df(0.02, 0.02, 0.02));
		this->get_weapon()->set_resist(15);
		//NECESITO UN METODO QUE ME DIGA SI EL ARMA VA CON O SIN ESCUDO, BOOLEANO QUE SE INICIE EN EL CONSTRUCTOR
		//DE CADA ARMA AL VALOR QUE TOQUE; LUEGO RECUPERAR CON UN GET

		//TAMBIEN ES NECESARIO QUE LOS VALORES DE AÑADIR AL NODO LOS PONGA LA CLASE DE CADA ARMA, PARA ABSTRAER Y QUE NO SEA
		//NECESARIO CONOCER EL ARMA PARA AÑADIRLA
	}
	catch(...)
	{}
}

void Npc::move_to(Position p)
{

	if(!paralysis)
	{
		ISceneNodeAnimator *anim = scene_manager->createFlyStraightAnimator(
			this->get_position(), vector3df(p.getX(), p.getY(), p.getZ()), 70 / slow, false);
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
				}
			}
		}
		else
		{
			if(steps_count < vp.size())
			{
				this->move_to(vp[steps_count]);
				steps_count++;
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
	inputs.push_back(this->get_health()); //mi vida
	inputs.push_back(this->getEnemigo()->get_health()); //vida enemigo
	//mi posicion, habria que ver como lo metemos, porque asi son muchos parametros
	inputs.push_back(this->get_position().X); 
	inputs.push_back(this->get_position().Y);
	inputs.push_back(this->get_position().Z);
	//posicion enemigo, a lo mejor habria que ver la distancia al enemigo en vez de esto
	inputs.push_back(this->getEnemigo()->get_position().X);
	inputs.push_back(this->getEnemigo()->get_position().Y);
	inputs.push_back(this->getEnemigo()->get_position().Z);
	//posiciones de objetos?

	inputs.push_back(this->get_weapon()->get_resist()); //resistencia arma
	vector<double> output = m_ItsBrain.Update(inputs);
	//make sure there were no errors in calculating the 
	//output
	if (output.size() < CParams::iNumOutputs) 
	{
		return false;
	}

	//manda al juego lo que tiene que hacer en función del output
	//en plan atacar, defender etcc..
	Position pIzq(get_position().X, get_position().Y, get_position().Z);
	Position pDer(get_position().X, get_position().Y, get_position().Z);
	Position pDelante(get_position().X, get_position().Y, get_position().Z);
	Position pAtras(get_position().X, get_position().Y, get_position().Z);
	if(output[0])
		this->attack(0);//distintos ataques??
	else if(output[1])
		this->defend();
	else if(output[2]){
		this->face_target(vector3df(pIzq.getX(),pIzq.getY(),pIzq.getZ()));
		this->move_to(pIzq);
	}
	return true;
	//aqui se se supone que debería actualizar inputs, pero eso
	//se debera hacer al actuar (se encarga irrlicht)
}

void Npc::setEnem(Npc* enem)
{
	enemigo=enem;	
}

Npc* Npc::getEnemigo()
{
	return enemigo;
}

