#include "Menu.h"

MyMenu::MyMenu()
{
	this->start = false;
}

MyMenu::~MyMenu(void)
{
}

bool MyMenu::AddMenu(video::E_DRIVER_TYPE &driverType)
{
	// ask user for driver
    driverType = driverChoiceConsole();
    if (driverType==video::EDT_COUNT)
        return false;


	core::dimension2d<u32> resolution ( 1366, 768 );

    // create device and exit if creation failed
	irr::SIrrlichtCreationParameters params;
	params.DriverType=driverType;
	params.WindowSize=resolution;
	params.Bits=32;
	params.Fullscreen=true;
	


    device = createDeviceEx(params);

    if (device == 0)
        return 1; // could not create selected driver.

	 device->setWindowCaption(L"MENU");
    device->setResizable(true);

    video::IVideoDriver* driver = device->getVideoDriver();
    IGUIEnvironment* env = device->getGUIEnvironment();

	 
 

	IGUISkin* skin = env->getSkin();
   gui::IGUIFont* font2 =env->getFont("../media/fontcourier.bmp");
   skin->setFont(font2);


	env->addButton(rect<s32>(700,150,1000,150 + 82), 0, GUI_ID_JUGAR_BUTTON,
            L"Jugar", L"Comienza el juego");

	env->addButton(rect<s32>(700,260,1000,260 + 82), 0, GUI_ID_INVENTARIO_BUTTON,
            L"Inventario", L"Echale un ojo a tus objetos");

	env->addButton(rect<s32>(700,370,1000,370 + 82), 0, GUI_ID_EDITOR_BUTTON,
            L"Editor personajes", L"Modela tus personajes");

	env->addButton(rect<s32>(700,480,1000,480 + 82), 0, GUI_ID_OPCIONES_BUTTON,
            L"Opciones", L"Configura el juego");

	env->addButton(rect<s32>(700,590,1000,590 + 82), 0, GUI_ID_QUIT_BUTTON,
            L"Quit", L"Sal del juego");

	
	

	video::ITexture* images = driver->getTexture("../media/Titulo1.png");
	
    // Store the appropriate data in a context structure.
    
    

    // Then create the event receiver, giving it that context structure.
    //MyEventReceiver receiver(context, *this);

    // And tell the device to use our custom event receiver.
    device->setEventReceiver(this);

	video::ITexture* irrlichtBack = driver->getTexture("../Imagenes/demoback.jpg");


	while(device->run() && driver && start == false)
	{
		if (device->isWindowActive())
		{
			driver->beginScene(true, true, SColor(0,200,200,200));
			if (irrlichtBack)
				driver->draw2DImage(irrlichtBack,
						core::position2d<int>(0,0));
			driver->draw2DImage(images,
						core::position2d<int>(80,100));
			env->drawAll();
    
			driver->endScene();

		}
	}
    device->drop();

	return start;
}


void MyMenu::setStart()
{
	this->start = true;
}


