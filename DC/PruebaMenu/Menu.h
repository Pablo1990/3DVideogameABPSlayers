#pragma once


#include <irrlicht.h>
#include "BotonesMenu.h"
#include "MyEventReceiver.h"

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#endif



class MyEventReceiver;

class MyMenu
{
public:
	MyMenu(void);
	~MyMenu(void);
	int AddMenu();

private:
	IGUIButton* botones[5];
};

