#pragma once


#include "Goal_Think.h"

class Goal_Think;
class Npc;
class Goal_GetItem : public Goal_Composite
{

public:
	Goal_GetItem(Npc *p,int item);
	~Goal_GetItem(void);
	void Activate();

  int Process();

  void Terminate(){estado=completed;}

	Npc* dueño;
	int item;//numero de item a coger o 1 que es salud o 2 que es arma
};

