#pragma once
#include "Weapon.h"
#include "Paths.h"
#include "Structs.h"
class Spear: public Weapon
{
public:
	Spear(int dmg, int sp, ISceneManager *sm);
	~Spear(void);
	bool attack(float first_x, float first_y, float last_x, float last_y);
	bool attack(int type, IAnimatedMeshSceneNode* node, vector3df player_position);
	void finish_animation();
};

