#include "Menu.h"


MyMenu::MyMenu(E_DRIVER_TYPE dt)
{
	driverType = dt;
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
	listbox_rect = rect<s32>(470,115,647,273);
	resize = false;
	selected_res = 0;
	selected_weapon = 0;

	if(!gd.load_config(volume, height, width, fullscreen))
	{
		height = 600;
		width = 800;
		volume = 0.7f;
		fullscreen = true;
		selected_res = 0;
	}
	
	if(width == 800 && height == 600)
		selected_res = 0;
	else if(width == 1024 && height == 768)
		selected_res = 1;
	else if(width == 1280 && height == 768)
		selected_res = 2;
	else if(width == 1280 && height == 960)
		selected_res = 3;
	else if(width == 1366 && height == 768)
		selected_res = 4;
	else if(width == 1600 && height == 1200)
		selected_res = 5;

	
	save = false;
}

MyMenu::~MyMenu(void)
{
	if(sound)
	{
		delete sound;
		sound = 0;
	}


}


void MyMenu::initialize()
{
	irr::SIrrlichtCreationParameters params;

	core::dimension2d<u32> resolution (width, height );
	// create device and exit if creation failed
	params.DriverType=driverType;
	params.WindowSize=resolution;
	params.Bits=32;
	params.Fullscreen=fullscreen;

	device = createDeviceEx(params);
	gh.calculate_scale(device);

	device->setWindowCaption(L"MENU");
	device->setResizable(true);

	driver = device->getVideoDriver();
	
	env = device->getGUIEnvironment();

	IGUISkin* skin = env->getSkin();

	IGUIFont* font2 =env->getFont("../media/fuentegrande.png");

	if(gh.get_scale_x() < 1.30)
	{
		if(font2)
			font2->drop();
		font2 = env->getFont("../media/fuentepeq.png");
	}


	skin->setFont(font2);
	skin->setColor(EGDC_3D_SHADOW  , video::SColor(25,210,50,0));
	skin->setColor(EGDC_3D_FACE  , video::SColor(70,215,0,15));
	skin->setColor(EGDC_HIGH_LIGHT , video::SColor(20,255,0,0));
	skin->setColor(EGDC_ICON_HIGH_LIGHT , video::SColor(100,255,255,0));

	device->setEventReceiver(this);

	irrlichtBack = driver->getTexture("../Imagenes/demoback1.png");
	this->load_sprite_bank();

}

