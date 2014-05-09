//#include <vld.h>
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

	// ask user for driver
	E_DRIVER_TYPE driverType = driverChoiceConsole();

    if (driverType==video::EDT_COUNT)
        return false;

	MyMenu *menu = new MyMenu(driverType);

	int estado=menu->AddMenu();
	if(estado==1 || estado == 4)
	{
		Juego game(driverType, menu->get_width(), menu->get_height(), menu->get_fullscreen(), menu->get_volume());
		game.setEstado(estado);
		game.set_level(menu->get_level());
		game.run();
	}
	//Estado aprendizaje

	else if(estado==2)
	{
		Juego game(driverType, menu->get_width(), menu->get_height(), menu->get_fullscreen(), menu->get_volume());
		game.setEstado(2);
		game.run();
	}
	else if(estado==3)
	{
		Juego game(driverType, menu->get_width(), menu->get_height(), menu->get_fullscreen(), menu->get_volume());
		game.setEstado(3);
		game.run();
	}
	if(menu)
		delete menu;
}