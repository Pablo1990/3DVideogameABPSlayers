#pragma once
#include "Weapon.h"
#include "Structs.h"
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
class RangeWeapon: public Weapon
{
public:
	RangeWeapon(const char* path, int dmg, int sp, ISceneManager *sm, ITriangleSelector *ms, 	IrrlichtDevice *d, int type);
	~RangeWeapon(void);
	virtual void finish_animation();
	virtual void attack(float first_x, float first_y, float last_x, float last_y);
	void shoot_anim();
protected:
	array<SParticleImpact> Impacts;
	ITriangleSelector* mapSelector;
	IrrlichtDevice *device;
	IParticleSystemSceneNode* campFire;
};

