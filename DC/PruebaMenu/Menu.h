#pragma once


#include <irrlicht.h>
#include "SoundEffect.h"
#include "Paths.h"
#include "driverChoice.h"
#include "BotonesMenu.h"
#include "GameData.h"
//#include "MyEventReceiver.h"

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#endif


//class MyEventReceiver;

class MyMenu: public IEventReceiver
{
public:
	MyMenu();
	~MyMenu(void);
	int AddMenu(video::E_DRIVER_TYPE&);
	void setStart();
	virtual bool OnEvent(const SEvent& event);
	int get_level();
private:
	int start;
	IrrlichtDevice * device;
	SoundEffect *sound;
	int level;
};

