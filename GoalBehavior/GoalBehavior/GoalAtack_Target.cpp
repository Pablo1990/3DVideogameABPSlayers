#include "stdafx.h"
#include "GoalAtack_Target.h"



GoalAtack_Target::GoalAtack_Target(Bot* b_)
{
	d=b_;
}


GoalAtack_Target::~GoalAtack_Target(void)
{
}

void GoalAtack_Target::Activate()
{
	estado=active;
	
	AddSubGoal(new Goal_RealizarAtaque(d));
	AddSubGoal(new MoveToPosition(d,3));
}

  int GoalAtack_Target::Process()
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


