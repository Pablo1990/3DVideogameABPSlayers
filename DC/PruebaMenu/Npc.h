#pragma once
#include "Character.h"
#include "Paths.h"
#include "Bow.h"

#include <IMeshManipulator.h>

class Npc: public Character
{
public:
	Npc(ISceneManager *sm);
	Npc(ISceneManager *sm, Weapon* w);
	~Npc(void);

	void manage_collision(Weapon *w); 
};

