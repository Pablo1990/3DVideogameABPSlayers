
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
	float p1 = std::abs(pos.X-d->get_position().X);
	float p2 = std::abs(pos.Y-d->get_position().Z);
	if((std::abs(pos.X-d->get_position().X)<=100 ) && std::abs(pos.Y-d->get_position().Z)<=100)
		{

			d->pick_weapon();
			estado=completed;
		}
		else
		{
			estado=error;
		}
		return estado;
}
