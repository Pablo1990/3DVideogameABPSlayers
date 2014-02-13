#pragma once

#include "Position.h"

class Pathfinding  {
private:
	Position pIni;
	Position pFin;

public:
	/* Constructores y destructor */
	Pathfinding(Position, Position);
	~Pathfinding(void);

	/*getters y setters*/
	float getPosInicio();
	void getPosInicio(float);

	float getPosFin();
	void getPosFin(float);

	/* functions */
	
};