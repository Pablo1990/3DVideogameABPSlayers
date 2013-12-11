#pragma once
#include "Character.h"
#include "Paths.h"
#include "Sword.h"
#include "Bow.h"

class Player: public Character
{
public:
	Player(ISceneManager *sm, ITriangleSelector* ms);
	Player(ISceneManager *sm, Weapon* w, ITriangleSelector *ms);
	~Player(void);
	
	void manage_collision(Weapon *w); 
	void drop_weapon(ISceneNode* cam);
	void pick_weapon(ISceneNode* cam, IAnimatedMeshSceneNode* w, IrrlichtDevice *device);

private:
	scene::ITriangleSelector* mapSelector;
};

