#include "Hud.h"


Hud::Hud(IrrlichtDevice* device)
{

	size = device->getVideoDriver()->getScreenSize();
	env = device->getGUIEnvironment();
	 skin = env->getSkin();

   font2 =env->getFont("../media/fonthaettenschweiler.bmp");
   skin->setFont(font2);
  

	skin->setColor(EGDC_3D_SHADOW  , video::SColor(1,25,250,15));
	skin->setColor(EGDC_3D_FACE  , video::SColor(1,25,250,15));
	skin->setColor(EGDC_3D_HIGH_LIGHT   , video::SColor(2,175,238,238));

	

	
}


Hud::~Hud(void)
{

}
Hud::Hud()
{

}
void Hud::setSkinTransparency(irr::gui::IGUISkin * skin)
{
    skin->setColor(EGDC_3D_SHADOW  , video::SColor(1,25,250,15));
	skin->setColor(EGDC_3D_FACE  , video::SColor(1,25,250,15));
	skin->setColor(EGDC_3D_HIGH_LIGHT   , video::SColor(2,175,238,238));
}
void Hud:: drawHud(IrrlichtDevice* device,Npc* npc, Player* player)
{
	
 setSkinTransparency(skin);
   
	wchar_t tmp[255];
	core::rect<int> pos(size.Width-200, size.Height-250, size.Width-5 ,size.Height-5);
	tab=device ->getGUIEnvironment()->addTabControl(pos,0,true,true);

	titulo=  device ->getGUIEnvironment()->addStaticText(L"Jugador 1", core::rect<s32>(5,15,80,66), false, false, tab);
	titulo ->setOverrideColor(video::SColor(250,25,25,112));

	//Salud
	textSalud=  device ->getGUIEnvironment()->addStaticText(L"Salud:", core::rect<s32>(5,55,80,86), false, false, tab);
	textSalud ->setOverrideColor(video::SColor(250,0,0,78));
	swprintf(tmp, 255, L"%.2f / 100", player->get_health());
	
		VSalud= device ->getGUIEnvironment()->addStaticText(tmp, core::rect<s32>(50,75,150,96), false, false, tab);
	VSalud ->setOverrideColor(video::SColor(250,78,15,8));

	//Desgaste Arma:
	textDesgaste=  device ->getGUIEnvironment()->addStaticText(L"Desgaste Arma:", core::rect<s32>(5,105,100,136), false, false, tab);
	textDesgaste ->setOverrideColor(video::SColor(250,0,0,78));
	swprintf(tmp, 255, L"%.2f / 15", player->get_weapon()->get_resist());
	Desgaste =device ->getGUIEnvironment()->addStaticText(tmp, core::rect<s32>(50,135,150,176), false, false, tab);
	Desgaste ->setOverrideColor(video::SColor(250,78,15,8));

	//Cansancio
	textCansancio=  device ->getGUIEnvironment()->addStaticText(L"Cansancio:", core::rect<s32>(5,155,80,186), false, false, tab);
	textCansancio ->setOverrideColor(video::SColor(250,0,0,78));
	Cansancio= device ->getGUIEnvironment()->addStaticText(L"100 / 100", core::rect<s32>(50,185,100,206), false, false, tab);
	Cansancio ->setOverrideColor(video::SColor(250,78,15,8));

	//Info de enemigo
	device ->getGUIEnvironment()->addImage( core::rect<s32>(0,205,250,209),tab,0,0);
	swprintf(tmp, 255, L"Salud Enemigo: %.2f / 100", npc->get_health());
	SaludBot= device ->getGUIEnvironment()->addStaticText(tmp, core::rect<s32>(10,215,200,286), false, false, tab);
	SaludBot ->setOverrideColor(video::SColor(250,75,0,130));
      	
}

void Hud::setHud(Npc* npc, Player* player)
{
	wchar_t tmp[255];
	swprintf(tmp, 255, L"%.2f / 100", player->get_health());

	if(VSalud)
		VSalud->setText(tmp);

	swprintf(tmp, 255, L"%.2f / 15",  player->get_weapon()->get_resist());

	if(Desgaste)
		Desgaste->setText(tmp);


	//Falta Cansancio

	swprintf(tmp, 255, L"%.2f / 100", npc->get_health());

	if(SaludBot)
		SaludBot->setText(tmp);

}
void Hud::drawHudText
(
IVideoDriver* driver,
	const core::stringw& text
	
)
{
	/*if (!device)
		return;
	
	gui::IGUIFont* font=device->getGUIEnvironment()->getFont("../media/fontcourier.bmp");;*/

	
	
	if (!driver)
		return;
	
	/*core::dimension2d<u32> textSize = font->getDimension(text.c_str());
	s32 tw = textSize.Width;
	s32 th = textSize.Height;
	
	*/
	core::rect<s32> rect;
video::ITexture* images2 = driver->getTexture("../media/2ddemo.png");
 driver->draw2DImage(images2, core::position2d<s32>(500,500),
                core::rect<s32>(0,0,342,224), 0,
                video::SColor(255,255,255,255), true);

	
	/*bool hcenter = true;
	bool vcenter = true;
	font->draw(text, rect, 0xFFFF0000, hcenter, vcenter);*/

}