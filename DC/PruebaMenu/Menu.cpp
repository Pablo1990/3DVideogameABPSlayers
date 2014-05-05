#include "Menu.h"


MyMenu::MyMenu()
{
	this->start = false;
	GameData gd;
	this->level = gd.load_game();
	first_rect = rect<s32>(470,115,647,178);
	second_rect = rect<s32>(470,200,647,263);
	third_rect = rect<s32>(470,285,647,348);
	fourth_rect = rect<s32>(470,370,647,433);
	fifth_rect = rect<s32>(470,455,647,518);
	scrollbar_rect = rect<s32>(470,115,647,130);
	combobox_rect = rect<s32>(470,150,647,165);
	resize = false;

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
	
	//Creamos los rectangulos base para los botones, por defecto para res 800x600 luego se hace el escalado en funcion
	// de la resolucion


	// ask user for driver
    driverType = driverChoiceConsole();
	dt = driverType;
    if (driverType==video::EDT_COUNT)
        return false;


	core::dimension2d<u32> resolution (/* 800,600*/1366, 768 );
    // create device and exit if creation failed
	irr::SIrrlichtCreationParameters params;
	params.DriverType=driverType;
	params.WindowSize=resolution;
	params.Bits=32;
	params.Fullscreen=true;
	


    device = createDeviceEx(params);

    if (device == 0)
        return 1; // could not create selected driver.
	
	gh = GUIHandler(device);

	if(!sound)
		sound = new SoundEffect(menu_music_path);
	
	device->setWindowCaption(L"MENU");
    device->setResizable(true);

    video::IVideoDriver* driver = device->getVideoDriver();
	
	IGUIEnvironment* env = device->getGUIEnvironment();

	 
	

	IGUISkin* skin = env->getSkin();

   gui::IGUIFont* font2 =env->getFont("../media/fuente1.png");

   if(gh.get_scale_x() < 1.2)
   {
	   if(font2)
		   font2->drop();
	   font2 = env->getFont("../media/fontcourier.bmp");
   }


   skin->setFont(font2);
   skin->setColor(EGDC_3D_SHADOW  , video::SColor(25,210,50,0));
	skin->setColor(EGDC_3D_FACE  , video::SColor(70,215,0,15));



	env->addButton(gh.ScaleValuebyScreenHeight(first_rect.UpperLeftCorner, first_rect.LowerRightCorner), 0, GUI_ID_JUGAR_BUTTON,
            L"Jugar", L"Comienza el juego");

	env->addButton(gh.ScaleValuebyScreenHeight(second_rect.UpperLeftCorner, second_rect.LowerRightCorner), 0, GUI_ID_INVENTARIO_BUTTON,
            L"Inventario", L"Echale un ojo a tus objetos");

	env->addButton(gh.ScaleValuebyScreenHeight(third_rect.UpperLeftCorner, third_rect.LowerRightCorner), 0, GUI_ID_EDITOR_BUTTON,
            L"Editor personajes", L"Modela tus personajes");

	env->addButton(gh.ScaleValuebyScreenHeight(fourth_rect.UpperLeftCorner, fourth_rect.LowerRightCorner), 0, GUI_ID_OPCIONES_BUTTON,
            L"Opciones", L"Configura el juego");

	env->addButton(gh.ScaleValuebyScreenHeight(fifth_rect.UpperLeftCorner, fifth_rect.LowerRightCorner), 0, GUI_ID_QUIT_BUTTON,
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
		if(resize)
		{
			core::dimension2d<u32> resolution (height, width );
			// create device and exit if creation failed
			params.DriverType=driverType;
			params.WindowSize=resolution;
			params.Bits=32;
			params.Fullscreen=true;
	


			device = createDeviceEx(params);
			gh.calculate_scale(device);

			device->setWindowCaption(L"MENU");
			device->setResizable(true);

			driver = device->getVideoDriver();
	
			env = device->getGUIEnvironment();

	 
	

			skin = env->getSkin();

			font2 =env->getFont("../media/fuente1.png");

			if(gh.get_scale_x() < 1.2)
			{
				if(font2)
					font2->drop();
				font2 = env->getFont("../media/fontcourier.bmp");
			}


			skin->setFont(font2);
			skin->setColor(EGDC_3D_SHADOW  , video::SColor(25,210,50,0));
			skin->setColor(EGDC_3D_FACE  , video::SColor(70,215,0,15));

			env->clear();

			volume_control = env->addScrollBar(true, gh.ScaleValuebyScreenHeight(scrollbar_rect.UpperLeftCorner, scrollbar_rect.LowerRightCorner),0,GUI_ID_VOLUME_SCROLLBAR);
			volume_control->setMax(100);
			volume_control->setPos(sound->get_volume() * 100);

			res_control = env->addComboBox(gh.ScaleValuebyScreenHeight(combobox_rect.UpperLeftCorner, combobox_rect.LowerRightCorner), 0, GUI_ID_RESOLUTION_COMBOBOX);
			res_control->addItem(L"800x600");
			res_control->addItem(L"1024x768");
			res_control->addItem(L"1280x768");
			res_control->addItem(L"1280x960");
			res_control->addItem(L"1366x768");
			res_control->addItem(L"1600x1200");

			env->addButton(gh.ScaleValuebyScreenHeight(second_rect.UpperLeftCorner, second_rect.LowerRightCorner), 0, GUI_ID_VOLVER_BUTTON, L"Inicio", L"Menu inicio");

			device->setEventReceiver(this);

			irrlichtBack = driver->getTexture("../Imagenes/demoback1.png");
			resize = false;
		}

		if (device->isWindowActive())
		{
			driver->beginScene(true, true, SColor(0,200,200,200));
			if (irrlichtBack)
				driver->draw2DImage(irrlichtBack,
						core::position2d<int>(0,0));
	
			
			device->getGUIEnvironment()->drawAll();
    
			driver->endScene();

		}
	}
    device->drop();

	sound->stop_all_sounds();
	return start;
}

