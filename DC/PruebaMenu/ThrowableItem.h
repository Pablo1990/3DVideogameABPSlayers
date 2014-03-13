#pragma once
#include "RangeWeapon.h"
#include "Paths.h"
class ThrowableItem: public RangeWeapon
{


public:
	ThrowableItem(ISceneManager *sm, ITriangleSelector *ms, IrrlichtDevice *d, int typ);
	~ThrowableItem(void);
	void attack(float first_x, float first_y, float last_x, float last_y);
	virtual void attack(int type, IAnimatedMeshSceneNode* node, vector3df player_position);
	
	static enum
	{
		RED_SHROOM,
		BLUE_SHROOM,
		YELLOW_SHROOM,
		STONE,
		TORCH
	};

protected:
		bool shot;
		vector3df scale;
};