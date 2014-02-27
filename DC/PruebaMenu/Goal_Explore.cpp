
#include "Goal_Explore.h"


Goal_Explore::Goal_Explore(Npc* b)
{
	d=b;
}


Goal_Explore::~Goal_Explore(void)
{
}

void Goal_Explore::Activate()
{
	estado=active;
	cout<<"Activado Explorar Terreno"<<endl;


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
	cout<<"Finalizado explorar"<<endl;
	Terminate();
	return estado;
  }