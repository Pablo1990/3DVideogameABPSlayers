#pragma once


#include <irrlicht.h>
#include "driverChoice.h"
#include "BotonesMenu.h"
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
	bool AddMenu(video::E_DRIVER_TYPE&);
	void setStart();
	virtual bool OnEvent(const SEvent& event);
private:
	bool start;
	IrrlichtDevice * device;
};

