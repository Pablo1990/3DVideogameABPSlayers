#pragma once

#include <irrlicht.h>
#include <iostream>
#include "Npc.h"
#include "Sword.h"
#include "Weapon.h"
#include "Player.h"
#include "Bow.h"
#include "ThrowableItem.h"
#include "Spear.h"
using namespace std;


using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#endif
class Juego: public IEventReceiver
{
public:

	Juego(video::E_DRIVER_TYPE);
	~Juego(void);
	void run();
	void switchToNextScene();
	virtual bool OnEvent(const SEvent& event);
	void loadSceneData();

private:
	video::E_DRIVER_TYPE driverType;
	IrrlichtDevice *device;
	s32 sceneStartTime;
	gui::IGUIStaticText* statusText;
	scene::IQ3LevelMesh* quakeLevelMesh;

	scene::IMetaTriangleSelector* metaSelector;
	scene::IMetaTriangleSelector* metaModelSelector;

	scene::ISceneNode* quakeLevelNode;

	scene::ITriangleSelector* mapSelector;
		video::SColor backColor;
	scene::IParticleSystemSceneNode* campFire;
	scene::IParticleSystemSceneNode* heal_camp;

	scene::ICameraSceneNode* camera;


	float lastX;
	float firstX;
	float firstY;
	float lastY;

	scene::ISceneNode* skyboxNode;
	
	Npc* npc;
	Player* player;
	Sword* sw;
	Sword* dropped_sword;
	Bow* dropped_bow;
	ThrowableItem* dropped_red_shroom;
	Spear* dropped_spear;
	bool crouch;
	scene::ITriangleSelector* model1_selector;
	scene::ITriangleSelector* selector;
	scene::ISceneNodeAnimatorCollisionResponse* collider;

	enum
{
    // I use this ISceneNode ID to indicate a scene node that is
    // not pickable by getSceneNodeAndCollisionPointFromRay()
    ID_IsNotPickable = 0,

    // I use this flag in ISceneNode IDs to indicate that the
    // scene node can be picked by ray selection.
    IDFlag_IsPickable = 1 << 0,

    // I use this flag in ISceneNode IDs to indicate that the
    // scene node can be highlighted.  In this example, the
    // homonids can be highlighted, but the level mesh can't.
    IDFlag_IsHighlightable = 1 << 1
};
};

