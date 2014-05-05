#include "GUIHandler.h"

GUIHandler::GUIHandler()
{
	scale_y = 1;
	scale_x = 1;
}

GUIHandler::GUIHandler(IrrlichtDevice* device)
{
	if(device)
	{
		scale_x = (device->getVideoDriver()->getScreenSize().Width/800.0f);
		scale_y = (device->getVideoDriver()->getScreenSize().Height/600.0f);
	}

}

void GUIHandler::calculate_scale(IrrlichtDevice* device)
{
	if(device)
	{
		scale_x = (device->getVideoDriver()->getScreenSize().Width/800.0f);
		scale_y = (device->getVideoDriver()->getScreenSize().Height/600.0f);
	}

}

GUIHandler &GUIHandler::operator=(const GUIHandler &gh)
{
	if(this != &gh)
	{
		this->scale_x = gh.scale_x;
		this->scale_y = gh.scale_y;
	}
	return *this;
}

rect<s32> GUIHandler::ScaleValuebyScreenHeight(vector2d<s32> upper_left_corner, vector2d<s32> lower_right_corner)
{
	upper_left_corner.X = upper_left_corner.X * scale_x;
	upper_left_corner.Y = upper_left_corner.Y * scale_y;

	lower_right_corner.X = lower_right_corner.X * scale_x;
	lower_right_corner.Y = lower_right_corner.Y * scale_y;
	
	return rect<s32>(upper_left_corner, lower_right_corner);
}


float GUIHandler::get_scale_x()
{
	return scale_x;
}

float GUIHandler::get_scale_y()
{
	return scale_y;
}

GUIHandler::~GUIHandler(void)
{
}
