#include "Npc.h"


Npc::Npc(ISceneManager *sm,vector3df pos): Character(knight_path, sm)
{
	posHealth=pos;
	is_dead = false;
}

Npc::Npc(ISceneManager *sm, Weapon* w,vector3df pos): Character(knight_path, sm, w)
{
	posHealth=pos;
	is_dead = false;
}

Npc::~Npc(void)
{
}

void Npc::manage_collision(Weapon *w)
{
	try
	{
		
		RangeWeapon* rw2 = dynamic_cast<RangeWeapon*>(w);
		if (w != NULL) 
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
				if(dynamic_cast<RangeWeapon*>(w))
				{
					RangeWeapon* rw = dynamic_cast<RangeWeapon*>(w);
					array<SParticleImpact> imp = rw->get_impacts();
					for(int i = 0; i < imp.size(); i++)
					{
						if(!imp[i].collision_flag && detect_collision(imp[i].node, this->head))
						{
							imp[i].collision_flag = true;
							if(scene_manager)
							{
								IMeshManipulator* mesh_manipulator = scene_manager->getMeshManipulator();
								if(mesh_manipulator)
								{
									mesh_manipulator->setVertexColors(character_node->getMesh(), SColor(255, 255, 0,   0));//RED
								}
							}
						}
						else if(!imp[i].collision_flag && detect_collision(imp[i].node, this->body))
						{
							imp[i].collision_flag = true;
							if(scene_manager)
							{
								IMeshManipulator* mesh_manipulator = scene_manager->getMeshManipulator();
								if(mesh_manipulator)
								{
									mesh_manipulator->setVertexColors(character_node->getMesh(), SColor(255, 0,   0,  255));//BLUE
								}
							}
						}
						else if(!imp[i].collision_flag && detect_collision(imp[i].node, this->extremity))
						{
							imp[i].collision_flag = true;
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
				}
			}
		}

		if((int)health <= 0 && !is_dead)
		{
			std::cout << "FIRST FRAME" << character_node->getStartFrame() << endl;
			std::cout << "END FRAME: " << character_node->getEndFrame() << endl;
			
			//character_node->setFrameLoop(1822,1928);
			//character_node->setAnimationSpeed(1);
			//character_node->setLoopMode(false);
			is_dead = true;
			
		}
	}
	catch(...)
	{

	}
}

std::list<Weapon*> Npc::getItems()
{
	return items;

}
void Npc::setItems(std::list<Weapon*> armas)
{
	items=armas;
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
		if(weapon)
		{
			weapon->attack(type);
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
	this->character_node->addAnimator(scene_manager->createFlyStraightAnimator(character_node->getPosition(), vector3df(p.getX(), p.getY(), p.getZ()), 1000, false, false));
}