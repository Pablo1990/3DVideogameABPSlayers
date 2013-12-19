#include "stdafx.h"
#include "Goal_Escape.h"


Goal_Escape::Goal_Escape(Bot * b)
{
	d=b;
}


Goal_Escape::~Goal_Escape(void)
{
}
void Goal_Escape::Activate()
{
	
	estado=active;

	if(d->isEnemigoPresent())
	{
		AddSubGoal(new Goal_MarcarHuida(d));
	}
		
}

int Goal_Escape::Process()
{

	Activate();
	  //process the subgoals
	estado= ProcessSubgoals();
	if(estado=completed && TamSubgoal()>1)
	{
		estado= ProcessSubgoals();
	}
	Terminate();
	return estado;
}
