#pragma once
#include "Character.h"
#include "Paths.h"
#include "Sword.h"
#include "Bow.h"
#include "ThrowableItem.h"
#include "Spear.h"
class Player: public Character
{
public:
	Player(ISceneManager *sm, ITriangleSelector* ms, ICameraSceneNode *camera);
	Player(ISceneManager *sm, Weapon* w, ITriangleSelector *ms, ICameraSceneNode *camera);
	~Player(void);
	
	void manage_collision(Weapon *w, IrrlichtDevice* d, SoundEffect* sound = NULL); 
	void drop_weapon(ISceneNode* cam);
	void pick_weapon(ISceneNode* cam, IAnimatedMeshSceneNode* w, IrrlichtDevice *device, std::list<Weapon*>* armas);
	void pick_weapon(ISceneNode* cam, int pick_type, IrrlichtDevice* device);
	void defend();
	void no_defend();

	void restore_condition(IrrlichtDevice* d);

private:
	ICameraSceneNode *cam;
	scene::ITriangleSelector* mapSelector;
	void drop_shield();
	void pick_shield();
};

