#pragma once


#include <irrlicht.h>
#include "SoundEffect.h"
#include "Paths.h"
#include "driverChoice.h"
#include "BotonesMenu.h"
#include "GameData.h"
#include "GUIHandler.h"
#include <vector>
//#include "MyEventReceiver.h"

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#endif


//class MyEventReceiver;

class MyMenu: public IEventReceiver
{

public:
	MyMenu(E_DRIVER_TYPE dt);
	~MyMenu(void);
	int AddMenu();
	void setStart();
	void setStart(bool s);
	virtual bool OnEvent(const SEvent& event);
	int get_level();
	int get_height();
	int get_width();
	float get_volume();
	bool get_fullscreen();
	int get_weapon();

private:
	void change_device(int h, int w);
	void select_screen_size(int selected);
	void initialize();
	void select_menu(int stat);
	int add_to_sprite_bank(rect<s32> sprite_rect, const char* path);
	void load_sprite_bank();


	int start;
	IrrlichtDevice * device;
	SoundEffect *sound;
	int level;
	IGUIScrollBar* volume_control;
	IGUIComboBox* res_control;
	rect<s32> first_rect;
	rect<s32> second_rect;
	rect<s32> third_rect;
	rect<s32> fourth_rect;
	rect<s32> fifth_rect;
	rect<s32> scrollbar_rect;
	rect<s32> combobox_rect;
	rect<s32> listbox_rect;
	E_DRIVER_TYPE driverType;
	bool resize;
	int height;
	int width;
	bool fullscreen;
	bool save;
	float volume;
	int selected_res;
	GUIHandler gh;
	video::IVideoDriver* driver;
	IGUIEnvironment* env;
	ITexture* irrlichtBack;
	IGUIListBox* list_box;
	IGUISpriteBank* spriteBank;
	int sprite_sword;
	int sprite_spear;
	int sprite_bow;
	int selected_weapon;
};

