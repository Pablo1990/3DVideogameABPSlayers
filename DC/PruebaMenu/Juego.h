#pragma once

#include <irrlicht.h>
#include <iostream>
#include "SoundEffect.h"
#include "Paths.h"
#include "Npc.h"
#include "Sword.h"
#include "Weapon.h"

#include "Player.h"
#include "Bow.h"
#include "ThrowableItem.h"
#include "Spear.h"
#include "Pathfinding.h"
#include "Position.h"
#include "Mapa.h";
#include "Hud.h";
#include "Aprendizaje\CController.h"
#include  "Aprendizaje\CParams.h"
#include "GameData.h"
#include "BotonesMenu.h"


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

	Juego(video::E_DRIVER_TYPE d, int w, int h, bool f, float v);
	~Juego(void);
	void run();
	void switchToNextScene();
	virtual bool OnEvent(const SEvent& event);
	void loadSceneData();

	void add_random_item(vector3df position);


	void setEstado(int estado);
	int getEstado();

	void set_level(int lvl);
	void replace_random_item(IrrlichtDevice *device, 	scene::ITriangleSelector* mapSelector);

	void set_weapon(int w);

private:
	void switch_to_next_level();
	static const int KMAX_LEVEL = 4;
	int level;
	bool cntinue;
	int selected_weapon;
	video::E_DRIVER_TYPE driverType;
	IrrlichtDevice *device;
	Hud * hud;
	Pathfinding*  pf;
	s32 sceneStartTime;
	gui::IGUIStaticText* statusText;
	gui::IGUIStaticText* statusText2;
	scene::IQ3LevelMesh* quakeLevelMesh;
	int estado;

	scene::IMetaTriangleSelector* metaSelector;
	scene::IMetaTriangleSelector* metaModelSelector;

	scene::ISceneNode* quakeLevelNode;

	scene::ITriangleSelector* mapSelector;
		video::SColor backColor;
	scene::IParticleSystemSceneNode* campFire;
	scene::IParticleSystemSceneNode* heal_camp;

	scene::ICameraSceneNode* camera;
	ISoundEngine* klang_engine;

	bool paused;


	float lastX;
	float firstX;
	float firstY;
	float lastY;

	scene::ISceneNode* skyboxNode;
	
	Npc* npc;
	Goal_Think* mente;
	Player* player;
	//Sword* sw;
	//Sword* dropped_sword;
	//Bow* dropped_bow;
	//ThrowableItem* dropped_red_shroom;
	//Spear* dropped_spear;
	bool crouch;
	//scene::ITriangleSelector* model1_selector;
	scene::ITriangleSelector* selector;
	scene::ISceneNodeAnimatorCollisionResponse* collider;
	std::list<Weapon*>* armas;
	int last_drop;
	double* types;
	int cycles;
	SoundEffect *sound;
	int win_condition; //0 sigue, 1 ganas, -1 pierdes
	int width;
	int height;
	bool fullscreen;
	float volume;
	s32 time_store;
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

