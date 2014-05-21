#include "Hud.h"


Hud::Hud(IrrlichtDevice* device, SoundEffect* s)
{

	 size = device->getVideoDriver()->getScreenSize();
	 env = device->getGUIEnvironment();
	 skin = env->getSkin();

	font2 =env->getFont("../media/fonthaettenschweiler.bmp");
	skin->setFont(font2);
  

	skin->setColor(EGDC_3D_SHADOW  , video::SColor(1,25,250,15));
	skin->setColor(EGDC_3D_FACE  , video::SColor(1,25,250,15));
	skin->setColor(EGDC_3D_HIGH_LIGHT   , video::SColor(2,175,238,238));

	this->sound = s;

	this->level = 0;
	

	
}


Hud::~Hud(void)
{


	if(this->titulo)
	{
		this->titulo->getParent()->removeChild(this->titulo);
		this->titulo = 0;
	}

	if(this->textSalud)
	{
		this->textSalud->getParent()->removeChild(this->textSalud);
		this->textSalud = 0;
	}

	if(this->VSalud)
	{
		this->VSalud->getParent()->removeChild(this->VSalud);
		this->VSalud = 0;
	}

	if(this->textDesgaste)
	{
		this->textDesgaste->getParent()->removeChild(this->textDesgaste);
		this->textDesgaste = 0;
	}

	if(this->textCansancio)
	{
		this->textCansancio->getParent()->removeChild(this->textCansancio);
		this->textCansancio = 0;
	}

	if(this->Cansancio)
	{
		this->Cansancio->getParent()->removeChild(this->Cansancio);
		this->Cansancio = 0;
	}

	if(this->SaludBot)
	{
		this->SaludBot->getParent()->removeChild(this->SaludBot);
		this->SaludBot = 0;
	}
	if(this->tab)
	{
		this->tab->getParent()->removeChild(this->tab);
		this->tab = 0;
	}

	
}
Hud::Hud()
{

}
void Hud::setSkinTransparency( irr::gui::IGUISkin * skin)
{
	skin->setColor(EGDC_3D_SHADOW  , video::SColor(50,25,250,15));
	skin->setColor(EGDC_3D_FACE  , video::SColor(50,25,250,15));
	skin->setColor(EGDC_3D_HIGH_LIGHT   , video::SColor(50,175,238,238));
}



void Hud::drawMenu(IrrlichtDevice* device)
{

	GUIHandler gh(device);


	rect<s32> pos(240,100,640,400/*size.Width*0.3, size.Height*0.4, size.Width*0.8 ,size.Height*0.8*/);
	pos = gh.ScaleValuebyScreenHeight(pos);
	tabMenu=device ->getGUIEnvironment()->addTabControl(pos,0,true,true);
	
	skin->setColor(EGDC_3D_SHADOW  , video::SColor(25,210,50,0));
	skin->setColor(EGDC_3D_FACE  , video::SColor(70,215,0,15));
	skin->setColor(EGDC_HIGH_LIGHT , video::SColor(20,255,0,0));
	skin->setColor(EGDC_ICON_HIGH_LIGHT , video::SColor(100,255,255,0));
	tituloPause=  device ->getGUIEnvironment()->addStaticText(L"Pause", gh.ScaleValuebyScreenHeight(rect<s32>(5,5,90,51)), false, false, tabMenu);

	Reanudar = device->getGUIEnvironment()->addButton(gh.ScaleValuebyScreenHeight(rect<s32>(80,50,330,95))
			,tabMenu,GUI_ID_CONTINUAR_BUTTON, L"Reanudar");

	Opciones = device->getGUIEnvironment()->addButton(gh.ScaleValuebyScreenHeight(rect<s32>(80,115,330,160))
			,tabMenu,GUI_ID_OPCIONES_BUTTON, L"Audio");

	Salir = device->getGUIEnvironment()->addButton(gh.ScaleValuebyScreenHeight(rect<s32>(80,180,330,225))
			,tabMenu,GUI_ID_QUIT_BUTTON, L"Salir");

	VMenu = device->getGUIEnvironment()->addButton(gh.ScaleValuebyScreenHeight(rect<s32>(80,245,330,290))
			,tabMenu,GUI_ID_MENU_BUTTON, L"Volver al menu");

	volver_button = device->getGUIEnvironment()->addButton(gh.ScaleValuebyScreenHeight(rect<s32>(80,245,330,290))
			,tabMenu,GUI_ID_VOLVER_BUTTON, L"Volver");

	volume_control = env->addScrollBar(true, gh.ScaleValuebyScreenHeight(rect<s32>(80,70,330,95)/*rect<s32>(470,115,647,130)*/),tabMenu,GUI_ID_VOLUME_SCROLLBAR);
	
	volume_control->setMax(100);
	volume_control->setPos(sound->get_volume() * 100);
	
	Audio = device ->getGUIEnvironment()->addStaticText(L"Volumen:", gh.ScaleValuebyScreenHeight(rect<s32>(80,50,330,70)), false, false, tabMenu);
	
	next_level_button = device->getGUIEnvironment()->addButton(gh.ScaleValuebyScreenHeight(rect<s32>(80,50,330,95))
		,tabMenu,GUI_ID_NEXT_LEVEL, L"Siguiente Nivel");
	/*	VMenu;
	Salir;
	Opciones;
	*/
}

