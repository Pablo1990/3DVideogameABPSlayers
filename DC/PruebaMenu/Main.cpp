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
	E_DRIVER_TYPE driverType = video::EDT_OPENGL;//driverChoiceConsole();

    if (driverType==video::EDT_COUNT)
        return false;

	MyMenu *menu ;//= new MyMenu(driverType);

	
	int estado = 0;//Para que entre
	int widht;
	int height;
	bool fullscreen;
	float volume;
	int level;
	int weapon;

	while(estado != -1)
	{
		menu = new MyMenu(driverType);
		estado=menu->AddMenu();
		widht = menu->get_width();
		height = menu->get_height();
		fullscreen = menu->get_fullscreen();
		volume = menu->get_volume();
		level = menu->get_level();
		weapon = menu->get_weapon();

		if(menu)
		{
			menu = NULL;
			delete menu;
		}
		if(estado==1 || estado == 4)
		{
			Juego game(driverType, widht, height, fullscreen, volume);
			game.setEstado(estado);
			game.set_level(level);
			game.set_weapon(weapon);
			game.run();
			estado = game.getEstado();
		}

		//Estado aprendizaje
		else if(estado==2)
		{
			Juego game(driverType, widht, height, fullscreen, volume);
			game.setEstado(2);
			game.run();
			estado = game.getEstado();
		}
		else if(estado==3)
		{
			Juego game(driverType, widht, height, fullscreen, volume);
			game.setEstado(3);
			game.run();
			estado = game.getEstado();
		}
	}
	if(menu)
		delete menu;
}