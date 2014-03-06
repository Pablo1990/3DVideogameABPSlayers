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
	RangeWeapon(const char* path, int dmg, int sp, ISceneManager *sm, ITriangleSelector *ms, 	IrrlichtDevice *d, int type, const char* ammo_paht);
	~RangeWeapon(void);
	void finish_animation();
	virtual void attack(float first_x, float first_y, float last_x, float last_y);
	void shoot_anim(vector3df scale, vector3df rotation);
	bool is_animated();
	array<SParticleImpact> get_impacts();
	void set_collision_flag(bool flag, int index);
	void set_ammo_mesh(const char* path);
protected:
	array<SParticleImpact> Impacts;
	ITriangleSelector* mapSelector;
	IrrlichtDevice *device;
	IParticleSystemSceneNode* campFire;
	IAnimatedMesh* ammo_mesh;
};

