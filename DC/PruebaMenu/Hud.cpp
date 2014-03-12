#include "Hud.h"


Hud::Hud(void)
{

}


Hud::~Hud(void)
{

}
void Hud:: drawHud(IrrlichtDevice* device)
{
		core::dimension2d<u32> size = device->getVideoDriver()->getScreenSize();
		const int lwidth = size.Width - 20;
	const int lheight = 16;
	core::rect<int> pos(size.Width-150, size.Height-150, size.Width-50, size.Height-50);
	device->getGUIEnvironment()->addButton(pos);
        
	
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