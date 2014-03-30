
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
	//hay que comprobar que parametros son correctos y despues cambiarlo para cada tipo de arma
	if(d->isEnemigoPresent() && ((std::abs(d->getEnem()->get_position().X-d->get_position().X)<=60 ) && std::abs(d->getEnem()->get_position().Z-d->get_position().Z)<=40))
	{
		//Añadir animación de ataque
		d->face_target(d->getEnem()->get_character_node());
		d->attack(0);
		//d->getEnem()->set_health(d->getEnem()->get_health()-5);
		//d->get_weapon()->set_resist(d->get_weapon()->get_resist()-0.01);
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