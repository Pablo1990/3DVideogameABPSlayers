#include "Shield.h"


Shield::Shield(ISceneManager *sm):Weapon(shield_path, 0, 0, sm, SHIELD_TYPE)
{
	cover = false;
}


Shield::~Shield(void)
{
}

bool Shield::attack(float first_x, float first_y, float last_x, float last_y)
{
	try
	{
		if(first_x == 0)
		{
			if (weapon_node != NULL && !cover)
			{
				this->main_position = vector3df(-5,-5,5);
				weapon_node->setPosition(vector3df(0,0,5));
				cover = true;
				weapon_node->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
				return true;
			}
		}
		else
		{
			if (weapon_node != NULL && !cover)
			{
				main_position = weapon_node->getPosition();
				weapon_node->setPosition(vector3df(0,130,-30));
				cover = true;
				weapon_node->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
				return true;
			}
		}
		return false;
	}
	catch(...)
	{
		return false;
	}
}

void Shield::finish_animation()
{
	try
	{
		if (weapon_node != NULL && cover)
		{
			weapon_node->setPosition(main_position);
			cover = false;
			weapon_node->setMaterialType(video::EMT_SOLID);
		}
	}
	catch(...)
	{
	}
}

bool Shield::get_cover()
{
	return cover;
}