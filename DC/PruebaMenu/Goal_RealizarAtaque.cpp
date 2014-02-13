
#include "Goal_RealizarAtaque.h"


Goal_RealizarAtaque::Goal_RealizarAtaque(Npc * b)
{
	d=b;
}


Goal_RealizarAtaque::~Goal_RealizarAtaque(void)
{
}
void Goal_RealizarAtaque::Activate()
{
	
	estado=active;
	cout<<"Activado Disparo de ataque"<<endl;

	
		
}

int Goal_RealizarAtaque::Process()
{

	Activate();
	if(d->isEnemigoPresent() && ((std::abs(d->getEnem()->get_position().X-d->get_position().X)<=25 ) && std::abs(d->getEnem()->get_position().Y-d->get_position().Y)<=25))
	{
		d->getEnem()->set_health(d->getEnem()->get_health()-5);
		d->get_weapon()->set_resist(d->get_weapon()->get_resist()-5);
		Terminate();
	}
	else
	{
		estado=error;
		cout<<"Tiro fallido el enemigo no esta en el blanco"<<endl;

	}
	cout<<"Finalizado Disparo de ataque"<<endl;
	
	return estado;
}