#pragma once
#include <irrlicht.h>
using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#endif
class GUIHandler
{
public:
	GUIHandler(IrrlichtDevice* device);
	rect<s32> ScaleValuebyScreenHeight(vector2d<s32> upper_left_corner, vector2d<s32> lower_right_corner);
	float get_scale_y();
	float get_scale_x();
	~GUIHandler(void);
private:
	float scale_x;
	float scale_y;
};

