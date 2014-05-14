#pragma once
#include "Weapon.h"
#include "Structs.h"
#include "Paths.h"
class Shield: public Weapon
{
public:
	Shield(ISceneManager *sm);
	~Shield(void);
	bool attack(float first_x, float first_y, float last_x, float last_y);
	void finish_animation();
	bool get_cover();
private:
	bool cover;
};