void MyMenu::change_device(int h, int w)
{
	core::dimension2d<u32> resolution ( /*800,600*/1366, 768 );
	// create device and exit if creation failed
	irr::SIrrlichtCreationParameters params;
	params.DriverType=dt;
	params.WindowSize=resolution;
	params.Bits=32;
	params.Fullscreen=true;
	
	
	device = createDeviceEx(params);

	IGUIEnvironment* env = device->getGUIEnvironment();
	
	IGUISkin* skin = env->getSkin();

	gui::IGUIFont* font2 =env->getFont("../media/fuente1.png");

	if(gh.get_scale_x() < 1.2)
	{
		if(font2)
		font2->drop();
		font2 = env->getFont("../media/fontcourier.bmp");
	}


	skin->setFont(font2);
	skin->setColor(EGDC_3D_SHADOW  , video::SColor(25,210,50,0));
	skin->setColor(EGDC_3D_FACE  , video::SColor(70,215,0,15));
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

					env->addButton(gh.ScaleValuebyScreenHeight(first_rect.UpperLeftCorner, first_rect.LowerRightCorner), 0, GUI_ID_JUGAR_BUTTON,
							L"Jugar", L"Comienza el juego");
					env->addButton(gh.ScaleValuebyScreenHeight(second_rect.UpperLeftCorner, second_rect.LowerRightCorner), 0, GUI_ID_INVENTARIO_BUTTON,
							L"Inventario", L"Echale un ojo a tus objetos");
					env->addButton(gh.ScaleValuebyScreenHeight(third_rect.UpperLeftCorner, third_rect.LowerRightCorner), 0, GUI_ID_EDITOR_BUTTON,
							L"Editor personajes", L"Modela tus personajes");
					env->addButton(gh.ScaleValuebyScreenHeight(fourth_rect.UpperLeftCorner, fourth_rect.LowerRightCorner), 0, GUI_ID_OPCIONES_BUTTON,
							L"Opciones", L"Configura el juego");
					env->addButton(gh.ScaleValuebyScreenHeight(fifth_rect.UpperLeftCorner, fifth_rect.LowerRightCorner), 0, GUI_ID_QUIT_BUTTON,
							L"Quit", L"Sal del juego");			

					return true;

					break;
				case GUI_ID_JUGAR_BUTTON:
					env->clear();

					env->addButton(gh.ScaleValuebyScreenHeight(first_rect.UpperLeftCorner, first_rect.LowerRightCorner), 0, GUI_ID_1VS1_BUTTON, L"1vs1", L"Juega en solitario");
					env->addButton(gh.ScaleValuebyScreenHeight(second_rect.UpperLeftCorner, second_rect.LowerRightCorner), 0, GUI_ID_EQUIPO_BUTTON, L"Equipo", L"Juega como miembro de equipo");
					env->addButton(gh.ScaleValuebyScreenHeight(third_rect.UpperLeftCorner, third_rect.LowerRightCorner), 0, GUI_ID_VOLVER_BUTTON, L"Inicio", L"Menu Anterior");

					return true;

				case GUI_ID_PRINCIPAL_BUTTON:
					env->clear();

					env->addButton(gh.ScaleValuebyScreenHeight(first_rect.UpperLeftCorner, first_rect.LowerRightCorner), 0, GUI_ID_JUGAR_BUTTON,L"Jugar", L"Comienza el juego");
					env->addButton(gh.ScaleValuebyScreenHeight(second_rect.UpperLeftCorner, second_rect.LowerRightCorner), 0, GUI_ID_INVENTARIO_BUTTON,  L"Inventario", L"Echale un ojo a tus objetos");
					env->addButton(gh.ScaleValuebyScreenHeight(third_rect.UpperLeftCorner, third_rect.LowerRightCorner), 0, GUI_ID_EDITOR_BUTTON, L"Editor personajes", L"Modela tus personajes");
					env->addButton(gh.ScaleValuebyScreenHeight(fourth_rect.UpperLeftCorner, fourth_rect.LowerRightCorner), 0, GUI_ID_OPCIONES_BUTTON, L"Opciones", L"Configura el juego");
					env->addButton(gh.ScaleValuebyScreenHeight(fifth_rect.UpperLeftCorner, fifth_rect.LowerRightCorner), 0, GUI_ID_QUIT_BUTTON, L"Quit", L"Sal del juego");

					


					return true;
				case GUI_ID_1VS1_BUTTON:
					env->clear();

				
					env->addButton(gh.ScaleValuebyScreenHeight(first_rect.UpperLeftCorner, first_rect.LowerRightCorner), 0, GUI_ID_CONTINUAR_BUTTON, L"Continuar", L"Continua desde el ultimo nivel desbloqueado");
					env->addButton(gh.ScaleValuebyScreenHeight(second_rect.UpperLeftCorner, second_rect.LowerRightCorner), 0, GUI_ID_NUEVA_PARTIDA_BUTTON, L"Nueva Partida", L"Comienza de nuevo");
					env->addButton(gh.ScaleValuebyScreenHeight(third_rect.UpperLeftCorner, third_rect.LowerRightCorner), 0, GUI_ID_APRENDIZAJE, L"Aprendizaje", L"Ejecutar Aprendizaje");
					env->addButton(gh.ScaleValuebyScreenHeight(fourth_rect.UpperLeftCorner, fourth_rect.LowerRightCorner), 0, GUI_ID_JAPRENDIZAJE, L"Juego Con Aprendizaje", L"Ejecutar JAprendizaje");
					env->addButton(gh.ScaleValuebyScreenHeight(fifth_rect.UpperLeftCorner, fifth_rect.LowerRightCorner), 0, GUI_ID_VOLVER_BUTTON, L"Inicio", L"Menu Inicio");
		
					return true;

				

				case GUI_ID_EQUIPO_BUTTON:
					env->clear();
					env->addButton(gh.ScaleValuebyScreenHeight(first_rect.UpperLeftCorner, first_rect.LowerRightCorner), 0, GUI_ID_NUEVA_PARTIDA_BUTTON, L"Nueva Partida", L"Comienza de nuevo");
					env->addButton(gh.ScaleValuebyScreenHeight(second_rect.UpperLeftCorner, second_rect.LowerRightCorner), 0, GUI_ID_VOLVER_BUTTON, L"Inicio", L"Menu inicio");

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
					

					env->addButton(gh.ScaleValuebyScreenHeight(first_rect.UpperLeftCorner, first_rect.LowerRightCorner), 0, GUI_ID_VOLVER_BUTTON, L"Inicio", L"Menu inicio");

					return true;

				case GUI_ID_OPCIONES_BUTTON:
					env->clear();

					volume_control = env->addScrollBar(true, gh.ScaleValuebyScreenHeight(scrollbar_rect.UpperLeftCorner, scrollbar_rect.LowerRightCorner),0,GUI_ID_VOLUME_SCROLLBAR);
					volume_control->setMax(100);
					volume_control->setPos(sound->get_volume() * 100);

					res_control = env->addComboBox(gh.ScaleValuebyScreenHeight(combobox_rect.UpperLeftCorner, combobox_rect.LowerRightCorner), 0, GUI_ID_RESOLUTION_COMBOBOX);
					res_control->addItem(L"800x600");
					res_control->addItem(L"1024x768");
					res_control->addItem(L"1280x768");
					res_control->addItem(L"1280x960");
					res_control->addItem(L"1366x768");
					res_control->addItem(L"1600x1200");

					env->addButton(gh.ScaleValuebyScreenHeight(second_rect.UpperLeftCorner, second_rect.LowerRightCorner), 0, GUI_ID_VOLVER_BUTTON, L"Inicio", L"Menu inicio");

					return true;

				case GUI_ID_INVENTARIO_BUTTON:
					env->clear();
					

					env->addButton(gh.ScaleValuebyScreenHeight(first_rect.UpperLeftCorner, first_rect.LowerRightCorner), 0, GUI_ID_VOLVER_BUTTON, L"Inicio", L"Menu inicio");

					return true;
					
				default:
					return false;
			}
            break;
		case EGET_SCROLL_BAR_CHANGED:
			if(id == GUI_ID_VOLUME_SCROLLBAR)
			{
				int pos = ((IGUIScrollBar*)event.GUIEvent.Caller)->getPos();
				sound->set_volume(pos / 100.0);

			}
			break;
		case EGET_COMBO_BOX_CHANGED:
			if(id == GUI_ID_RESOLUTION_COMBOBOX)
			{
				int selected = ((IGUIComboBox*)event.GUIEvent.Caller)->getSelected();
				resize = true;

				switch(selected)
				{
					case 0:
						this->height = 800;
						this->width = 600;
						break;
					case 1:
						this->height = 1024;
						this->width = 768;
						break;
					case 2:
						this->height = 1280;
						this->width = 768;
						break;
					case 3:
						this->height = 1280;
						this->width = 960;
						break;
					case 4:
						this->height = 1366;
						this->width = 768;
						break;
					case 5:
						this->height = 1600;
						this->width = 1200;
						break;
				}


				env->addButton(gh.ScaleValuebyScreenHeight(second_rect.UpperLeftCorner, second_rect.LowerRightCorner), 0, GUI_ID_VOLVER_BUTTON, L"Inicio", L"Menu inicio");

			}
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