void Hud::show_main_buttons()
{
	tabMenu->setVisible(true);
	Reanudar->setVisible(true);
	Opciones->setVisible(true);
	Salir->setVisible(true);
	VMenu->setVisible(true);
	volume_control->setVisible(false);
	volver_button->setVisible(false);
	Audio->setVisible(false);
	next_level_button->setVisible(false);
}

void Hud::show_end_menu()
{
	tabMenu->setVisible(true);
	Reanudar->setVisible(false);
	Opciones->setVisible(false);
	Salir->setVisible(true);
	VMenu->setVisible(true);
	volume_control->setVisible(false);
	volver_button->setVisible(false);
	Audio->setVisible(false);
	next_level_button->setVisible(true);

	wchar_t tmp[255];

	swprintf(tmp, 255, L"HAS GANADO, SUPERADO NIVEL %i",this->level);
	if(tituloPause)
		VSalud->setText(tmp);

}

void Hud::show_audio_menu()
{
	volume_control->setPos(sound->get_volume() * 100);
	tabMenu->setVisible(true);
	Reanudar->setVisible(false);
	Opciones->setVisible(false);
	Salir->setVisible(false);
	VMenu->setVisible(false);
	volume_control->setVisible(true);
	volver_button->setVisible(true);
	Audio->setVisible(true);
	next_level_button->setVisible(false);
}

void Hud::ActivaMenu()
{
	skin->setColor(EGDC_3D_SHADOW  , video::SColor(25,210,50,0));
	skin->setColor(EGDC_3D_FACE  , video::SColor(70,215,0,15));
	skin->setColor(EGDC_HIGH_LIGHT , video::SColor(20,255,0,0));
	skin->setColor(EGDC_ICON_HIGH_LIGHT , video::SColor(100,255,255,0));
	
	wchar_t tmp[255];

	swprintf(tmp, 255, L"Pause",this->level);
	if(tituloPause)
		VSalud->setText(tmp);

	show_main_buttons();
}

void Hud::borrarMenu(IrrlichtDevice* device)
{
	tabMenu->setVisible(false);
}

void Hud::setVisibleHudT()
{
	tab->setVisible(true);
}

void Hud::setVisibleHudF()
{
	tab->setVisible(false);
}

