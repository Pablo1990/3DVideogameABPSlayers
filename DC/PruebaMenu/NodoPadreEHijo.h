#pragma once

#include "NodoPathfinding.h"

class NodoPadreEHijo
{
public:
	NodoPathfinding nodo;
	NodoPadreEHijo *padre;
public:
	NodoPadreEHijo();
	NodoPadreEHijo(NodoPathfinding n, NodoPadreEHijo* padre);
	NodoPadreEHijo(const NodoPadreEHijo&);
	NodoPadreEHijo(NodoPadreEHijo*);
	~NodoPadreEHijo(void);

	NodoPadreEHijo* getPadre();
	NodoPathfinding getNodo();

	void setNodo(NodoPathfinding n);
	void setPadre(NodoPadreEHijo* padre);

	NodoPadreEHijo operator=(NodoPadreEHijo* p);
	NodoPadreEHijo operator=(NodoPadreEHijo& p);
};

