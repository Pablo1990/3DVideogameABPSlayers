#include <irrlicht.h>
#include "BotonesMenu.h"
#include "Menu.h"

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#endif

int main()
{
	MyMenu *menu = new MyMenu();


	return menu->AddMenu();

}