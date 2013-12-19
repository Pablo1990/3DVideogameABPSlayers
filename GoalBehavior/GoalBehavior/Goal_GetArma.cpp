#include "stdafx.h"
#include "Goal_GetArma.h"


Goal_GetArma::Goal_GetArma(Bot *b)
{
	d=b;
}


Goal_GetArma::~Goal_GetArma(void)
{
}

void 
Goal_GetArma::Activate()
{
	estado=active;
}

int Goal_GetArma::Process()
{
	Activate();
	pair<double,double> pos=d->DarPosArmaCercana();
		if((abs(pos.first-d->getPos().first)<=25 ) && abs(pos.second-d->getPos().second)<=25)
		{
			d->setArma(10);
			estado=completed;
		}
		else
		{
			estado=error;
		}
		return estado;
}
