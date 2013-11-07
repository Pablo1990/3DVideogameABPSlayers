#include "MyEventReceiver.h"
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




MyEventReceiver::MyEventReceiver(SAppContext & context,const MyMenu &m) : Context(context)
{
}


MyEventReceiver::~MyEventReceiver(void)
{
}

bool MyEventReceiver::OnEvent(const SEvent& event)
{
    if (event.EventType == EET_GUI_EVENT)
    {
        s32 id = event.GUIEvent.Caller->getID();
        IGUIEnvironment* env = Context.device->getGUIEnvironment();

        switch(event.GUIEvent.EventType)
        {
			case EGET_BUTTON_CLICKED:

            switch(id)
            {


				case GUI_ID_QUIT_BUTTON:
					Context.device->closeDevice();
					return true;

				case GUI_ID_VOLVER_BUTTON:
					env->clear();

					env->addButton(rect<s32>(500,150,600,150 + 32), 0, GUI_ID_JUGAR_BUTTON,	L"Jugar", L"Comienza el juego");
					env->addButton(rect<s32>(500,190,600,190 + 32), 0, GUI_ID_INVENTARIO_BUTTON, L"Inventario", L"Echale un ojo a tus objetos");
					env->addButton(rect<s32>(500,230,600,230 + 32), 0, GUI_ID_EDITOR_BUTTON, L"Editor personajes", L"Modela tus personajes");
					env->addButton(rect<s32>(500,270,600,270 + 32), 0, GUI_ID_OPCIONES_BUTTON, L"Opciones", L"Configura el juego");
					env->addButton(rect<s32>(500,310,600,310 + 32), 0, GUI_ID_QUIT_BUTTON, L"Quit", L"Sal del juego");

   					env->addStaticText(L"AQUI VA UNA IMAGEN", rect<s32>(50,110,250,130), true);

					return true;

					break;
				case GUI_ID_JUGAR_BUTTON:
					env->clear();

					env->addButton(rect<s32>(500,150,600,150 + 32), 0, GUI_ID_1VS1_BUTTON, L"1vs1", L"Juega en solitario");
					env->addButton(rect<s32>(500,190,600,190 + 32), 0, GUI_ID_EQUIPO_BUTTON, L"Equipo", L"Juega como miembro de equipo");
					env->addButton(rect<s32>(500,310,600,310 + 32), 0, GUI_ID_VOLVER_BUTTON, L"Inicio", L"Menu Anterior");

					return true;

				case GUI_ID_PRINCIPAL_BUTTON:
					env->clear();

					env->addButton(rect<s32>(500,150,600,150 + 32), 0, GUI_ID_JUGAR_BUTTON, L"Jugar", L"Comienza el juego");
					env->addButton(rect<s32>(500,190,600,190 + 32), 0, GUI_ID_INVENTARIO_BUTTON, L"Inventario", L"Echale un ojo a tus objetos");
					env->addButton(rect<s32>(500,230,600,230 + 32), 0, GUI_ID_EDITOR_BUTTON, L"Editor personajes", L"Modela tus personajes");
					env->addButton(rect<s32>(500,270,600,270 + 32), 0, GUI_ID_OPCIONES_BUTTON, L"Opciones", L"Configura el juego");
					env->addButton(rect<s32>(500,310,600,310 + 32), 0, GUI_ID_QUIT_BUTTON, L"Quit", L"Sal del juego");

					env->addStaticText(L"AQUI VA UNA IMAGEN", rect<s32>(50,110,250,130), true);


					return true;
				case GUI_ID_1VS1_BUTTON:
					env->clear();

					env->addStaticText(L"Destino de caballero 1vs1 Nivel 1", rect<s32>(50,110,250,130), true);
					env->addButton(rect<s32>(500,150,600,150 + 32), 0, GUI_ID_CONTINUAR_BUTTON, L"Continuar", L"Continua desde el ultimo nivel desbloqueado");
					env->addButton(rect<s32>(500,190,600,190 + 32), 0, GUI_ID_NUEVA_PARTIDA_BUTTON, L"Nueva Partida", L"Comienza de nuevo");
					env->addButton(rect<s32>(500,310,600,310 + 32), 0, GUI_ID_VOLVER_BUTTON, L"Inicio", L"Menu Inicio");

					return true;

				case GUI_ID_EQUIPO_BUTTON:
					env->clear();
					env->addStaticText(L"PANTALLA POR EQUIPOS", rect<s32>(50,110,250,130), true);
					env->addButton(rect<s32>(500,310,600,310 + 32), 0, GUI_ID_VOLVER_BUTTON, L"Inicio", L"Menu inicio");

					return true;

				case GUI_ID_CONTINUAR_BUTTON:
					env->clear();
					env->addStaticText(L"PANTALLA DE JUEGO", rect<s32>(50,110,250,130), true);

					env->addButton(rect<s32>(500,310,600,310 + 32), 0, GUI_ID_VOLVER_BUTTON, L"Inicio", L"Menu inicio");

					return true;

				case GUI_ID_NUEVA_PARTIDA_BUTTON:
					env->clear();
					env->addStaticText(L"PANTALLA DE JUEGO", rect<s32>(50,110,250,130), true);

					env->addButton(rect<s32>(500,310,600,310 + 32), 0, GUI_ID_VOLVER_BUTTON, L"Inicio", L"Menu inicio");

					return true;

				case GUI_ID_EDITOR_BUTTON:
					env->clear();
					env->addStaticText(L"PANTALLA DEL EDITOR", rect<s32>(50,110,250,130), true);

					env->addButton(rect<s32>(500,310,600,310 + 32), 0, GUI_ID_VOLVER_BUTTON, L"Inicio", L"Menu inicio");

					return true;

				case GUI_ID_OPCIONES_BUTTON:
					env->clear();
					env->addStaticText(L"PANTALLA DE OPCIONES", rect<s32>(50,110,250,130), true);

					env->addButton(rect<s32>(500,310,600,310 + 32), 0, GUI_ID_VOLVER_BUTTON, L"Inicio", L"Menu inicio");

					return true;

				case GUI_ID_INVENTARIO_BUTTON:
					env->clear();
					env->addStaticText(L"PANTALLA DE INVENTARIO", rect<s32>(50,110,250,130), true);

					env->addButton(rect<s32>(500,310,600,310 + 32), 0, GUI_ID_VOLVER_BUTTON, L"Inicio", L"Menu inicio");

					return true;
				default:
					return false;
			}
            break;

		

       
        default:
            break;
        }
    }

    return false;
}


