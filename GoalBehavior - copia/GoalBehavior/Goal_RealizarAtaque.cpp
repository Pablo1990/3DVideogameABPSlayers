#include "stdafx.h"
#include "Goal_RealizarAtaque.h"


Goal_RealizarAtaque::Goal_RealizarAtaque(Bot * b)
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
	if(d->isEnemigoPresent() && ((abs(d->getEnem()->getPos().first-d->getPos().first)<=25 ) && abs(d->getEnem()->getPos().second-d->getPos().second)<=25))
	{
		d->getEnem()->setSalud(d->getEnem()->getSalud()-5);
		d->setArma(d->getArma()-5);
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