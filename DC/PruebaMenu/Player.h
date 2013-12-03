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
	void drop_weapon(ISceneNode* cam);
	void pick_weapon(ISceneNode* cam, IAnimatedMeshSceneNode* w);
};