bool MyMenu::OnEvent(const SEvent& event)
{
    if (event.EventType == EET_GUI_EVENT)
    {
        s32 id = event.GUIEvent.Caller->getID();
        IGUIEnvironment* env = device->getGUIEnvironment();

        switch(event.GUIEvent.EventType)
        {
			case EGET_BUTTON_CLICKED:

            switch(id)
            {


				case GUI_ID_QUIT_BUTTON:
					device->closeDevice();
					start = true;
					return false;

				case GUI_ID_VOLVER_BUTTON:
					env->clear();

				
				env->addButton(rect<s32>(700,150,1000,150 + 82), 0, GUI_ID_JUGAR_BUTTON,
            L"Jugar", L"Comienza el juego");
				

	env->addButton(rect<s32>(700,260,1000,260 + 82), 0, GUI_ID_INVENTARIO_BUTTON,
            L"Inventario", L"Echale un ojo a tus objetos");

	env->addButton(rect<s32>(700,370,1000,370 + 82), 0, GUI_ID_EDITOR_BUTTON,
            L"Editor personajes", L"Modela tus personajes");

	env->addButton(rect<s32>(700,480,1000,480 + 82), 0, GUI_ID_OPCIONES_BUTTON,
            L"Opciones", L"Configura el juego");

	env->addButton(rect<s32>(700,590,1000,590 + 82), 0, GUI_ID_QUIT_BUTTON,
            L"Quit", L"Sal del juego");
					
   					

					return true;

					break;
				case GUI_ID_JUGAR_BUTTON:
					env->clear();

					env->addButton(rect<s32>(700,150,1000,150 + 82), 0, GUI_ID_1VS1_BUTTON, L"1vs1", L"Juega en solitario");
					env->addButton(rect<s32>(700,260,1000,260 + 82), 0, GUI_ID_EQUIPO_BUTTON, L"Equipo", L"Juega como miembro de equipo");
					env->addButton(rect<s32>(700,370,1000,370 + 82), 0, GUI_ID_VOLVER_BUTTON, L"Inicio", L"Menu Anterior");

					return true;

				case GUI_ID_PRINCIPAL_BUTTON:
					env->clear();

					env->addButton(rect<s32>(700,150,1000,150 + 82), 0, GUI_ID_JUGAR_BUTTON,L"Jugar", L"Comienza el juego");
					env->addButton(rect<s32>(700,260,1000,260 + 82), 0, GUI_ID_INVENTARIO_BUTTON,  L"Inventario", L"Echale un ojo a tus objetos");
					env->addButton(rect<s32>(700,370,1000,370 + 82), 0, GUI_ID_EDITOR_BUTTON, L"Editor personajes", L"Modela tus personajes");
					env->addButton(rect<s32>(700,480,1000,480 + 82), 0, GUI_ID_OPCIONES_BUTTON, L"Opciones", L"Configura el juego");
					env->addButton(rect<s32>(700,590,1000,590 + 82), 0, GUI_ID_QUIT_BUTTON, L"Quit", L"Sal del juego");

					


					return true;
				case GUI_ID_1VS1_BUTTON:
					env->clear();

				
					env->addButton(rect<s32>(700,150,1000,150 + 82), 0, GUI_ID_CONTINUAR_BUTTON, L"Continuar", L"Continua desde el ultimo nivel desbloqueado");
					env->addButton(rect<s32>(700,260,1000,260 + 82), 0, GUI_ID_NUEVA_PARTIDA_BUTTON, L"Nueva Partida", L"Comienza de nuevo");
					env->addButton(rect<s32>(700,370,1000,370 + 82), 0, GUI_ID_VOLVER_BUTTON, L"Inicio", L"Menu Inicio");

					return true;

				

				case GUI_ID_EQUIPO_BUTTON:
					env->clear();
						env->addButton(rect<s32>(700,260,1000,260 + 82), 0, GUI_ID_NUEVA_PARTIDA_BUTTON, L"Nueva Partida", L"Comienza de nuevo");
					env->addButton(rect<s32>(700,370,1000,370 + 82), 0, GUI_ID_VOLVER_BUTTON, L"Inicio", L"Menu inicio");

					return true;

				case GUI_ID_CONTINUAR_BUTTON:
					env->clear();
				
					env->addButton(rect<s32>(700,310,1000,310 + 82), 0, GUI_ID_VOLVER_BUTTON, L"Inicio", L"Menu inicio");

					return true;

				case GUI_ID_NUEVA_PARTIDA_BUTTON:
					device->closeDevice();
					//menu->setStart(true);
					start = true;
					return true;

				case GUI_ID_EDITOR_BUTTON:
					env->clear();
					

					env->addButton(rect<s32>(700,310,1000,310 + 82), 0, GUI_ID_VOLVER_BUTTON, L"Inicio", L"Menu inicio");

					return true;

				case GUI_ID_OPCIONES_BUTTON:
					env->clear();
					
					env->addButton(rect<s32>(700,310,1000,310 + 82), 0, GUI_ID_VOLVER_BUTTON, L"Inicio", L"Menu inicio");

					return true;

				case GUI_ID_INVENTARIO_BUTTON:
					env->clear();
					

					env->addButton(rect<s32>(700,310,1000,310 + 82), 0, GUI_ID_VOLVER_BUTTON, L"Inicio", L"Menu inicio");

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
