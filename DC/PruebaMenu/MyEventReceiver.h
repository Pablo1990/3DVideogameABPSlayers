

#pragma once


#include <irrlicht.h>
#include "driverChoice.h"
#include "BotonesMenu.h"
#include "Menu.h"
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

class MyMenu;

class MyEventReceiver : public IEventReceiver
{
public:

	MyEventReceiver(SAppContext&, const MyMenu m) ;
	~MyEventReceiver(void);
	virtual bool OnEvent(const SEvent& event);

	// Declare a structure to hold some context for the event receiver so that it
	// has it available inside its OnEvent() method.
	

	// Define some values that we'll use to identify individual GUI controls.
	
private:
	SAppContext & Context;
	s32 ant;
	MyMenu *menu;

};

