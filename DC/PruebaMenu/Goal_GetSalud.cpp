
#include "Goal_GetSalud.h"


Goal_GetSalud::Goal_GetSalud(Npc * b)
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
	vector3df pos=d->DarPosSalud();
	if((std::abs(pos.X-d->get_position().X)<=25 ) && std::abs(pos.Z-d->get_position().Z)<=25)
		{
			//d->set_health(100);
			estado=completed;
		}
		else
		{
			estado=error;
		}
		return estado;
}

