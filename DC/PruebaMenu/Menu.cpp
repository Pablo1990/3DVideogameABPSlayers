#include "Menu.h"

MyMenu::MyMenu(void)
{
}

MyMenu::~MyMenu(void)
{
}

int MyMenu::AddMenu()
{
	// ask user for driver
    video::E_DRIVER_TYPE driverType = driverChoiceConsole();
    if (driverType==video::EDT_COUNT)
        return 1;


	core::dimension2d<u32> resolution ( 1366, 768 );

    // create device and exit if creation failed
	irr::SIrrlichtCreationParameters params;
	params.DriverType=driverType;
	params.WindowSize=resolution;
	params.Bits=32;
	params.Fullscreen=true;
	


    IrrlichtDevice * device = createDeviceEx(params);

    if (device == 0)
        return 1; // could not create selected driver.

	 device->setWindowCaption(L"Irrlicht Engine - User Interface Demo");
    device->setResizable(true);

    video::IVideoDriver* driver = device->getVideoDriver();
    IGUIEnvironment* env = device->getGUIEnvironment();

	IGUISkin* skin = env->getSkin();
   
    skin->setFont(env->getBuiltInFont(), EGDF_TOOLTIP);


	env->addButton(rect<s32>(500,150,600,150 + 32), 0, GUI_ID_JUGAR_BUTTON,
            L"Jugar", L"Comienza el juego");

	env->addButton(rect<s32>(500,190,600,190 + 32), 0, GUI_ID_INVENTARIO_BUTTON,
            L"Inventario", L"Echale un ojo a tus objetos");

	env->addButton(rect<s32>(500,230,600,230 + 32), 0, GUI_ID_EDITOR_BUTTON,
            L"Editor personajes", L"Modela tus personajes");

	env->addButton(rect<s32>(500,270,600,270 + 32), 0, GUI_ID_OPCIONES_BUTTON,
            L"Opciones", L"Configura el juego");

	env->addButton(rect<s32>(500,310,600,310 + 32), 0, GUI_ID_QUIT_BUTTON,
            L"Quit", L"Sal del juego");

	
   	env->addStaticText(L"AQUI VA UNA IMAGEN", rect<s32>(50,110,250,130), true);


    // Store the appropriate data in a context structure.
    SAppContext context;
    context.device = device;
    context.counter = 0;

    // Then create the event receiver, giving it that context structure.
    MyEventReceiver receiver(context, *this);

    // And tell the device to use our custom event receiver.
    device->setEventReceiver(&receiver);

	video::ITexture* irrlichtBack = driver->getTexture("../Imagenes/demoback.jpg");


	while(device->run() && driver)
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

	return 0;
}
