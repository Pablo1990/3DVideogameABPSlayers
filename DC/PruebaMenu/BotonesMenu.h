#pragma once
#include <irrlicht.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#endif

struct SAppContext
{
	IrrlichtDevice *device;
	s32             counter;
	IGUIListBox*    listbox;
};

enum botones
{
	GUI_ID_QUIT_BUTTON = 101,
	GUI_ID_NEW_WINDOW_BUTTON,
	GUI_ID_FILE_OPEN_BUTTON,
	GUI_ID_TRANSPARENCY_SCROLL_BAR,
	GUI_ID_JUGAR_BUTTON,
	GUI_ID_INVENTARIO_BUTTON,
	GUI_ID_EDITOR_BUTTON,
	GUI_ID_OPCIONES_BUTTON,
	GUI_ID_1VS1_BUTTON,
	GUI_ID_EQUIPO_BUTTON,
	GUI_ID_CONTINUAR_BUTTON,
	GUI_ID_NUEVA_PARTIDA_BUTTON,
	GUI_ID_VOLVER_BUTTON,
	GUI_ID_PRINCIPAL_BUTTON,
	GUI_ID_APRENDIZAJE,
	GUI_ID_JAPRENDIZAJE, 
	GUI_ID_VOLUME_SCROLLBAR,
	GUI_ID_RESOLUTION_COMBOBOX
};