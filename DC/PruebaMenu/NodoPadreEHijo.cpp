#include "NodoPadreEHijo.h"


NodoPadreEHijo::NodoPadreEHijo(NodoPathfinding n, NodoPathfinding padre)
{
	this->nodo = n;
	this->padre = padre;
}

NodoPadreEHijo::~NodoPadreEHijo(void)
{
	this->nodo.~NodoPathfinding();
	this->padre.~NodoPathfinding();
}
