#include "Npc.h"


Npc::Npc(ISceneManager *sm): Character(knight_path, sm)
{
}

Npc::Npc(ISceneManager *sm, Weapon* w): Character(knight_path, sm, w)
{
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
vector3df  Npc::DarPosSalud()
{
	vector3df v3=vector3df();
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
			
				
			double distaux=sqrt((pow((get_position().X-(*it)->get_absolute_position().X),2))+(pow((get_position().Y-(*it)->get_absolute_position().Y),2)));
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
	int y_E=player->get_position().Y;
	int x=get_position().X;
	int y=get_position().Y;
	int distaux=sqrt(pow((x-x_E),2)+pow((y-y_E),2));

	//POner distancia máxima de visión
	if(distaux <=9999)
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