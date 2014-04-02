
#include "Goal_MarcarHuida.h"


Goal_MarcarHuida::Goal_MarcarHuida(Npc* b)
{
	d=b;
}


Goal_MarcarHuida::~Goal_MarcarHuida(void)
{
}

void 
Goal_MarcarHuida::Activate()
{
	estado=active;
}

int Goal_MarcarHuida::Process()
{
	Activate();
	//Nos movemos un posicion 
	d->Move_ToFreeAttack();
	Terminate();	
	return estado;
}
