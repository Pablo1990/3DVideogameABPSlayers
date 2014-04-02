
#include "Goal_GetItem.h"


Goal_GetItem::Goal_GetItem(Npc *_p,int _item)
{
	due�o=_p;
	item=_item;
}


Goal_GetItem::~Goal_GetItem(void)
{
}
void Goal_GetItem::Activate()
{
	estado=active;

	 RemoveAllSubgoals();

	if(item==1)
	{
		AddSubGoal(new Goal_GetSalud(due�o));
		AddSubGoal(new MoveToPosition(due�o,1));
		
		
	}
	else
	{
		AddSubGoal(new Goal_GetArma(due�o));
		AddSubGoal(new MoveToPosition(due�o,2));
		
		
	}
}

  int Goal_GetItem::Process()
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