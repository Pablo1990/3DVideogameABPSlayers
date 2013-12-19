#include "stdafx.h"
#include "Goal_GetSalud.h"


Goal_GetSalud::Goal_GetSalud(Bot * b)
{
	d=b;
}


Goal_GetSalud::~Goal_GetSalud(void)
{
}


void 
Goal_GetSalud::Activate()
{
	estado=active;
}

int Goal_GetSalud::Process()
{
	Activate();
	pair<double,double> pos=d->DarPosSalud();
		if((abs(pos.first-d->getPos().first)<=25 ) && abs(pos.second-d->getPos().second)<=25)
		{
			d->setSalud(100);
			estado=completed;
		}
		else
		{
			estado=error;
		}
		return estado;
}

