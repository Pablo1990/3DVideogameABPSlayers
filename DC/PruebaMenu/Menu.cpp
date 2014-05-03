#include "Menu.h"

MyMenu::MyMenu()
{
	this->start = false;
	GameData gd;
	this->level = gd.load_game();
}

MyMenu::~MyMenu(void)
{
	if(sound)
	{
		delete sound;
		sound = 0;
	}
}

int MyMenu::AddMenu(video::E_DRIVER_TYPE &driverType)
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
	
	if(!sound)
		sound = new SoundEffect(menu_music_path);
	
	device->setWindowCaption(L"MENU");
    device->setResizable(true);

    video::IVideoDriver* driver = device->getVideoDriver();
    IGUIEnvironment* env = device->getGUIEnvironment();

	 
	

	IGUISkin* skin = env->getSkin();
   gui::IGUIFont* font2 =env->getFont("../media/fuente1.png");
   skin->setFont(font2);
   skin->setColor(EGDC_3D_SHADOW  , video::SColor(25,210,50,0));
	skin->setColor(EGDC_3D_FACE  , video::SColor(70,215,0,15));
	

	env->addButton(rect<s32>(800,150,1100,150 + 82), 0, GUI_ID_JUGAR_BUTTON,
            L"Jugar", L"Comienza el juego");

	env->addButton(rect<s32>(800,260,1100,260 + 82), 0, GUI_ID_INVENTARIO_BUTTON,
            L"Inventario", L"Echale un ojo a tus objetos");

	env->addButton(rect<s32>(800,370,1100,370 + 82), 0, GUI_ID_EDITOR_BUTTON,
            L"Editor personajes", L"Modela tus personajes");

	env->addButton(rect<s32>(800,480,1100,480 + 82), 0, GUI_ID_OPCIONES_BUTTON,
            L"Opciones", L"Configura el juego");

	env->addButton(rect<s32>(800,590,1100,590 + 82), 0, GUI_ID_QUIT_BUTTON,
            L"Quit", L"Sal del juego");

	
	
			


	
    // Store the appropriate data in a context structure.
    
    

    // Then create the event receiver, giving it that context structure.
    //MyEventReceiver receiver(context, *this);

    // And tell the device to use our custom event receiver.
    device->setEventReceiver(this);

	video::ITexture* irrlichtBack = driver->getTexture("../Imagenes/demoback1.png");

	sound->play_background();
	while(device->run() && driver && start == false)
	{
		if (device->isWindowActive())
		{
			driver->beginScene(true, true, SColor(0,200,200,200));
			if (irrlichtBack)
				driver->draw2DImage(irrlichtBack,
						core::position2d<int>(0,0));
	
			
			env->drawAll();
    
			driver->endScene();

		}
	}
    device->drop();

	sound->stop_all_sounds();
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
					start = false;
					return false;

				case GUI_ID_VOLVER_BUTTON:
					env->clear();

				
				env->addButton(rect<s32>(800,150,1100,150 + 82), 0, GUI_ID_JUGAR_BUTTON,
            L"Jugar", L"Comienza el juego");
				

	env->addButton(rect<s32>(800,260,1100,260 + 82), 0, GUI_ID_INVENTARIO_BUTTON,
            L"Inventario", L"Echale un ojo a tus objetos");

	env->addButton(rect<s32>(800,370,1100,370 + 82), 0, GUI_ID_EDITOR_BUTTON,
            L"Editor personajes", L"Modela tus personajes");

	env->addButton(rect<s32>(800,480,1100,480 + 82), 0, GUI_ID_OPCIONES_BUTTON,
            L"Opciones", L"Configura el juego");

	env->addButton(rect<s32>(800,590,1100,590 + 82), 0, GUI_ID_QUIT_BUTTON,
            L"Quit", L"Sal del juego");
					
   					

					return true;

					break;
				case GUI_ID_JUGAR_BUTTON:
					env->clear();

					env->addButton(rect<s32>(800,150,1100,150 + 82), 0, GUI_ID_1VS1_BUTTON, L"1vs1", L"Juega en solitario");
					env->addButton(rect<s32>(800,260,1100,260 + 82), 0, GUI_ID_EQUIPO_BUTTON, L"Equipo", L"Juega como miembro de equipo");
					env->addButton(rect<s32>(800,370,1100,370 + 82), 0, GUI_ID_VOLVER_BUTTON, L"Inicio", L"Menu Anterior");

					return true;

				case GUI_ID_PRINCIPAL_BUTTON:
					env->clear();

					env->addButton(rect<s32>(800,150,1100,150 + 82), 0, GUI_ID_JUGAR_BUTTON,L"Jugar", L"Comienza el juego");
					env->addButton(rect<s32>(800,260,1100,260 + 82), 0, GUI_ID_INVENTARIO_BUTTON,  L"Inventario", L"Echale un ojo a tus objetos");
					env->addButton(rect<s32>(800,370,1100,370 + 82), 0, GUI_ID_EDITOR_BUTTON, L"Editor personajes", L"Modela tus personajes");
					env->addButton(rect<s32>(800,480,1100,480 + 82), 0, GUI_ID_OPCIONES_BUTTON, L"Opciones", L"Configura el juego");
					env->addButton(rect<s32>(800,590,1100,590 + 82), 0, GUI_ID_QUIT_BUTTON, L"Quit", L"Sal del juego");

					


					return true;
				case GUI_ID_1VS1_BUTTON:
					env->clear();

				
					env->addButton(rect<s32>(800,150,1100,150 + 82), 0, GUI_ID_CONTINUAR_BUTTON, L"Continuar", L"Continua desde el ultimo nivel desbloqueado");
					env->addButton(rect<s32>(800,260,1100,260 + 82), 0, GUI_ID_NUEVA_PARTIDA_BUTTON, L"Nueva Partida", L"Comienza de nuevo");
					env->addButton(rect<s32>(800,370,1100,370 + 82), 0, GUI_ID_APRENDIZAJE, L"Aprendizaje", L"Ejecutar Aprendizaje");
					env->addButton(rect<s32>(800,480,1100,480 + 82), 0, GUI_ID_JAPRENDIZAJE, L"Juego Con Aprendizaje", L"Ejecutar JAprendizaje");
					env->addButton(rect<s32>(800,590,1100,590 + 82), 0, GUI_ID_VOLVER_BUTTON, L"Inicio", L"Menu Inicio");
		
					return true;

				

				case GUI_ID_EQUIPO_BUTTON:
					env->clear();
						env->addButton(rect<s32>(800,260,1100,260 + 82), 0, GUI_ID_NUEVA_PARTIDA_BUTTON, L"Nueva Partida", L"Comienza de nuevo");
					env->addButton(rect<s32>(800,370,1100,370 + 82), 0, GUI_ID_VOLVER_BUTTON, L"Inicio", L"Menu inicio");

					return true;

				case GUI_ID_CONTINUAR_BUTTON:
					env->clear();
					start = 4;

					return true;

				case GUI_ID_NUEVA_PARTIDA_BUTTON:
					device->closeDevice();
					//menu->setStart(true);
					this->level = 0;
					start = 1;
					return true;

				case GUI_ID_APRENDIZAJE:
					device->closeDevice();
					//menu->setStart(true);
					start = 2;
					return true;

				case GUI_ID_JAPRENDIZAJE:
					device->closeDevice();
					//menu->setStart(true);
					start = 3;
					return true;

				case GUI_ID_EDITOR_BUTTON:
					env->clear();
					

					env->addButton(rect<s32>(800,310,1100,310 + 82), 0, GUI_ID_VOLVER_BUTTON, L"Inicio", L"Menu inicio");

					return true;

				case GUI_ID_OPCIONES_BUTTON:
					env->clear();
					
					env->addButton(rect<s32>(800,310,1100,310 + 82), 0, GUI_ID_VOLVER_BUTTON, L"Inicio", L"Menu inicio");

					return true;

				case GUI_ID_INVENTARIO_BUTTON:
					env->clear();
					

					env->addButton(rect<s32>(800,310,1100,310 + 82), 0, GUI_ID_VOLVER_BUTTON, L"Inicio", L"Menu inicio");

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

int MyMenu::get_level()
{
	return this->level;
}