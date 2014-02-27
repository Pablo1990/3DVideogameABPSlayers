#include "Npc.h"


Npc::Npc(ISceneManager *sm,vector3df pos): Character(knight_path, sm)
{
	posHealth=pos;
}

Npc::Npc(ISceneManager *sm, Weapon* w,vector3df pos): Character(knight_path, sm, w)
{
	posHealth=pos;
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
					this->health = this->health - (w->get_damage() + 0.50 * w->get_damage());
					
					if((int)health <= 0)
					{
						
					}

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