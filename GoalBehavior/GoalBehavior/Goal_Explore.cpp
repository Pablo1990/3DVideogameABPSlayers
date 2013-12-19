#include "stdafx.h"
#include "Goal_Explore.h"


Goal_Explore::Goal_Explore(Bot* b)
{
	d=b;
}


Goal_Explore::~Goal_Explore(void)
{
}

void Goal_Explore::Activate()
{
	estado=active;


	AddSubGoal(new Goal_Vagar(d));

}

  int Goal_Explore::Process()
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
