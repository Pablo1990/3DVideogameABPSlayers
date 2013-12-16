#ifndef _STRUCTS_H
#define _STRUCTS_H
#include <irrlicht.h>
using namespace irr;
using namespace core;

struct SParticleImpact
{
	u32 when;
	vector3df pos;
	vector3df outVector;
	bool collision_flag;
	ISceneNode* node;
};

enum
{
	SWORD_TYPE,
	BOW_TYPE,
	RED_SHROOM_TYPE,
	BLUE_SHROOM_TYPE,
	YELLOW_SHROOM_TYPE
};

#endif