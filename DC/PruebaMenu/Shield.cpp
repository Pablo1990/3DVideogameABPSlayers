#include "Shield.h"


Shield::Shield(ISceneManager *sm):Weapon(shield_path, 0, 0, sm, SHIELD_TYPE)
{
	cover = false;
}


Shield::~Shield(void)
{
}

void Shield::attack(float first_x, float first_y, float last_x, float last_y)
{
	if (weapon_node != NULL && !cover)
	{
		weapon_node->setPosition(vector3df(0,0,5));
		cover = true;
		weapon_node->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	}
}

void Shield::finish_animation()
{
	try
	{
		if (weapon_node != NULL && cover)
		{
			weapon_node->setPosition(vector3df(-5,-5,5));
			cover = false;
			weapon_node->setMaterialType(video::EMT_SOLID);
		}
	}
	catch(exception ex)
	{
	}
}

bool Shield::get_cover()
{
	return cover;
}