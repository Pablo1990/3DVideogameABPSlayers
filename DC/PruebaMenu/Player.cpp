#include "Player.h"
#include <string.h>
#include <iostream>;
using namespace std;

Player::Player(ISceneManager *sm, ITriangleSelector* ms, ICameraSceneNode *camera): Character(knight_path, sm)
{
	mapSelector = ms;
	cam = camera;
}

Player::Player(ISceneManager *sm, Weapon* w, ITriangleSelector* ms, ICameraSceneNode *camera): Character(knight_path, sm, w)
{
		mapSelector = ms;
		cam = camera;;
}


Player::~Player(void)
{
}

void Player::manage_collision(Weapon *w, IrrlichtDevice* d)
{
	try
	{
		
		//RangeWeapon* rw2 = dynamic_cast<RangeWeapon*>(w);
		if (w != NULL && !w->no_weapon() && !is_dead) 
		{
			if(!dynamic_cast<RangeWeapon*>(w) && !w->get_collision_flag() && w->is_animated())
			{
				if (detect_collision(w->get_weapon_node(), this->head))
				{
					w->set_collision_flag(true);
					this->health = this->health - (w->get_damage() + 0.50 * w->get_damage());
					//this->health = 0;
					
					

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
							core::list<ISceneNodeAnimator*>::ConstIterator anims=cam->getAnimators().begin();
							ISceneNodeAnimatorCameraFPS *anim=(ISceneNodeAnimatorCameraFPS*)*anims;
							switch(rw->get_type())
							{
								case RED_SHROOM_TYPE:
									this->slow_start = d->getTimer()->getTime();
									this->slow = 2;
									
									anim->setMoveSpeed(.2f);
									break;
								case YELLOW_SHROOM_TYPE:
									this->paralysis_start = d->getTimer()->getTime();
									this->paralysis = true;
									anim->setMoveSpeed(0);
									//((ISceneNodeAnimatorCameraFPS*)cam)->setMoveSpeed(0);
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
		
				//character_node->setFrameLoop(62,211);
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

void Player::drop_weapon(ISceneNode* cam)
{

	try
	{
		if(weapon->get_weapon_node() && weapon->get_weapon_node()->getAnimators().empty())
		{
			cam->removeChild(weapon->get_weapon_node());
			weapon->set_weapon_node(NULL);
			weapon->set_no_weapon(true);
			pick_shield();
		}
	}
	catch(...)
	{
	}
	
}

void Player::pick_weapon(ISceneNode* cam, IAnimatedMeshSceneNode* w, 	IrrlichtDevice *device, std::list<Weapon*>* armas)
{
	try
	{
		int pick_type = atoi(((std::string)w->getName()).substr(0, strcspn(w->getName(), "_")).c_str()); // strcspn (str,keys);//strtok(((std::string)w->getName()), "_");
		cout << "Recojo " << pick_type << endl;
		if (no_weapon() && cam != NULL && (cam = dynamic_cast<ICameraSceneNode*>(cam)))
		{
			if(pick_type == SWORD_TYPE)
			{
				weapon = new Sword(4,7,scene_manager);
				weapon -> add_to_camera(core::vector3df(15,-10,20), core::vector3df(0,50,90), core::vector3df(0.008,0.008,0.008), cam);
				pick_shield();
			}
			else if(pick_type ==  BOW_TYPE)
			{
				weapon = new Bow(4,4,scene_manager, mapSelector, device);
				weapon->add_to_camera(core::vector3df(15,-10,20), core::vector3df(0,-90,0), core::vector3df(0.02,0.02,0.02), cam);
				drop_shield();
			}	
			else if(pick_type ==  RED_SHROOM_TYPE)
			{
				weapon = new ThrowableItem(scene_manager, mapSelector, device, ThrowableItem::RED_SHROOM);
				weapon->add_to_camera(core::vector3df(15,-10,20), core::vector3df(0,-90,0), core::vector3df(0.02,0.02,0.02), cam);
				pick_shield();
			}	
			else if(pick_type == SPEAR_TYPE)
			{
				weapon = new Spear(7,5,scene_manager);
				weapon -> add_to_camera(core::vector3df(10,-30,20), core::vector3df(-140,0,0), core::vector3df(1.4,1.4,1.4), cam);
				drop_shield();
			}
			else if(pick_type ==  BLUE_SHROOM_TYPE)
			{
				weapon = new ThrowableItem(scene_manager, mapSelector, device, ThrowableItem::BLUE_SHROOM);
				weapon->add_to_camera(core::vector3df(15,-10,20), core::vector3df(0,-90,0), core::vector3df(0.02,0.02,0.02), cam);
				pick_shield();
			}	
			else if(pick_type ==  YELLOW_SHROOM_TYPE)
			{
				weapon = new ThrowableItem(scene_manager, mapSelector, device, ThrowableItem::YELLOW_SHROOM);
				weapon->add_to_camera(core::vector3df(15,-10,20), core::vector3df(0,-90,0), core::vector3df(0.02,0.02,0.02), cam);
				pick_shield();
			}
			else if(pick_type ==  STONE_TYPE)
			{
				weapon = new ThrowableItem(scene_manager, mapSelector, device, ThrowableItem::STONE);
				weapon->add_to_camera(core::vector3df(15,-10,20), core::vector3df(0,-90,0), core::vector3df(0.05,0.05,0.05), cam);
				pick_shield();
			}
			else if(pick_type ==  TORCH_TYPE)
			{
				weapon = new ThrowableItem(scene_manager, mapSelector, device, ThrowableItem::TORCH);
				weapon->add_to_camera(core::vector3df(15,-10,20), core::vector3df(0,-90,0), core::vector3df(1,1,1), cam);
				pick_shield();
			}

			this->delete_item(atoi(((std::string)w->getName()).substr(strcspn(w->getName(), "_") + 1).c_str()), armas);
			//this->replace_random_item(atoi(((std::string)w->getName()).substr(strcspn(w->getName(), "_") + 1).c_str()), armas, device, mapSelector);
		}
	}
	catch(...)
	{}
}

void Player::drop_shield()
{
	try
	{
		if(sh != NULL && sh->get_weapon_node())
		{
			cam->removeChild(sh->get_weapon_node());
			sh->set_weapon_node(NULL);
		}
	}
	catch(...)
	{
	}
}

void Player::pick_shield()
{
	try
	{
		if(sh && !sh->get_weapon_node())
		{
			sh->add_to_camera(vector3df(-5,-5,5), vector3df(0,0,0), vector3df(3,3,3), cam);
		}
	}
	catch(...)
	{
	}
}

void Player::defend()
{
	try
	{
		if(weapon && !this->paralysis)
		{
			if((weapon->get_weapon_node() && !weapon->is_animated()) || !weapon->get_weapon_node())
			{
				sh->attack(0,0,0,0);
			}
		}
		else if(!this->paralysis)
		{
			sh->attack(0,0,0,0);
		}
	}
	catch(...)
	{
	}
}

void Player::no_defend()
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

void Player::restore_condition(IrrlichtDevice* d)
{
	if(slow_start != -1 && d->getTimer()->getTime() - slow_start > 3000)
	{
		slow = 1;
		slow_start = -1;
		core::list<ISceneNodeAnimator*>::ConstIterator anims=cam->getAnimators().begin();
		ISceneNodeAnimatorCameraFPS *anim=(ISceneNodeAnimatorCameraFPS*)*anims;
		anim->setMoveSpeed(.2f);
	}

	if(paralysis_start != -1 && d->getTimer()->getTime() - paralysis_start > 3000)
	{
		paralysis = false;
		paralysis_start = -1;
		
		core::list<ISceneNodeAnimator*>::ConstIterator anims=cam->getAnimators().begin();
		ISceneNodeAnimatorCameraFPS *anim=(ISceneNodeAnimatorCameraFPS*)*anims;
		anim->setMoveSpeed(.2f);
	}
}