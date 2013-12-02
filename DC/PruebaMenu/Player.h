#pragma once
#include "Character.h"
#include "Paths.h"
class Player: public Character
{
public:
	Player(ISceneManager *sm);
	Player(ISceneManager *sm, Weapon* w);
	~Player(void);
	void manage_collision(Weapon *w); 
	void drop_weapon(ICameraSceneNode* cam);
	void pick_weapon(ICameraSceneNode* cam, IAnimatedMeshSceneNode* w);
};

