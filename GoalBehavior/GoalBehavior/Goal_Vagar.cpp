#include "stdafx.h"
#include "Goal_Vagar.h"


Goal_Vagar::Goal_Vagar(Bot * b)
{
	d=b;
}


Goal_Vagar::~Goal_Vagar(void)
{
}

void 
Goal_Vagar::Activate()
{
	estado=active;
}

int Goal_Vagar::Process()
{
	Activate();
	//Nos movemos un posicion 
	d->Move_Explore();
	Terminate();	
	return estado;
}

