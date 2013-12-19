#include "Bow.h"


Bow::Bow(int dmg, int sp, ISceneManager *sm, ITriangleSelector *ms, IrrlichtDevice *d):RangeWeapon(bow_path, dmg, sp, sm, ms, d, BOW_TYPE,"../media/arrowita.obj")
{
	last_shot = -2000;
	
}

Bow::~Bow(void)
{
	last_shot = -2000;
}

void Bow::attack(float first_x, float first_y, float last_x, float last_y)
{
	if(device)
	{
		u32 now = device->getTimer()->getTime();
		if(now -last_shot > reload_time)
		{
			this->shoot_anim(vector3df(1.4,1.4,1.4));
			last_shot = device->getTimer()->getTime();
		}
	}
}