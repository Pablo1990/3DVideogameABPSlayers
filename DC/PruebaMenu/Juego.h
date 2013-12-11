#pragma once

#include <irrlicht.h>
#include <iostream>
#include "Npc.h"
#include "Sword.h"
#include "Weapon.h"
#include "Player.h"
#include "Bow.h"
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
	scene::IAnimatedMesh* gunmesh;
		video::SColor backColor;
	scene::IParticleSystemSceneNode* campFire;
	scene::ICameraSceneNode* camera;

	scene::IAnimatedMeshSceneNode* gun;
	scene::IAnimatedMeshSceneNode* droppedGun;

	float lastX;
	float firstX;
	float firstY;
	float lastY;
	scene::ISceneNodeAnimator* anim1;

	scene::ISceneNode* skyboxNode;

	scene::IAnimatedMeshSceneNode* model1;
	scene::IAnimatedMeshSceneNode* model2;
	
	bool collision(ISceneNode*, ISceneNode*);
	IBoneSceneNode *model1_head;
	IBoneSceneNode *model1_high_spine;
	IBoneSceneNode *model1_low_spine;
	IBoneSceneNode *model1_pelvis;
	IBoneSceneNode *model1_RThight;
	IBoneSceneNode *model1_RFoot;
	IBoneSceneNode *model1_RCalf;
	IBoneSceneNode *model1_LThight;
	IBoneSceneNode *model1_LFoot;
	IBoneSceneNode *model1_LCalf;
	IBoneSceneNode *model1_RClavicle;
	IBoneSceneNode *model1_RUpperarm;
	IBoneSceneNode *model1_RForearm;
	IBoneSceneNode *model1_RHand;
	IBoneSceneNode *model1_LClavicle;
	IBoneSceneNode *model1_LUpperarm;
	IBoneSceneNode *model1_LForearm;
	IBoneSceneNode *model1_LHand;
	IBoneSceneNode *model1_Neck;
	Npc* npc;
	Player* player;
	Sword* sw;
	Sword* dropped_sword;
	Bow* dropped_bow;
	scene::ITriangleSelector* model1_selector;
	scene::ITriangleSelector* selector;
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

