#pragma once


#include <irrlicht.h>
#include <irrKlang.h>
#include "Paths.h"
#include "driverChoice.h"
#include "BotonesMenu.h"
//#include "MyEventReceiver.h"

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
using namespace irrklang;


#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#endif
#pragma comment(lib, "irrKlang.lib") 


//class MyEventReceiver;

class MyMenu: public IEventReceiver
{
public:
	MyMenu();
	~MyMenu(void);
	int AddMenu(video::E_DRIVER_TYPE&);
	void setStart();
	virtual bool OnEvent(const SEvent& event);
private:
	int start;
	IrrlichtDevice * device;
	ISoundEngine* klang_engine;
};

