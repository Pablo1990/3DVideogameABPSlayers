#pragma once
#include "RangeWeapon.h"
#include "Paths.h"
#include "Structs.h"

const s32 reload_time = 1200;

class Bow: public RangeWeapon
{
public:
	Bow(int dmg, int sp, ISceneManager *sm, ITriangleSelector *ms, 	IrrlichtDevice *d);
	~Bow(void);

	void finish_animation();
	void attack(float first_x, float first_y, float last_x, float last_y);

private:
	u32 last_shot;
};

