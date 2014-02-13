
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
		cout<<"Activado Objetivo Coger Salud"<<endl;
		AddSubGoal(new Goal_GetSalud(due�o));
		AddSubGoal(new MoveToPosition(due�o,1));
		
		
	}
	else
	{
		cout<<"Activado Objetivo Coger Arma"<<endl;
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
	cout<<"Finalizado coger arma o salud"<<endl;
	return estado;


  }