void MyMenu::select_menu(int stat)
{
	IGUIEnvironment* env = device->getGUIEnvironment();
	switch(stat)
	{
		case 0://principal
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
			break;
		case 1://jugar
			env->clear();
			env->addButton(gh.ScaleValuebyScreenHeight(first_rect.UpperLeftCorner, first_rect.LowerRightCorner), 0, GUI_ID_1VS1_BUTTON, L"1vs1", L"Juega en solitario");
			env->addButton(gh.ScaleValuebyScreenHeight(second_rect.UpperLeftCorner, second_rect.LowerRightCorner), 0, GUI_ID_EQUIPO_BUTTON, L"Equipo", L"Juega como miembro de equipo");
			env->addButton(gh.ScaleValuebyScreenHeight(third_rect.UpperLeftCorner, third_rect.LowerRightCorner), 0, GUI_ID_VOLVER_BUTTON, L"Inicio", L"Menu Anterior");
			break;
		case 2://1vs1
			env->clear();
			env->addButton(gh.ScaleValuebyScreenHeight(first_rect.UpperLeftCorner, first_rect.LowerRightCorner), 0, GUI_ID_CONTINUAR_BUTTON, L"Continuar", L"Continua desde el ultimo nivel desbloqueado");
			env->addButton(gh.ScaleValuebyScreenHeight(second_rect.UpperLeftCorner, second_rect.LowerRightCorner), 0, GUI_ID_NUEVA_PARTIDA_BUTTON, L"Nueva Partida", L"Comienza de nuevo");
			env->addButton(gh.ScaleValuebyScreenHeight(third_rect.UpperLeftCorner, third_rect.LowerRightCorner), 0, GUI_ID_APRENDIZAJE, L"Aprendizaje", L"Ejecutar Aprendizaje");
			env->addButton(gh.ScaleValuebyScreenHeight(fourth_rect.UpperLeftCorner, fourth_rect.LowerRightCorner), 0, GUI_ID_JAPRENDIZAJE, L"Juego Con Aprendizaje", L"Ejecutar JAprendizaje");
			env->addButton(gh.ScaleValuebyScreenHeight(fifth_rect.UpperLeftCorner, fifth_rect.LowerRightCorner), 0, GUI_ID_VOLVER_BUTTON, L"Inicio", L"Menu Inicio");
			break;
		case 3://equipos
			env->clear();
			env->addButton(gh.ScaleValuebyScreenHeight(first_rect.UpperLeftCorner, first_rect.LowerRightCorner), 0, GUI_ID_NUEVA_PARTIDA_BUTTON, L"Nueva Partida", L"Comienza de nuevo");
			env->addButton(gh.ScaleValuebyScreenHeight(second_rect.UpperLeftCorner, second_rect.LowerRightCorner), 0, GUI_ID_VOLVER_BUTTON, L"Inicio", L"Menu inicio");
			break;
		case 4: //inventario
			env->clear();

			list_box = env->addListBox(gh.ScaleValuebyScreenHeight(listbox_rect.UpperLeftCorner, listbox_rect.LowerRightCorner), 0, GUI_ID_INVENTORY_LISTBOX, true);
			list_box->setSpriteBank(spriteBank);
			list_box->setItemHeight(50*gh.get_scale_y());
			list_box->addItem(L"Espada", sprite_sword);
			list_box->addItem(L"Lanza", sprite_spear);
			list_box->addItem(L"Arco", sprite_bow);
			
			env->addButton(gh.ScaleValuebyScreenHeight(fifth_rect.UpperLeftCorner, fifth_rect.LowerRightCorner), 0, GUI_ID_VOLVER_BUTTON, L"Inicio", L"Menu inicio");
			break;
		case 5://opciones
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
			res_control->setSelected(selected_res);

			env->addCheckBox(fullscreen, gh.ScaleValuebyScreenHeight(second_rect.UpperLeftCorner, second_rect.LowerRightCorner), 0, GUI_ID_FULLSCREEN_CHECKBOX, L"Pantalla completa");
			env->addButton(gh.ScaleValuebyScreenHeight(third_rect.UpperLeftCorner, third_rect.LowerRightCorner), 0, GUI_ID_VOLVER_BUTTON, L"Inicio", L"Menu inicio");
			break;
		case 6://editor
			env->clear();
			env->addButton(gh.ScaleValuebyScreenHeight(first_rect.UpperLeftCorner, first_rect.LowerRightCorner), 0, GUI_ID_VOLVER_BUTTON, L"Inicio", L"Menu inicio");
			break;
		case 7://solo borramos
			env->clear();
	}
}

