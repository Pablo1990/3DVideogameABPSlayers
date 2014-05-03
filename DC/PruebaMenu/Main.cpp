#include <vld.h>
#include <irrlicht.h>
#include "BotonesMenu.h"
#include "Menu.h"
#include "Juego.h"

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
	video::E_DRIVER_TYPE driverType;
	int estado=menu->AddMenu(driverType);
	if(estado==1 || estado == 4)
	{
		Juego game(driverType);
		game.setEstado(estado);
		game.set_level(menu->get_level());
		game.run();
	}
	//Estado aprendizaje

	else if(estado==2)
	{
		Juego game(driverType);
		game.setEstado(2);
		game.run();
	}
	else if(estado==3)
	{
		Juego game(driverType);
		game.setEstado(3);
		game.run();
	}

	delete menu;
}