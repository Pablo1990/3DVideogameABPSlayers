#include "Sword.h"


Sword::Sword(int dmg, int sp, ISceneManager *sm):Weapon(sword_path, dmg, sp, sm, SWORD_TYPE)
{
}

Sword::~Sword(void)
{
}

void Sword::attack(float first_x, float first_y, float last_x, float last_y)
{
	try
	{
	if (weapon_node != NULL && weapon_node->getAnimators().empty())
		{
			float difX, difY;
			difX = abs(abs(first_x) - abs(last_x));
			difY = abs(abs(first_y) - abs(last_y));

			if (difY < 3 && difX < 3)
			{
				weapon_node->setRotation(core::vector3df(0,0,0));
				
				weapon_node->addAnimator(scene_manager->createFlyStraightAnimator(
					weapon_node->getPosition(), 
					core::vector3df(weapon_node->getPosition().X, weapon_node->getPosition().Y, weapon_node->getPosition().Z + 20)
					, 150, false, true));

				weapon_node->setLoopMode(false);
			}
			else
			{
				if (difY < difX) //Estocada lateral
				{
					if (first_x < last_x) //Hacia la derecha
					{
						weapon_node->setRotation(core::vector3df(10,0,0));
	
						weapon_node->addAnimator(scene_manager->createFlyStraightAnimator(
							core::vector3df(weapon_node->getPosition().X - 30, weapon_node->getPosition().Y , weapon_node->getPosition().Z),
							core::vector3df(weapon_node->getPosition().X + 40, weapon_node->getPosition().Y , weapon_node->getPosition().Z),
							300, false, true));

						weapon_node->setLoopMode(false);
			
					}
					else //Hacia la izquierda
					{
						weapon_node->setRotation(core::vector3df(10,0,0));

						weapon_node->addAnimator(scene_manager->createFlyStraightAnimator(
							core::vector3df(weapon_node->getPosition().X + 30, weapon_node->getPosition().Y , weapon_node->getPosition().Z),
							core::vector3df(weapon_node->getPosition().X - 40, weapon_node->getPosition().Y , weapon_node->getPosition().Z),
							300, false, true));

						weapon_node->setLoopMode(false);
			
					}
				}
				else //Estocada vertical
				{
						weapon_node->setRotation(core::vector3df(0,0,12));		

						weapon_node->addAnimator(scene_manager->createFlyStraightAnimator(
							core::vector3df(weapon_node->getPosition().X, weapon_node->getPosition().Y + 40 , weapon_node->getPosition().Z),
							core::vector3df(weapon_node->getPosition().X, weapon_node->getPosition().Y - 30, weapon_node->getPosition().Z),
							300, false, true));

						weapon_node->setLoopMode(false);
				}
			}
	}
	}
	catch(...)
	{
	}
}

void Sword::finish_animation()
{
	try
	{
		if (weapon_node && weapon_node->getAnimators().size() > 0)
		{
			list<ISceneNodeAnimator*>::ConstIterator it = weapon_node->getAnimators().begin();
			if ((*it)->hasFinished())
			{
				weapon_node->setPosition(core::vector3df(15, -10, 20));
				weapon_node->setRotation(core::vector3df(0, 50, 90));
				weapon_node->removeAnimators();
				weapon_node->removeAnimators();
				collision_flag = false;
			}
		}
	}
	catch(...)
	{
	}
}