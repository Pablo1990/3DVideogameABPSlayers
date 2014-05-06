#pragma once


#include <irrlicht.h>
#include "SoundEffect.h"
#include "Paths.h"
#include "driverChoice.h"
#include "BotonesMenu.h"
#include "GameData.h"
#include "GUIHandler.h"
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
	MyMenu();
	~MyMenu(void);
	int AddMenu(video::E_DRIVER_TYPE&);
	void setStart();
	virtual bool OnEvent(const SEvent& event);
	int get_level();

private:
	void change_device(int h, int w);
	void select_screen_size(int selected);

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
	E_DRIVER_TYPE dt;
	bool resize;
	int height;
	int width;
	bool fullscreen;
	bool save;
	float volume;
	int selected_res;
	GUIHandler gh;
};