void Hud:: drawHud(IrrlichtDevice* device,Npc* npc, Player* player)
{
	
	setSkinTransparency(skin);
   
	wchar_t tmp[255];
	core::rect<int> pos(size.Width-200, size.Height-250, size.Width-5 ,size.Height-5);
	tab=device ->getGUIEnvironment()->addTabControl(pos,0,true,true);

	titulo=  device ->getGUIEnvironment()->addStaticText(L"Jugador 1", core::rect<s32>(5,15,150,66), false, false, tab);
	titulo ->setOverrideColor(video::SColor(250,25,25,112));

	//Salud
	textSalud=  device ->getGUIEnvironment()->addStaticText(L"Salud:", core::rect<s32>(5,55,80,86), false, false, tab);
	textSalud ->setOverrideColor(video::SColor(250,0,0,78));
	swprintf(tmp, 255, L"", player->get_health());
	
		VSalud= device ->getGUIEnvironment()->addStaticText(L"", core::rect<s32>(50,75,150,96), false, false, tab);
	VSalud ->setOverrideColor(video::SColor(250,78,15,8));

	//Desgaste Arma:
	textDesgaste=  device ->getGUIEnvironment()->addStaticText(L"Desgaste Arma:", core::rect<s32>(5,105,100,136), false, false, tab);
	textDesgaste ->setOverrideColor(video::SColor(250,0,0,78));
	swprintf(tmp, 255, L"", player->get_weapon()->get_resist());
	Desgaste =device ->getGUIEnvironment()->addStaticText(L"", core::rect<s32>(50,135,150,176), false, false, tab);
	Desgaste ->setOverrideColor(video::SColor(250,78,15,8));

	//Cansancio
	textCansancio=  device ->getGUIEnvironment()->addStaticText(L"Resistencia:", core::rect<s32>(5,155,80,186), false, false, tab);
	textCansancio ->setOverrideColor(video::SColor(250,0,0,78));
	Cansancio= device ->getGUIEnvironment()->addStaticText(L"20 / 20", core::rect<s32>(50,185,100,206), false, false, tab);
	Cansancio ->setOverrideColor(video::SColor(250,78,15,8));

	//Info de enemigo
	device ->getGUIEnvironment()->addImage( core::rect<s32>(0,205,250,209),tab,0,0);
	swprintf(tmp, 255, L"", npc->get_health());
	SaludBot= device ->getGUIEnvironment()->addStaticText(L"", core::rect<s32>(10,215,200,286), false, false, tab);
	SaludBot ->setOverrideColor(video::SColor(250,255,255,255));
      	
}

void Hud::set_level(int level)
{
	this->level = level;
	wchar_t tmp[255];

	swprintf(tmp, 255, L"Jugador 1 - Nivel %i", level + 1);
		if(titulo)
			titulo->setText(tmp);
}

void Hud::setHud(Npc* npc, Player* player)
{
	wchar_t tmp[255];
	if(player)
	{
		
		swprintf(tmp, 255, L"%.2f / 100",player->get_health());
		if(VSalud)
			VSalud->setText(tmp);

		if(player->get_weapon() !=NULL)
		{
			swprintf(tmp, 255, L" %.2f / 15", player->get_weapon()->get_resist());

			if(Desgaste)
				Desgaste->setText(tmp);
				
		}

		swprintf(tmp, 255, L"%i / 20",player->get_resistance());
		if(Cansancio)
		{
			Cansancio->setText(tmp);
		}

			
	}
	


	//Falta Cansancio
	if(npc)
	{
		swprintf(tmp, 255, L"Salud Enemigo: %.2f / 100", npc->get_health());

		if(SaludBot)
			SaludBot->setText(tmp);
	}
		

}
void Hud::drawHudText
(
IVideoDriver* driver,
	const core::stringw& text
	
)
{
	
	
	
	if (!driver)
		return;

	core::rect<s32> rect;
	video::ITexture* images2 = driver->getTexture("../media/2ddemo.png");
	driver->draw2DImage(images2, core::position2d<s32>(500,500),
                core::rect<s32>(0,0,342,224), 0,
                video::SColor(255,255,255,255), true);


}