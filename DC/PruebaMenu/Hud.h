
#ifndef _IRRLICHT_HUD_UTIL_H_
#define _IRRLICHT_HUD_UTIL_H_


#include <irrlicht.h>
#include <string>
#include <vector>

#include "Player.h"
#include "Npc.h"
#include "BotonesMenu.h"
#include "GUIHandler.h"
using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
enum
{
    GUI_ID_DIALOG_ROOT_WINDOW  = 0x10000,

    GUI_ID_X_SCALE,
    GUI_ID_Y_SCALE,
    GUI_ID_Z_SCALE,

    GUI_ID_OPEN_MODEL,
    GUI_ID_SET_MODEL_ARCHIVE,
    GUI_ID_LOAD_AS_OCTREE,

    GUI_ID_SKY_BOX_VISIBLE,
    GUI_ID_TOGGLE_DEBUG_INFO,

    GUI_ID_DEBUG_OFF,
    GUI_ID_DEBUG_BOUNDING_BOX,
    GUI_ID_DEBUG_NORMALS,
    GUI_ID_DEBUG_SKELETON,
    GUI_ID_DEBUG_WIRE_OVERLAY,
    GUI_ID_DEBUG_HALF_TRANSPARENT,
    GUI_ID_DEBUG_BUFFERS_BOUNDING_BOXES,
    GUI_ID_DEBUG_ALL,

    GUI_ID_MODEL_MATERIAL_SOLID,
    GUI_ID_MODEL_MATERIAL_TRANSPARENT,
    GUI_ID_MODEL_MATERIAL_REFLECTION,

    GUI_ID_CAMERA_MAYA,
    GUI_ID_CAMERA_FIRST_PERSON,

    GUI_ID_POSITION_TEXT,

    GUI_ID_ABOUT,
    GUI_ID_QUIT,

    GUI_ID_TEXTUREFILTER,
    GUI_ID_SKIN_TRANSPARENCY,
    GUI_ID_SKIN_ANIMATION_FPS,

    GUI_ID_BUTTON_SET_SCALE,
    GUI_ID_BUTTON_SCALE_MUL10,
    GUI_ID_BUTTON_SCALE_DIV10,
    GUI_ID_BUTTON_OPEN_MODEL,
    GUI_ID_BUTTON_SHOW_ABOUT,
    GUI_ID_BUTTON_SHOW_TOOLBOX,
    GUI_ID_BUTTON_SELECT_ARCHIVE,

    GUI_ID_ANIMATION_INFO,

    // And some magic numbers
    MAX_FRAMERATE = 80,
    DEFAULT_FRAMERATE = 30
};

class Hud
{
public:
	Hud(IrrlichtDevice* device, SoundEffect* s);
	Hud();
	~Hud(void);
	void drawHud(IrrlichtDevice*,Npc* npc, Player* player);
	void  setSkinTransparency(irr::gui::IGUISkin * skin);
	void setHud(Npc* npc, Player* player);
	void drawMenu(IrrlichtDevice* device);
	void borrarMenu(IrrlichtDevice* device);
	void ActivaMenu();
	void setVisibleHudT();
	void setVisibleHudF();
	void show_main_buttons();
	void show_audio_menu();
	void show_end_menu(bool);
	void set_level(int level);
	void show_controls();

private:
	int level;
	SoundEffect* sound;
	IGUIScrollBar* volume_control;
	IGUIEnvironment* env ;
	IGUISkin* skin ;
	IGUITabControl* tab;
	IGUIStaticText* titulo;
	IGUIStaticText* tituloPause;
	IGUIStaticText* textSalud;
	IGUIStaticText* VSalud;
	IGUIStaticText* textDesgaste;
	IGUIStaticText* Desgaste;
	IGUIStaticText* textCansancio;
	IGUIStaticText* Cansancio;
	IGUIStaticText* SaludBot;
	IGUIStaticText* Audio;

	IGUITabControl* tabMenu;
	IGUIButton*		Reanudar;
	IGUIButton*		VMenu;
	IGUIButton*		Salir;
	IGUIButton*		Opciones;
	IGUIButton*		volver_button;
	IGUIButton*		next_level_button;
	IGUIImage*		controls_image;

	core::dimension2d<u32> size;
   gui::IGUIFont* font2 ;

	static
void drawHudText
(
	IVideoDriver* driver,
	const core::stringw& text
);

};

#endif // _IRRLICHT_HUD_UTIL_H_