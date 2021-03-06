#pragma once
#include "Weapon.h"
#include "Paths.h"
#include "Structs.h"

class Sword: public Weapon
{
public:
	Sword(int dmg, int sp, ISceneManager *sm);
	~Sword(void);
	bool attack(float first_x, float first_y, float last_x, float last_y);
	bool attack(int type, IAnimatedMeshSceneNode* node, vector3df player_position);
	void finish_animation();
};

