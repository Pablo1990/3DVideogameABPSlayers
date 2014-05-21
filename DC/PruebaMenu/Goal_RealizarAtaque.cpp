
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

	
		
}

int Goal_RealizarAtaque::Process()
{

	Activate();
	//hay que comprobar que parametros son correctos y despues cambiarlo para cada tipo de arma
	if(d->isEnemigoPresent() && ((std::abs(d->getEnem()->get_position().X-d->get_position().X)<=d->get_weapon()->get_distance() ) && std::abs(d->getEnem()->get_position().Z-d->get_position().Z)<=d->get_weapon()->get_distance()))
	{
		//Añadir animación de ataque
		d->face_target(d->getEnem()->get_character_node());
		d->attack(0);

		Terminate();
	}
	else
	{
		estado=error;

	}
	
	return estado;
}