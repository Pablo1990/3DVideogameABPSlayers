#pragma once
#include <irrlicht.h>
using namespace irr;
using namespace core;


class Mapa
{
public:
	Mapa(void);
	~Mapa(void);
private:
	float first_map_corner[3];
	float last_map_corner[3];
	float first_bridge_corner[3];
	float last_bride_corner[4];
	float bridge_heigth;
	float obstacles[];//Posicion 0 -> esquina1_obstaculo1, posicion 1 -> esquina2_obstaculo1, etc
};

