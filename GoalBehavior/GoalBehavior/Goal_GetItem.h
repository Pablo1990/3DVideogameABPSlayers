#pragma once


#include "Goal_Think.h"

class Goal_Think;
class Bot;
class Goal_GetItem : public Goal_Composite
{

public:
	Goal_GetItem(Bot *p,int item);
	~Goal_GetItem(void);
	void Activate();

  int Process();

  void Terminate(){estado=completed;}

	Bot* dueño;
	int item;//numero de item a coger o 1 que es salud o 2 que es arma
};

