#pragma once

#include <irrlicht.h>
#include <iostream>
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
	scene::ISceneNode* quakeLevelNode;

	scene::ITriangleSelector* mapSelector;
	scene::IAnimatedMesh* gunmesh;
		video::SColor backColor;
	scene::IParticleSystemSceneNode* campFire;
	scene::ICameraSceneNode* camera;



};

