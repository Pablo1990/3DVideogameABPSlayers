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