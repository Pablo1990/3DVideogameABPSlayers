
#include "Goal_GetArma.h"
#include "math.h"


Goal_GetArma::Goal_GetArma(Npc *b)
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
	cout<<"Activado Subojetivo coger una Arma"<<endl;
}

int Goal_GetArma::Process()
{
	Activate();
	vector3df pos=d->DarPosArmaCercana();
	if((std::abs(pos.X-d->get_position().X)<=25 ) && std::abs(pos.Y-d->get_position().Y)<=25)
		{

			// Se hara cada clase de arma con distintos valores de resist
			d->get_weapon()->set_resist(10);
			estado=completed;
		}
		else
		{
			estado=error;
		}
		return estado;
}