int MyMenu::AddMenu()
{
	this->initialize();
	gh = GUIHandler(device);


	if(!sound)
		sound = new SoundEffect(menu_music_path);
	
	this->select_menu(0);

	sound->play_background();
	sound->set_volume(volume);
	while(device->run() && driver && start == false)
	{
		if(resize)
		{


			device->closeDevice();
			device->run(); // Very important to do this here!

			this->initialize();

			this->select_menu(5);
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

	sound->stop_all_sounds();
	return start;
}

void MyMenu::change_device(int h, int w)
{
	core::dimension2d<u32> resolution ( /*800,600*/1366, 768 );
	// create device and exit if creation failed
	irr::SIrrlichtCreationParameters params;
	params.DriverType=driverType;
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
					this->select_menu(0);

					if(save)
					{
						GameData gd;
						gd.save_config(sound->get_volume(), height, width, fullscreen);
						save = false;
					}
					return true;

					break;
				case GUI_ID_JUGAR_BUTTON:
					this->select_menu(1);
					return true;

				case GUI_ID_1VS1_BUTTON:
					this->select_menu(2);
					return true;

				

				case GUI_ID_EQUIPO_BUTTON:
					this->select_menu(3);
					return true;

				case GUI_ID_CONTINUAR_BUTTON:
					this->select_menu(7);//Solo borramos
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
					this->select_menu(6);
					return true;

				case GUI_ID_OPCIONES_BUTTON:
					this->select_menu(5);
					return true;

				case GUI_ID_INVENTARIO_BUTTON:
					this->select_menu(4);
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
				save = true;

			}
			break;
		case EGET_COMBO_BOX_CHANGED:
			if(id == GUI_ID_RESOLUTION_COMBOBOX)
			{
				int selected = ((IGUIComboBox*)event.GUIEvent.Caller)->getSelected();
				resize = true;
				save = true;
				selected_res = selected;
				this->select_screen_size(selected);
			}
			break;
		case EGET_CHECKBOX_CHANGED:
			if(id == GUI_ID_FULLSCREEN_CHECKBOX)
			{
				fullscreen = ((IGUICheckBox*)event.GUIEvent.Caller)->isChecked();
				resize = true;
				save = true;
			}
			break;
		case EGET_LISTBOX_CHANGED:
			if(id == GUI_ID_INVENTORY_LISTBOX)
			{
				selected_weapon = ((IGUIListBox*)event.GUIEvent.Caller)->getSelected();
				cout << selected_weapon << endl;
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

void MyMenu::select_screen_size(int selected)
{
	switch(selected)
	{
		case 0:
			this->width = 800;
			this->height = 600;
			break;
		case 1:
			this->width = 1024;
			this->height = 768;
			break;
		case 2:
			this->width = 1280;
			this->height = 768;
			break;
		case 3:
			this->width = 1280;
			this->height = 960;
			break;
		case 4:
			this->width = 1366;
			this->height = 768;
			break;
		case 5:
			this->width = 1600;
			this->height = 1200;
			break;
	}
}

int MyMenu::add_to_sprite_bank(rect<s32> sprite_rect, const char* path)
{
	spriteBank = env->getSkin()->getSpriteBank();
	
	ITexture * tex = driver->getTexture(path );
	if ( tex )
    {
        core::array< core::rect<irr::s32> >& spritePositions = spriteBank->getPositions();
		spritePositions.push_back(gh.ScaleValuebyScreenHeight(sprite_rect.UpperLeftCorner, sprite_rect.LowerRightCorner));
        array< SGUISprite >& sprites = spriteBank->getSprites();
        spriteBank->addTextureAsSprite( tex );
        gui::SGUISpriteFrame frame;
        frame.rectNumber = spritePositions.size()-1;
        frame.textureNumber = spriteBank->getTextureCount()-1;

        SGUISprite sprite;
        sprite.frameTime = 0;
        sprite.Frames.push_back( frame );

        sprites.push_back( sprite );
		return sprites.size()-1;
    }
	return -1;
}

void MyMenu::load_sprite_bank()
{
	rect<s32> sprite_rect(0,0,70,70);
	sprite_sword = this->add_to_sprite_bank(gh.ScaleValuebyScreenHeight(sprite_rect.UpperLeftCorner, sprite_rect.LowerRightCorner), sword_icon_path);
	
	sprite_rect = rect<s32>(70,0, 140, 70);
	sprite_spear = this->add_to_sprite_bank(gh.ScaleValuebyScreenHeight(sprite_rect.UpperLeftCorner, sprite_rect.LowerRightCorner), spear_icon_path);

	sprite_rect = rect<s32>(0,70, 70, 140);
	sprite_bow = this->add_to_sprite_bank(gh.ScaleValuebyScreenHeight(sprite_rect.UpperLeftCorner, sprite_rect.LowerRightCorner), bow_icon_path);

}

int MyMenu::get_width()
{
	return this->width;
}

int MyMenu::get_height()
{
	return this->height;
}

bool MyMenu::get_fullscreen()
{
	return this->fullscreen;
}

float MyMenu::get_volume()
{
	return this->volume;
}