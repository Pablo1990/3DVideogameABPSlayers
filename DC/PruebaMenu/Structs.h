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
};

enum
{
	SWORD_TYPE,
	BOW_TYPE
};

#endif