
#include "Goal_GetItem.h"


Goal_GetItem::Goal_GetItem(Npc *_p,int _item)
{
	dueño=_p;
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
		AddSubGoal(new Goal_GetSalud(dueño));
		AddSubGoal(new MoveToPosition(dueño,1));
		
		
	}
	else
	{
		cout<<"Activado Objetivo Coger Arma"<<endl;
		AddSubGoal(new Goal_GetArma(dueño));
		AddSubGoal(new MoveToPosition(dueño,2));
		
		
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